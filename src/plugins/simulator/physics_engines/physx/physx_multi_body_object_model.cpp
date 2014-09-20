#include "physx_multi_body_object_model.h"
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPhysXMultiBodyObjectModel::CPhysXMultiBodyObjectModel(CPhysXEngine& c_engine,
                                                          CComposableEntity& c_entity) :
      CPhysXModel(c_engine, c_entity.GetComponent<CEmbodiedEntity>("body")),
      m_cEntity(c_entity) {}

   /****************************************/
   /****************************************/

   CPhysXMultiBodyObjectModel::~CPhysXMultiBodyObjectModel() {
   }

   /****************************************/
   /****************************************/

   bool CPhysXMultiBodyObjectModel::MoveTo(const CVector3& c_position,
                                           const CQuaternion& c_orientation,
                                           bool b_check_only) {
      /* Save old body pose */
      physx::PxTransform cOldPose(m_pcBody->getGlobalPose());
      /* Set target position and orientation */
      physx::PxVec3 cPos;
      physx::PxQuat cOrient;
      CVector3ToPxVec3(c_position, cPos);
      CQuaternionToPxQuat(c_orientation, cOrient);
      /* Create the transform
       * 1. a translation from m_cARGoSReferencePoint to center of mass
       * 2. a rotation around the box base
       * 3. a translation to the final position
       */
      physx::PxTransform cTranslation1(-m_cARGoSReferencePoint);
      physx::PxTransform cRotation(cOrient);
      physx::PxTransform cTranslation2(cPos);
      physx::PxTransform cFinalTrans = cTranslation2 * cRotation * cTranslation1;
      /* Make the body into a kinematic actor to move it */
      m_pcBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
      /* Move the body to the target position */
      m_pcBody->setGlobalPose(cFinalTrans);
      /* Check whether the body is colliding with something */
      bool bIsColliding = IsCollidingWithSomething();
      if(b_check_only || bIsColliding) {
         /* This was only a check or a collision is detected
          * Restore the old pose
          */
         m_pcBody->setGlobalPose(cOldPose);
      }
      else {
         /* The change is accepted, update the entity */
         UpdateEntityStatus();
      }
      /* Reset the body into a dynamic actor */
      m_pcBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
      /* Return true if the move was without collisions */
      return !bIsColliding;
   }

   /****************************************/
   /****************************************/

   void CPhysXMultiBodyObjectModel::Reset() {
      /* Initial position and orientation from embodied entity */
      physx::PxVec3 cInitPos;
      physx::PxQuat cInitOrient;
      CVector3ToPxVec3(GetEmbodiedEntity().GetInitPosition(), cInitPos);
      CQuaternionToPxQuat(GetEmbodiedEntity().GetInitOrientation(), cInitOrient);
      /* Create the transform
       * 1. a translation from m_cARGoSReferencePoint to center of mass
       * 2. a rotation around the box base
       * 3. a translation to the final position
       */
      physx::PxTransform cTranslation1(-m_cARGoSReferencePoint);
      physx::PxTransform cRotation(cInitOrient);
      physx::PxTransform cTranslation2(cInitPos);
      physx::PxTransform cFinalTrans = cTranslation2 * cRotation * cTranslation1;
      /* Make the body into a kinematic actor to move it */
      m_pcBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
      /* Move the body to the target position */
      m_pcBody->setGlobalPose(cFinalTrans);
      /* Reset the body into a dynamic actor */
      m_pcBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
   }

   /****************************************/
   /****************************************/

   void CPhysXMultiBodyObjectModel::CalculateBoundingBox() {
      physx::PxBounds3 cPxAABB(m_pcBody->getWorldBounds());
      PxVec3ToCVector3(cPxAABB.minimum, GetBoundingBox().MinCorner);
      PxVec3ToCVector3(cPxAABB.maximum, GetBoundingBox().MaxCorner);
   }

   /****************************************/
   /****************************************/

   void CPhysXMultiBodyObjectModel::UpdateEntityStatus() {
      /* Update bounding box */
      CalculateBoundingBox();
      /* Get the global pose of the object */
      physx::PxTransform cTrans = m_pcBody->getGlobalPose();
      /* Vector to the object base */
      /* Transform base */
      physx::PxVec3 cBaseGlobal(cTrans.rotate(m_cARGoSReferencePoint));
      cBaseGlobal += cTrans.p;
      /* Set object position into ARGoS space */
      CVector3 cPos;
      PxVec3ToCVector3(cBaseGlobal, cPos);
      GetEmbodiedEntity().SetPosition(cPos);
      /* Set object orientation into ARGoS space */
      CQuaternion cOrient;
      PxQuatToCQuaternion(cTrans.q, cOrient);
      GetEmbodiedEntity().SetOrientation(cOrient);
      /* Update entity components */
      m_cEntity.UpdateComponents();
   }

   /****************************************/
   /****************************************/

   bool CPhysXMultiBodyObjectModel::IsCollidingWithSomething() const {
      /* Set query flags to accept any overlap */
      static physx::PxQueryFilterData cQueryFlags(
         physx::PxQueryFlag::eANY_HIT |
         physx::PxQueryFlag::eSTATIC  |
         physx::PxQueryFlag::eDYNAMIC |
         physx::PxQueryFlag::ePREFILTER);
      /* Get body pose transformation */
      physx::PxTransform cTrans(m_pcBody->getGlobalPose());
      /* Buffer to store query results */
      physx::PxOverlapBuffer cOverlapBuf;
      /*
       * Create a filter to exclude some shapes from the overlap results
       */
      CPhysXEngine::CQueryIgnoreShapes cIgnoreShapes;
      /* Exclude ground shape */
      cIgnoreShapes.Ignore(&GetPhysXEngine().GetGroundShape());
      /* Exclude this object's shapes */
      for(size_t i = 0; i < m_vecShapes.size(); ++i) {
         cIgnoreShapes.Ignore(m_vecShapes[i]);
      }
      /* Perform the query - it returns true if anything is overlapping this object */
      for(size_t i = 0; i < m_vecGeometries.size(); ++i) {
         if(GetPhysXEngine().GetScene().overlap(*m_vecGeometries[i],
                                                cTrans,
                                                cOverlapBuf,
                                                cQueryFlags,
                                                &cIgnoreShapes)) {
            return true;
         }
      }
      return false;
   }

   /****************************************/
   /****************************************/

}
