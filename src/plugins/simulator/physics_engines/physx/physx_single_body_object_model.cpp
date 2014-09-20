#include "physx_single_body_object_model.h"
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPhysXSingleBodyObjectModel::CPhysXSingleBodyObjectModel(CPhysXEngine& c_engine,
                                                            CComposableEntity& c_entity) :
      CPhysXModel(c_engine, c_entity.GetComponent<CEmbodiedEntity>("body")),
      m_cEntity(c_entity),
      m_pcGenericBody(NULL) {}

   /****************************************/
   /****************************************/

   CPhysXSingleBodyObjectModel::~CPhysXSingleBodyObjectModel() {
      m_pcGenericBody->release();
   }

   /****************************************/
   /****************************************/

   void CPhysXSingleBodyObjectModel::SetupBody(physx::PxRigidActor* pc_body) {
      /* Cleanup lists of shapes and geometries */
      m_vecShapes.clear();
      /* Fill up the lists */
      size_t unShapes = pc_body->getNbShapes();
      physx::PxShape** pcShapes = new physx::PxShape*[unShapes];
      pc_body->getShapes(pcShapes, unShapes);
      for(size_t i = 0; i < unShapes; ++i) {
         m_vecShapes.push_back(pcShapes[i]);
      }
      delete[] pcShapes;
      /* Assign pointer to body */
      m_pcGenericBody = pc_body;
      /* Set the flag to say whether the body is static or dynamic */
      m_bIsDynamic = (m_pcGenericBody->is<physx::PxRigidDynamic>() != NULL);
   }

   /****************************************/
   /****************************************/

   bool CPhysXSingleBodyObjectModel::MoveTo(const CVector3& c_position,
                                            const CQuaternion& c_orientation,
                                            bool b_check_only) {
      /* Can't move a static body */
      if(! m_bIsDynamic) return false;
      /* Save old body pose */
      physx::PxTransform cOldPose(m_pcDynamicBody->getGlobalPose());
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
      m_pcDynamicBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
      /* Move the body to the target position */
      m_pcDynamicBody->setGlobalPose(cFinalTrans);
      /* Check whether the body is colliding with something */
      bool bIsColliding = IsCollidingWithSomething();
      if(b_check_only || bIsColliding) {
         /* This was only a check or a collision is detected
          * Restore the old pose
          */
         m_pcDynamicBody->setGlobalPose(cOldPose);
      }
      else {
         /* The change is accepted, update the entity */
         UpdateEntityStatus();
      }
      /* Reset the body into a dynamic actor */
      m_pcDynamicBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
      /* Return true if the move was without collisions */
      return !bIsColliding;
   }

   /****************************************/
   /****************************************/

   void CPhysXSingleBodyObjectModel::Reset() {
      /* No need to reset a static body */
      if(! m_bIsDynamic) return;
      /* Initial position and orientation from embodied entity */
      physx::PxVec3 cInitPos;
      physx::PxQuat cInitOrient;
      CVector3ToPxVec3(GetEmbodiedEntity().GetOriginAnchor().Position, cInitPos);
      CQuaternionToPxQuat(GetEmbodiedEntity().GetOriginAnchor().Orientation, cInitOrient);
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
      m_pcDynamicBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
      /* Move the body to the target position */
      m_pcDynamicBody->setGlobalPose(cFinalTrans);
      /* Reset the body into a dynamic actor */
      m_pcDynamicBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
   }

   /****************************************/
   /****************************************/

   void CPhysXSingleBodyObjectModel::CalculateBoundingBox() {
      physx::PxBounds3 cPxAABB(m_pcGenericBody->getWorldBounds());
      PxVec3ToCVector3(cPxAABB.minimum, GetBoundingBox().MinCorner);
      PxVec3ToCVector3(cPxAABB.maximum, GetBoundingBox().MaxCorner);
   }

   /****************************************/
   /****************************************/

   void CPhysXSingleBodyObjectModel::UpdateEntityStatus() {
      /* No need to update a static entity */
      if(! m_bIsDynamic) return;
      /* Update bounding box */
      CalculateBoundingBox();
      /* Get the global pose of the object */
      physx::PxTransform cTrans = m_pcGenericBody->getGlobalPose();
      /* Vector to the object base */
      /* Transform base */
      physx::PxVec3 cBaseGlobal(cTrans.rotate(m_cARGoSReferencePoint));
      cBaseGlobal += cTrans.p;
      /* Set object position into ARGoS space */
      CVector3 cPos;
      PxVec3ToCVector3(cBaseGlobal, cPos);
      GetEmbodiedEntity().GetOriginAnchor().Position = cPos;
      /* Set object orientation into ARGoS space */
      CQuaternion cOrient;
      PxQuatToCQuaternion(cTrans.q, cOrient);
      GetEmbodiedEntity().GetOriginAnchor().Orientation = cOrient;
      /* Update entity components */
      m_cEntity.UpdateComponents();
   }

   /****************************************/
   /****************************************/

   bool CPhysXSingleBodyObjectModel::IsCollidingWithSomething() const {
      /* Set query flags to accept any overlap */
      static physx::PxQueryFilterData cQueryFlags(
         physx::PxQueryFlag::eANY_HIT |
         physx::PxQueryFlag::eSTATIC  |
         physx::PxQueryFlag::eDYNAMIC |
         physx::PxQueryFlag::ePREFILTER);
      /* Get body pose transformation */
      physx::PxTransform cTrans(m_pcGenericBody->getGlobalPose());
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
      /* Perform the query
       * It returns true if anything is overlapping this object */
      for(size_t i = 0; i < m_vecShapes.size(); ++i) {
         if(GetPhysXEngine().GetScene().overlap(m_vecShapes[i]->getGeometry().any(),
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
