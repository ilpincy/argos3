/**
 * @file <argos3/plugins/robots/e-puck/simulator/dynamics3d_epuck_model.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_epuck_model.h"

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_engine.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_shape_manager.h>

#include <argos3/plugins/simulator/entities/wheeled_entity.h>
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics3DEPuckModel::CDynamics3DEPuckModel(CDynamics3DEngine& c_engine,
                                                CEPuckEntity& c_epuck) :
      /* technically, the CDynamics3DMultiBodyObjectModel should be initialized with 7 children
         links, however, setting it to 7 makes the epuck less stable for reasons. */
      CDynamics3DMultiBodyObjectModel(c_engine, c_epuck, 3, false),
      m_cWheeledEntity(c_epuck.GetWheeledEntity()) {
      /* get the required collision shapes */
      std::shared_ptr<btCollisionShape> ptrBodyShape =
         CDynamics3DShapeManager::RequestBox(m_cBodyHalfExtents);
      std::shared_ptr<btCollisionShape> ptrWheelShape =
         CDynamics3DShapeManager::RequestCylinder(m_cWheelHalfExtents);
      /* calculate the inertia of the collision objects */
      btVector3 cBodyInertia;
      btVector3 cWheelInertia;
      ptrBodyShape->calculateLocalInertia(m_fBodyMass, cBodyInertia);
      ptrWheelShape->calculateLocalInertia(m_fWheelMass, cWheelInertia);
      /* calculate a btTransform that moves us from the global coordinate system to the
         local coordinate system */
      const SAnchor& sOriginAnchor = c_epuck.GetEmbodiedEntity().GetOriginAnchor();
      const CQuaternion& cOrientation = sOriginAnchor.Orientation;
      const CVector3& cPosition = sOriginAnchor.Position;     
      const btTransform& cStartTransform = btTransform(
         btQuaternion(cOrientation.GetX(),
                      cOrientation.GetZ(),
                     -cOrientation.GetY(),
                      cOrientation.GetW()),
         btVector3(cPosition.GetX(),
                   cPosition.GetZ(),
                  -cPosition.GetY()));
      /* create a CAbstractBody::SData structure for each body */
      CAbstractBody::SData sBodyData(
         cStartTransform * m_cBodyOffset,
         m_cBodyGeometricOffset,
         cBodyInertia,
         m_fBodyMass,
         GetEngine().GetDefaultFriction());
      CAbstractBody::SData sLeftWheelData(
         cStartTransform * m_cLeftWheelOffset,
         m_cWheelGeometricOffset,
         cWheelInertia,
         m_fWheelMass,
         m_fWheelFriction);
      CAbstractBody::SData sRightWheelData(
         cStartTransform * m_cRightWheelOffset,
         m_cWheelGeometricOffset,
         cWheelInertia,
         m_fWheelMass,
         m_fWheelFriction);
      /* create an anchor for the body (not strictly necessary but easier than
         overloading CDynamics3DMultiBodyObjectModel::UpdateOriginAnchor) */
      SAnchor* psBodyAnchor = &c_epuck.GetEmbodiedEntity().AddAnchor("body", {0.0, 0.0, 0.00125});
      /* create the bodies */
      m_ptrBody = std::make_shared<CBase>(*this, psBodyAnchor, ptrBodyShape, sBodyData);
      m_ptrLeftWheel = std::make_shared<CLink>(*this, 0, nullptr, ptrWheelShape, sLeftWheelData);
      m_ptrRightWheel = std::make_shared<CLink>(*this, 1, nullptr, ptrWheelShape, sRightWheelData);
      /* copy the bodies to the base class */
      m_vecBodies = {m_ptrBody, m_ptrLeftWheel, m_ptrRightWheel};
      /* synchronize with the entity with the space */
      Reset();
   }
   
   /****************************************/
   /****************************************/
   
   void CDynamics3DEPuckModel::Reset() {
      /* reset the base class */
      CDynamics3DMultiBodyObjectModel::Reset();
      /* set up wheels */
      m_cMultiBody.setupRevolute(m_ptrLeftWheel->GetIndex(),
                                 m_ptrLeftWheel->GetData().Mass,
                                 m_ptrLeftWheel->GetData().Inertia,
                                 m_ptrBody->GetIndex(),
                                 m_cBodyToLeftWheelJointRotation,
                                 btVector3(0.0, 1.0, 0.0),
                                 m_cBodyToLeftWheelJointOffset,
                                 m_cLeftWheelToBodyJointOffset,
                                 true);
      m_cMultiBody.setupRevolute(m_ptrRightWheel->GetIndex(),
                                 m_ptrRightWheel->GetData().Mass,
                                 m_ptrRightWheel->GetData().Inertia,
                                 m_ptrBody->GetIndex(),
                                 m_cBodyToRightWheelJointRotation,
                                 btVector3(0.0, 1.0, 0.0),
                                 m_cBodyToRightWheelJointOffset,
                                 m_cRightWheelToBodyJointOffset,
                                 true);
      /* set up motors for the wheels */
      m_ptrLeftMotor = 
         std::unique_ptr<btMultiBodyJointMotor>(
            new btMultiBodyJointMotor(&m_cMultiBody,
                                      m_ptrLeftWheel->GetIndex(),
                                      0.0,
                                      m_fWheelMotorMaxImpulse));
      m_ptrRightMotor = 
         std::unique_ptr<btMultiBodyJointMotor>(
            new btMultiBodyJointMotor(&m_cMultiBody,
                                      m_ptrRightWheel->GetIndex(),
                                      0.0,
                                      m_fWheelMotorMaxImpulse));
      /* Allocate memory and prepare the btMultiBody */
      m_cMultiBody.finalizeMultiDof();
      /* Synchronize with the entity in the space */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DEPuckModel::CalculateBoundingBox() {
      btVector3 cModelAabbMin, cModelAabbMax;
      /* Initialize the bounding box with the base's AABB */
      m_ptrBody->GetShape().getAabb(m_ptrBody->GetTransform(), cModelAabbMin, cModelAabbMax);
      /* Write back the bounding box swapping the coordinate systems and the Y component */
      GetBoundingBox().MinCorner.Set(cModelAabbMin.getX(), -cModelAabbMax.getZ(), cModelAabbMin.getY());
      GetBoundingBox().MaxCorner.Set(cModelAabbMax.getX(), -cModelAabbMin.getZ(), cModelAabbMax.getY());
   }

   /****************************************/
   /****************************************/
   
   void CDynamics3DEPuckModel::UpdateEntityStatus() {
      /* run the base class's implementation of this method */
      CDynamics3DMultiBodyObjectModel::UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DEPuckModel::UpdateFromEntityStatus() {
      /* run the base class's implementation of this method */
      CDynamics3DMultiBodyObjectModel::UpdateFromEntityStatus();
      /* update joint velocities */
      m_ptrLeftMotor->setVelocityTarget(m_cWheeledEntity.GetWheelVelocities()[0]);
      m_ptrRightMotor->setVelocityTarget(m_cWheeledEntity.GetWheelVelocities()[1]);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DEPuckModel::AddToWorld(btMultiBodyDynamicsWorld& c_world) {
      /* run the base class's implementation of this method */
      CDynamics3DMultiBodyObjectModel::AddToWorld(c_world);
      /* add the actuators (btMultiBodyJointMotors) constraints to the world */
      c_world.addMultiBodyConstraint(m_ptrLeftMotor.get());
      c_world.addMultiBodyConstraint(m_ptrRightMotor.get());
   }

   /****************************************/
   /****************************************/

   void CDynamics3DEPuckModel::RemoveFromWorld(btMultiBodyDynamicsWorld& c_world) {
      /* remove the actuators (btMultiBodyJointMotors) constraints from the world */
      c_world.removeMultiBodyConstraint(m_ptrRightMotor.get());
      c_world.removeMultiBodyConstraint(m_ptrLeftMotor.get());
      /* run the base class's implementation of this method */
      CDynamics3DMultiBodyObjectModel::RemoveFromWorld(c_world);
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS3D_OPERATIONS_ON_ENTITY(CEPuckEntity, CDynamics3DEPuckModel);

   /****************************************/
   /****************************************/

   const btVector3    CDynamics3DEPuckModel::m_cBodyHalfExtents(0.0362, 0.0236, 0.0362);
   const btScalar     CDynamics3DEPuckModel::m_fBodyMass(0.242);
   const btTransform  CDynamics3DEPuckModel::m_cBodyOffset(btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0,0.00125,0.0));
   const btTransform  CDynamics3DEPuckModel::m_cBodyGeometricOffset(btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, -0.0236, 0.0));
   const btVector3    CDynamics3DEPuckModel::m_cWheelHalfExtents(0.02125,0.0015,0.02125);
   const btScalar     CDynamics3DEPuckModel::m_fWheelMass(0.006);
   const btTransform  CDynamics3DEPuckModel::m_cWheelGeometricOffset(btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0,-0.0015,0.0));
   const btTransform  CDynamics3DEPuckModel::m_cLeftWheelOffset(btQuaternion(btVector3(-1,0,0), SIMD_HALF_PI), btVector3(0.0, 0.02125, -0.0255));
   const btTransform  CDynamics3DEPuckModel::m_cRightWheelOffset(btQuaternion(btVector3(1,0,0), SIMD_HALF_PI), btVector3(0.0, 0.02125, 0.0255));
   const btVector3    CDynamics3DEPuckModel::m_cBodyToRightWheelJointOffset(0.0, -0.0036, 0.0255);
   const btVector3    CDynamics3DEPuckModel::m_cRightWheelToBodyJointOffset(0.0, 0.0015, 0.0);
   const btQuaternion CDynamics3DEPuckModel::m_cBodyToRightWheelJointRotation(btVector3(-1,0,0), SIMD_HALF_PI);
   const btVector3    CDynamics3DEPuckModel::m_cBodyToLeftWheelJointOffset(0.0, -0.0036, -0.0255);
   const btVector3    CDynamics3DEPuckModel::m_cLeftWheelToBodyJointOffset(0.0, 0.0015, -0.0);
   const btQuaternion CDynamics3DEPuckModel::m_cBodyToLeftWheelJointRotation(btVector3(1,0,0), SIMD_HALF_PI);
   /* TODO calibrate these values */
   const btScalar     CDynamics3DEPuckModel::m_fWheelMotorMaxImpulse(0.15);
   const btScalar     CDynamics3DEPuckModel::m_fWheelFriction(5.0);

   /****************************************/
   /****************************************/

}
