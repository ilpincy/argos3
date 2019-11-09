/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_multi_body_object_model.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_multi_body_object_model.h"

#include <argos3/core/simulator/entity/composable_entity.h>

#include <argos3/core/utility/math/matrix/transformationmatrix3.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics3DMultiBodyObjectModel::CDynamics3DMultiBodyObjectModel(CDynamics3DEngine& c_engine,
                                                                    CComposableEntity& c_entity,
                                                                    UInt32 un_num_links,
                                                                    bool b_fixed_base) :
      CDynamics3DModel(c_engine, c_entity),
      m_cMultiBody(un_num_links, 0.0f, btVector3(0,0,0), b_fixed_base, false),
      m_unNumLinks(un_num_links),
      m_bFixedBase(b_fixed_base) {
      /* Register the origin anchor update method */
      RegisterAnchorMethod(GetEmbodiedEntity().GetOriginAnchor(),
                           &CDynamics3DMultiBodyObjectModel::UpdateOriginAnchor);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::Reset() {
      /* Run the multi-body destructor */
      m_cMultiBody.~btMultiBody();
      /* Rerun the multi-body constructor */
      new (&m_cMultiBody) btMultiBody(m_unNumLinks,
                                      m_vecBodies[0]->GetData().Mass,
                                      m_vecBodies[0]->GetData().Inertia,
                                      m_bFixedBase,
                                      false);
      /* Disable collisions between bodies in the same model */
      m_cMultiBody.setHasSelfCollision(false);
      /* Reset bodies */
      for(const std::shared_ptr<CAbstractBody>& ptr_body : m_vecBodies) {
         ptr_body->Reset();
      }
      /* TODO. For the moment, we rely on the class that derives from
         this model to call UpdateEntityStatus after the joints
         have been configured. This is ugly and should be fixed
         by moving the code for joints into this class */
      /* Synchronize with the entity in the space */
      //UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::AddToWorld(btMultiBodyDynamicsWorld& c_world) {
      /* Prepare the multi-body (set up internal offsets, reserve memory) */
      m_cMultiBody.finalizeMultiDof();
      /* Add multi-body to the world */
      c_world.addMultiBody(&m_cMultiBody);
      /* Add the multi-body's links (collision objects) to the world */
      for(const std::shared_ptr<CAbstractBody>& ptr_body : m_vecBodies) {
         ptr_body->AddToWorld(c_world);
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::RemoveFromWorld(btMultiBodyDynamicsWorld& c_world) {
      /* Remove the multi-body's links (collision objects) from the world */
      for(const std::shared_ptr<CAbstractBody>& ptr_body : m_vecBodies) {
         ptr_body->RemoveFromWorld(c_world);
      }
      /* Remove multi-body from the world */
      c_world.removeMultiBody(&m_cMultiBody);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::CalculateBoundingBox() {
      btVector3 cModelAabbMin, cModelAabbMax, cBodyAabbMin, cBodyAabbMax;
      /* Initialize the bounding box with the base's AABB */
      std::shared_ptr<CAbstractBody>& ptr_base = m_vecBodies[0];
      ptr_base->GetShape().getAabb(ptr_base->GetTransform(), cModelAabbMin, cModelAabbMax);
      /* Extend AABB to include other bodies */
      for(const std::shared_ptr<CAbstractBody>& ptr_body : m_vecBodies) {
         /* Get the axis aligned bounding box for the current body */
         ptr_body->GetShape().getAabb(ptr_body->GetTransform(), cBodyAabbMin, cBodyAabbMax);
         /* Update minimum corner */
         cModelAabbMin.setX(cModelAabbMin.getX() < cBodyAabbMin.getX() ? cModelAabbMin.getX() : cBodyAabbMin.getX());
         cModelAabbMin.setY(cModelAabbMin.getY() < cBodyAabbMin.getY() ? cModelAabbMin.getY() : cBodyAabbMin.getY());
         cModelAabbMin.setZ(cModelAabbMin.getZ() < cBodyAabbMin.getZ() ? cModelAabbMin.getZ() : cBodyAabbMin.getZ());
         /* Update maximum corner */
         cModelAabbMax.setX(cModelAabbMax.getX() > cBodyAabbMax.getX() ? cModelAabbMax.getX() : cBodyAabbMax.getX());
         cModelAabbMax.setY(cModelAabbMax.getY() > cBodyAabbMax.getY() ? cModelAabbMax.getY() : cBodyAabbMax.getY());
         cModelAabbMax.setZ(cModelAabbMax.getZ() > cBodyAabbMax.getZ() ? cModelAabbMax.getZ() : cBodyAabbMax.getZ());
      }
      /* Write back the bounding box swapping the coordinate systems and the Y component */
      GetBoundingBox().MinCorner.Set(cModelAabbMin.getX(), -cModelAabbMax.getZ(), cModelAabbMin.getY());
      GetBoundingBox().MaxCorner.Set(cModelAabbMax.getX(), -cModelAabbMin.getZ(), cModelAabbMax.getY());
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::MoveTo(const CVector3& c_position,
                                                const CQuaternion& c_orientation) {
      /* Calculate the destination transform */
      const btTransform& cMoveToTransform =
         btTransform(btQuaternion(c_orientation.GetX(),
                                  c_orientation.GetZ(),
                                 -c_orientation.GetY(),
                                  c_orientation.GetW()),
                     btVector3(c_position.GetX(),
                               c_position.GetZ(),
                              -c_position.GetY()));
      /* Calculate the origin anchor transform */
      const SAnchor& sOriginAnchor = GetEmbodiedEntity().GetOriginAnchor();
      const btTransform& cOriginAnchorTransform =
         btTransform(btQuaternion(sOriginAnchor.Orientation.GetX(),
                                  sOriginAnchor.Orientation.GetZ(), 
                                 -sOriginAnchor.Orientation.GetY(),
                                  sOriginAnchor.Orientation.GetW()),
                     btVector3(sOriginAnchor.Position.GetX(),
                               sOriginAnchor.Position.GetZ(),
                              -sOriginAnchor.Position.GetY()));
      /* Move the base of the model to the destination */
      m_cMultiBody.setBaseWorldTransform(cMoveToTransform * 
         cOriginAnchorTransform.inverse() * m_cMultiBody.getBaseWorldTransform());
      /* Move the remaining bodies */
      btAlignedObjectArray<btQuaternion> vecRotations;
      btAlignedObjectArray<btVector3> vecTranslations;
      m_cMultiBody.updateCollisionObjectWorldTransforms(vecRotations, vecTranslations);
      /* Update entity state */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::UpdateOriginAnchor(SAnchor& s_anchor) {
      /* Calculate the origin anchor's position and orientation from the base link's anchor */
      const SAnchor& sBaseAnchor = m_vecBodies[0]->GetAnchor();
      /* Calculate orientation */
      CQuaternion cOriginAnchorOrientation = sBaseAnchor.OffsetOrientation.Inverse() *
         sBaseAnchor.Orientation;
      /* Calculate position */
      CVector3 cOriginAnchorPosition = -sBaseAnchor.OffsetPosition;
      cOriginAnchorPosition.Rotate(cOriginAnchorOrientation);
      cOriginAnchorPosition += sBaseAnchor.Position;
      /* Transfer to origin anchor */
      s_anchor.Position = cOriginAnchorPosition;
      s_anchor.Orientation = cOriginAnchorOrientation;
   }

   /****************************************/
   /****************************************/

   CDynamics3DMultiBodyObjectModel::CLink::CLink(CDynamics3DMultiBodyObjectModel& c_model,
                                                 UInt32 un_link_index,
                                                 SAnchor* ps_anchor,
                                                 const std::shared_ptr<btCollisionShape>& ptr_shape,
                                                 const SData& s_data) :
      CAbstractBody(c_model, ps_anchor, ptr_shape, s_data),
      m_cModel(c_model),
      m_unLinkIndex(un_link_index),
      m_cMultiBodyLink(nullptr, 0) {}

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::CLink::Reset() {
      /* Call the destructor */
      m_cMultiBodyLink.~btMultiBodyLinkCollider();
      /* Construct a new btMultiBodyLinkCollider in its place */
      btMultiBody& cMultiBody = m_cModel.GetMultiBody();
      new (&m_cMultiBodyLink) btMultiBodyLinkCollider(&cMultiBody, m_unLinkIndex);
      /* Set up the btMultiBodyLinkCollider */
      m_cMultiBodyLink.setCollisionShape(m_ptrShape.get());
      m_cMultiBodyLink.setWorldTransform(m_sData.StartTransform * 
         m_sData.InverseCenterOfMassOffset);
      m_cMultiBodyLink.setFriction(m_sData.Friction);
      m_cMultiBodyLink.setUserPointer(&m_cModel);
      cMultiBody.getLink(m_unLinkIndex).m_collider = &m_cMultiBodyLink;
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::CLink::AddToWorld(btMultiBodyDynamicsWorld& c_world) {
      c_world.addCollisionObject(&m_cMultiBodyLink,
                                 btBroadphaseProxy::DefaultFilter,
                                 btBroadphaseProxy::AllFilter);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::CLink::RemoveFromWorld(btMultiBodyDynamicsWorld& c_world) {
      c_world.removeCollisionObject(&m_cMultiBodyLink);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::CLink::ApplyForce(const btVector3& c_force) {
      btMultiBody& cMultiBody = m_cModel.GetMultiBody();
      cMultiBody.addLinkForce(m_unLinkIndex, c_force);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::CLink::ApplyForce(const btVector3& c_force,
                                                           const btVector3& c_offset) {
      btMultiBody& cMultiBody = m_cModel.GetMultiBody();
      cMultiBody.addLinkForce(m_unLinkIndex, c_force);
      cMultiBody.addLinkTorque(m_unLinkIndex, c_offset.cross(c_force));
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::CLink::ApplyTorque(const btVector3& c_torque) {
      btMultiBody& cMultiBody = m_cModel.GetMultiBody();
      cMultiBody.addLinkTorque(m_unLinkIndex, c_torque);
   }

   /****************************************/
   /****************************************/

   btTransform& CDynamics3DMultiBodyObjectModel::CLink::GetTransform() {
      return m_cMultiBodyLink.getWorldTransform();
   }

   /****************************************/
   /****************************************/

   UInt32 CDynamics3DMultiBodyObjectModel::CLink::GetIndex() const {
      return m_unLinkIndex;
   }

   /****************************************/
   /****************************************/

   CDynamics3DMultiBodyObjectModel::CBase::CBase(CDynamics3DMultiBodyObjectModel& c_model,
                                                 SAnchor* ps_anchor,
                                                 const std::shared_ptr<btCollisionShape>& ptr_shape,
                                                 const SData& s_data) :
      CLink(c_model, -1, ps_anchor, ptr_shape, s_data) {}

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::CBase::Reset() {
      /* Call the destructor */
      m_cMultiBodyLink.~btMultiBodyLinkCollider();
      /* Construct a new btMultiBodyLinkCollider in its place */
      btMultiBody& cMultiBody = m_cModel.GetMultiBody();
      new (&m_cMultiBodyLink) btMultiBodyLinkCollider(&cMultiBody, -1);
      /* Set up the btMultiBodyLinkCollider */
      m_cMultiBodyLink.setCollisionShape(m_ptrShape.get());
      m_cMultiBodyLink.setWorldTransform(m_sData.StartTransform * 
         m_sData.InverseCenterOfMassOffset);
      m_cMultiBodyLink.setFriction(m_sData.Friction);
      m_cMultiBodyLink.setUserPointer(&m_cModel);
      /* Set up the base */
      cMultiBody.setBaseWorldTransform(m_sData.StartTransform * m_sData.InverseCenterOfMassOffset);
      cMultiBody.setBaseCollider(&m_cMultiBodyLink);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::CBase::ApplyForce(const btVector3& c_force) {
      btMultiBody& cMultiBody = m_cModel.GetMultiBody();
      cMultiBody.addBaseForce(c_force);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::CBase::ApplyForce(const btVector3& c_force,
                                                           const btVector3& c_offset) {
      btMultiBody& cMultiBody = m_cModel.GetMultiBody();
      cMultiBody.addBaseForce(c_force);
      cMultiBody.addBaseTorque(c_offset.cross(c_force));
   }

   /****************************************/
   /****************************************/

   void CDynamics3DMultiBodyObjectModel::CBase::ApplyTorque(const btVector3& c_torque) {
      btMultiBody& cMultiBody = m_cModel.GetMultiBody();
      cMultiBody.addBaseTorque(c_torque);
   }

   /****************************************/
   /****************************************/
   
}

