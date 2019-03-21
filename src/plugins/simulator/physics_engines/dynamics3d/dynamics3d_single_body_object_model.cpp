/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_single_body_object_model.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_single_body_object_model.h"

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics3DSingleBodyObjectModel::CDynamics3DSingleBodyObjectModel(CDynamics3DEngine& c_engine,
                                                                      CComposableEntity& c_entity) :
      CDynamics3DModel(c_engine, c_entity) {
      /* Single body models only contain one body */
      m_vecBodies.reserve(1);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DSingleBodyObjectModel::MoveTo(const CVector3& c_position,
                                                 const CQuaternion& c_orientation) {
      /* Calculate the transform */
      const btTransform& cTransform = btTransform(
         btQuaternion(c_orientation.GetX(),
                      c_orientation.GetZ(),
                     -c_orientation.GetY(),
                      c_orientation.GetW()),
         btVector3(c_position.GetX(),
                   c_position.GetZ(),
                  -c_position.GetY()));
      /* Move the body */
      m_vecBodies[0]->GetTransform() = 
         cTransform * m_vecBodies[0]->GetData().InverseCenterOfMassOffset;
      /* Synchronize with the entity in the space */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DSingleBodyObjectModel::Reset() {
      m_vecBodies[0]->Reset();
      /* Synchronize with the entity in the space */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DSingleBodyObjectModel::CalculateBoundingBox() {
      btCollisionShape& cShape = m_vecBodies[0]->GetShape();
      btVector3 cAabbMin;
      btVector3 cAabbMax;
      /* Get the axis aligned bounding box for the current body */
      cShape.getAabb(m_vecBodies[0]->GetTransform(), cAabbMin, cAabbMax);
      /* Write back the bounding box swapping the coordinate systems and the Y component */
      GetBoundingBox().MinCorner.Set(cAabbMin.getX(), -cAabbMax.getZ(), cAabbMin.getY());
      GetBoundingBox().MaxCorner.Set(cAabbMax.getX(), -cAabbMin.getZ(), cAabbMax.getY());
   }

   /****************************************/
   /****************************************/

   void CDynamics3DSingleBodyObjectModel::AddToWorld(btMultiBodyDynamicsWorld& c_world) {
      m_vecBodies[0]->AddToWorld(c_world);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DSingleBodyObjectModel::RemoveFromWorld(btMultiBodyDynamicsWorld& c_world) {
      m_vecBodies[0]->RemoveFromWorld(c_world);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DSingleBodyObjectModel::CBody::Reset() {
      btRigidBody::btRigidBodyConstructionInfo cInfo(m_sData.Mass, 
                                                     nullptr,
                                                     &GetShape(),
                                                     m_sData.Inertia);
      cInfo.m_friction = m_sData.Friction;
      cInfo.m_startWorldTransform = m_sData.StartTransform *
         m_sData.InverseCenterOfMassOffset;
      /* Call the destructor */
      m_cRigidBody.~btRigidBody();
      /* Rerun the constructor */
      new (&m_cRigidBody) btRigidBody(cInfo);
      m_cRigidBody.setUserPointer(&m_cModel);
   }

   /****************************************/
   /****************************************/

   CDynamics3DSingleBodyObjectModel::CBody::CBody(CDynamics3DModel& c_model,
                                                  SAnchor* ps_anchor,
                                                  const std::shared_ptr<btCollisionShape>& ptr_shape,
                                                  const SData& s_data) :
      CAbstractBody(c_model, ps_anchor, ptr_shape, s_data),
      m_cRigidBody(0.0f, nullptr, nullptr) {}

   /****************************************/
   /****************************************/

   void CDynamics3DSingleBodyObjectModel::CBody::AddToWorld(btMultiBodyDynamicsWorld& c_world) {
      c_world.addRigidBody(&m_cRigidBody);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DSingleBodyObjectModel::CBody::RemoveFromWorld(btMultiBodyDynamicsWorld& c_world) {
      c_world.removeRigidBody(&m_cRigidBody);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DSingleBodyObjectModel::CBody::ApplyForce(const btVector3& c_force) {
      m_cRigidBody.applyCentralForce(c_force);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DSingleBodyObjectModel::CBody::ApplyForce(const btVector3& c_force,
                                                            const btVector3& c_offset) {
      m_cRigidBody.applyForce(c_force, c_offset);         
   }

   /****************************************/
   /****************************************/

   void CDynamics3DSingleBodyObjectModel::CBody::ApplyTorque(const btVector3& c_torque) {
      m_cRigidBody.applyTorque(c_torque);
   }

   /****************************************/
   /****************************************/

   btTransform& CDynamics3DSingleBodyObjectModel::CBody::GetTransform() {
      return m_cRigidBody.getWorldTransform();
   }

   /****************************************/
   /****************************************/
}
