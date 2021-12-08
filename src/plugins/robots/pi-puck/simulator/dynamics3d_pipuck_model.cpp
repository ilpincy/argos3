/**
 * @file <argos3/plugins/robots/pi-puck/simulator/dynamics3d_pipuck_model.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_pipuck_model.h"

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_engine.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_shape_manager.h>

#include <argos3/plugins/robots/pi-puck/simulator/pipuck_differential_drive_entity.h>
#include <argos3/plugins/robots/pi-puck/simulator/pipuck_entity.h>

#include <functional>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics3DPiPuckModel::CDynamics3DPiPuckModel(CDynamics3DEngine& c_engine,
                                                  CPiPuckEntity& c_pipuck) :
      CDynamics3DMultiBodyObjectModel(c_engine, c_pipuck, 3, false),
      m_cDifferentialDriveEntity(c_pipuck.GetDifferentialDriveEntity()) {
      /* get the required collision shapes */
      std::shared_ptr<btCollisionShape> ptrBodyShape =
         CDynamics3DShapeManager::RequestCylinder(BODY_HALF_EXTENTS);
      std::shared_ptr<btCollisionShape> ptrWheelShape =
         CDynamics3DShapeManager::RequestCylinder(WHEEL_HALF_EXTENTS);
      /* calculate the inertia of the collision objects */
      btVector3 cBodyInertia;
      btVector3 cWheelInertia;
      ptrBodyShape->calculateLocalInertia(BODY_MASS, cBodyInertia);
      ptrWheelShape->calculateLocalInertia(WHEEL_MASS, cWheelInertia);
      /* calculate a btTransform that moves us from the global coordinate system to the
         local coordinate system */
      const SAnchor& sOriginAnchor = c_pipuck.GetEmbodiedEntity().GetOriginAnchor();
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
         cStartTransform * BODY_OFFSET,
         BODY_GEOMETRIC_OFFSET,
         cBodyInertia,
         BODY_MASS,
         BODY_FRICTION);
      CAbstractBody::SData sLeftWheelData(
         cStartTransform * WHEEL_OFFSET_LEFT,
         WHEEL_GEOMETRIC_OFFSET,
         cWheelInertia,
         WHEEL_MASS,
         WHEEL_FRICTION);
      CAbstractBody::SData sRightWheelData(
         cStartTransform * WHEEL_OFFSET_RIGHT,
         WHEEL_GEOMETRIC_OFFSET,
         cWheelInertia,
         WHEEL_MASS,
         WHEEL_FRICTION);
      SAnchor* psBodyAnchor = &c_pipuck.GetEmbodiedEntity().GetAnchor("body");
      SAnchor* psLeftWheelAnchor = &c_pipuck.GetEmbodiedEntity().GetAnchor("left_wheel");
      SAnchor* psRightWheelAnchor = &c_pipuck.GetEmbodiedEntity().GetAnchor("right_wheel");
      /* create the bodies */
      m_ptrBody = std::make_shared<CBase>(*this, psBodyAnchor, ptrBodyShape, sBodyData);
      m_ptrLeftWheel = std::make_shared<CLink>(*this, 0, psLeftWheelAnchor, ptrWheelShape, sLeftWheelData);
      m_ptrRightWheel = std::make_shared<CLink>(*this, 1, psRightWheelAnchor, ptrWheelShape, sRightWheelData);
      /* copy the bodies to the base class */
      m_vecBodies = {m_ptrBody, m_ptrLeftWheel, m_ptrRightWheel};
      /* synchronize with the entity with the space */
      Reset();
   }
   
   /****************************************/
   /****************************************/
   
   void CDynamics3DPiPuckModel::Reset() {
      /* reset the base class */
      CDynamics3DMultiBodyObjectModel::Reset();
      /* set up wheels */
      m_cMultiBody.setupRevolute(m_ptrLeftWheel->GetIndex(),
                                 m_ptrLeftWheel->GetData().Mass,
                                 m_ptrLeftWheel->GetData().Inertia,
                                 m_ptrBody->GetIndex(),
                                 BODY_TO_WHEEL_LEFT_JOINT_ROTATION,
                                 btVector3(0.0, 1.0, 0.0),
                                 BODY_TO_WHEEL_LEFT_JOINT_OFFSET,
                                 WHEEL_LEFT_TO_BODY_JOINT_OFFSET,
                                 true);
      m_cMultiBody.setupRevolute(m_ptrRightWheel->GetIndex(),
                                 m_ptrRightWheel->GetData().Mass,
                                 m_ptrRightWheel->GetData().Inertia,
                                 m_ptrBody->GetIndex(),
                                 BODY_TO_WHEEL_RIGHT_JOINT_ROTATION,
                                 btVector3(0.0, 1.0, 0.0),
                                 BODY_TO_WHEEL_RIGHT_JOINT_OFFSET,
                                 WHEEL_RIGHT_TO_BODY_JOINT_OFFSET,
                                 true);
      /* set up motors for the wheels */
      m_ptrLeftMotor = 
         std::make_unique<btMultiBodyJointMotor>(&m_cMultiBody,
                                                 m_ptrLeftWheel->GetIndex(),
                                                 0.0,
                                                 WHEEL_MOTOR_MAX_IMPULSE);
      m_ptrLeftMotor->setRhsClamp(WHEEL_MOTOR_CLAMP);
      m_ptrRightMotor = 
         std::make_unique<btMultiBodyJointMotor>(&m_cMultiBody,
                                                 m_ptrRightWheel->GetIndex(),
                                                 0.0,
                                                 WHEEL_MOTOR_MAX_IMPULSE);
      m_ptrRightMotor->setRhsClamp(WHEEL_MOTOR_CLAMP);
      /* Allocate memory and prepare the btMultiBody */
      m_cMultiBody.finalizeMultiDof();
      /* Synchronize with the entity in the space */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DPiPuckModel::CalculateBoundingBox() {
      btVector3 cModelAabbMin, cModelAabbMax;
      /* Initialize the bounding box with the base's AABB */
      m_ptrBody->GetShape().getAabb(m_ptrBody->GetTransform(), cModelAabbMin, cModelAabbMax);
      /* Write back the bounding box swapping the coordinate systems and the Y component */
      GetBoundingBox().MinCorner.Set(cModelAabbMin.getX(), -cModelAabbMax.getZ(), cModelAabbMin.getY());
      GetBoundingBox().MaxCorner.Set(cModelAabbMax.getX(), -cModelAabbMin.getZ(), cModelAabbMax.getY());
   }

   /****************************************/
   /****************************************/
   
   void CDynamics3DPiPuckModel::UpdateEntityStatus() {
      /* write current wheel speeds back to the simulation */
      m_cDifferentialDriveEntity.SetVelocityLeft(m_cMultiBody.getJointVel(m_ptrLeftWheel->GetIndex()) * WHEEL_HALF_EXTENTS.getX());
      m_cDifferentialDriveEntity.SetVelocityRight(-m_cMultiBody.getJointVel(m_ptrRightWheel->GetIndex()) * WHEEL_HALF_EXTENTS.getX());
      /* run the base class's implementation of this method */
      CDynamics3DMultiBodyObjectModel::UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DPiPuckModel::UpdateFromEntityStatus() {
      /* run the base class's implementation of this method */
      CDynamics3DMultiBodyObjectModel::UpdateFromEntityStatus();
      /* update joint velocities */
      m_ptrLeftMotor->setVelocityTarget(m_cDifferentialDriveEntity.GetTargetVelocityLeft() / WHEEL_HALF_EXTENTS.getX(),
                                        WHEEL_MOTOR_KD_COEFFICIENT);
      m_ptrRightMotor->setVelocityTarget(-m_cDifferentialDriveEntity.GetTargetVelocityRight() / WHEEL_HALF_EXTENTS.getX(),
                                         WHEEL_MOTOR_KD_COEFFICIENT);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DPiPuckModel::AddToWorld(btMultiBodyDynamicsWorld& c_world) {
      /* run the base class's implementation of this method */
      CDynamics3DMultiBodyObjectModel::AddToWorld(c_world);
      /* add the actuators (btMultiBodyJointMotors) constraints to the world */
      c_world.addMultiBodyConstraint(m_ptrLeftMotor.get());
      c_world.addMultiBodyConstraint(m_ptrRightMotor.get());
   }

   /****************************************/
   /****************************************/

   void CDynamics3DPiPuckModel::RemoveFromWorld(btMultiBodyDynamicsWorld& c_world) {
      /* remove the actuators (btMultiBodyJointMotors) constraints from the world */
      c_world.removeMultiBodyConstraint(m_ptrRightMotor.get());
      c_world.removeMultiBodyConstraint(m_ptrLeftMotor.get());
      /* run the base class's implementation of this method */
      CDynamics3DMultiBodyObjectModel::RemoveFromWorld(c_world);
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS3D_OPERATIONS_ON_ENTITY(CPiPuckEntity, CDynamics3DPiPuckModel);

   /****************************************/
   /****************************************/

   const btScalar CDynamics3DPiPuckModel::BODY_MASS(0.13);
   const btScalar CDynamics3DPiPuckModel::BODY_DISTANCE_FROM_FLOOR(0.00125);
   const btVector3 CDynamics3DPiPuckModel::BODY_HALF_EXTENTS(0.0362, 0.0304, 0.0362);
   const btTransform CDynamics3DPiPuckModel::BODY_OFFSET(
      btQuaternion(0.0, 0.0, 0.0, 1.0),
      btVector3(0.0,BODY_DISTANCE_FROM_FLOOR, 0.0)
   );
   const btTransform CDynamics3DPiPuckModel::BODY_GEOMETRIC_OFFSET(
      btQuaternion(0.0, 0.0, 0.0, 1.0),
      btVector3(0.0, -BODY_HALF_EXTENTS.getY(), 0.0)
   );
   const btScalar CDynamics3DPiPuckModel::WHEEL_MASS(0.02);
   const btScalar CDynamics3DPiPuckModel::WHEEL_DISTANCE_BETWEEN(0.054);
   const btVector3 CDynamics3DPiPuckModel::WHEEL_HALF_EXTENTS(0.02125, 0.0015, 0.02125);
   const btTransform CDynamics3DPiPuckModel::WHEEL_GEOMETRIC_OFFSET(
      btQuaternion(0.0, 0.0, 0.0, 1.0),
      btVector3(0.0, -WHEEL_HALF_EXTENTS.getY(), 0.0)
   );
   const btTransform CDynamics3DPiPuckModel::WHEEL_OFFSET_LEFT(
      btQuaternion(btVector3(-1,0,0), SIMD_HALF_PI),
      btVector3(0.0, WHEEL_HALF_EXTENTS.getX(), -0.5 * WHEEL_DISTANCE_BETWEEN + WHEEL_HALF_EXTENTS.getY())
   );
   const btTransform CDynamics3DPiPuckModel::WHEEL_OFFSET_RIGHT(
      btQuaternion(btVector3(1,0,0), SIMD_HALF_PI),
      btVector3(0.0, WHEEL_HALF_EXTENTS.getX(), 0.5 * WHEEL_DISTANCE_BETWEEN - WHEEL_HALF_EXTENTS.getY())
   );
   const btVector3 CDynamics3DPiPuckModel::BODY_TO_WHEEL_RIGHT_JOINT_OFFSET(0.0, -BODY_HALF_EXTENTS.getY() + 
      (WHEEL_HALF_EXTENTS.getX() - BODY_DISTANCE_FROM_FLOOR), 0.5 * WHEEL_DISTANCE_BETWEEN - WHEEL_HALF_EXTENTS.getY());
   const btVector3 CDynamics3DPiPuckModel::WHEEL_RIGHT_TO_BODY_JOINT_OFFSET(0.0, WHEEL_HALF_EXTENTS.getY(), 0.0);
   const btQuaternion CDynamics3DPiPuckModel::BODY_TO_WHEEL_RIGHT_JOINT_ROTATION(btVector3(-1,0,0), SIMD_HALF_PI);
   const btVector3 CDynamics3DPiPuckModel::BODY_TO_WHEEL_LEFT_JOINT_OFFSET(0.0, -BODY_HALF_EXTENTS.getY() + 
      (WHEEL_HALF_EXTENTS.getX() - BODY_DISTANCE_FROM_FLOOR), -0.5 * WHEEL_DISTANCE_BETWEEN + WHEEL_HALF_EXTENTS.getY());
   const btVector3 CDynamics3DPiPuckModel::WHEEL_LEFT_TO_BODY_JOINT_OFFSET(0.0, WHEEL_HALF_EXTENTS.getY(), -0.0);
   const btQuaternion CDynamics3DPiPuckModel::BODY_TO_WHEEL_LEFT_JOINT_ROTATION(btVector3(1,0,0), SIMD_HALF_PI);
   /* TODO calibrate these values */
   const btScalar CDynamics3DPiPuckModel::BODY_FRICTION(0.125);
   const btScalar CDynamics3DPiPuckModel::WHEEL_MOTOR_MAX_IMPULSE(0.05);
   const btScalar CDynamics3DPiPuckModel::WHEEL_FRICTION(2.0);
   const btScalar CDynamics3DPiPuckModel::WHEEL_MOTOR_CLAMP(5.0);
   const btScalar CDynamics3DPiPuckModel::WHEEL_MOTOR_KD_COEFFICIENT(2.0);

   /****************************************/
   /****************************************/

}
