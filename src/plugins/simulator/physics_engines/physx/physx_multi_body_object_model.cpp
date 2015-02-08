#include "physx_multi_body_object_model.h"
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPhysXMultiBodyObjectModel::CPhysXMultiBodyObjectModel(CPhysXEngine& c_engine,
                                                          CComposableEntity& c_entity,
                                                          UInt32 un_num_bodies) :
      CPhysXModel(c_engine, c_entity.GetComponent<CEmbodiedEntity>("body")),
      m_cEntity(c_entity),
      m_pcAggregate(NULL) {
      m_pcAggregate = c_engine.GetPhysics().createAggregate(un_num_bodies, false);
      c_engine.GetScene().addAggregate(*m_pcAggregate);
   }

   /****************************************/
   /****************************************/

   CPhysXMultiBodyObjectModel::~CPhysXMultiBodyObjectModel() {
      /* Get bodies */
      UInt32 unNumBodies = m_pcAggregate->getNbActors();
      physx::PxActor** ppcBodies = new physx::PxActor*[unNumBodies];
      m_pcAggregate->getActors(ppcBodies, unNumBodies);
      /* Release all bodies */
      for(UInt32 i = 0; i < unNumBodies; ++i) {
         ppcBodies[i]->release();
      }
      delete[] ppcBodies;
      /* Release aggregate */
      m_pcAggregate->release();
   }

   /****************************************/
   /****************************************/

   void CPhysXMultiBodyObjectModel::Reset() {
      /* Move the bodies to the initial position and orientation */
      MoveTo(GetEmbodiedEntity().GetOriginAnchor().Position,
             GetEmbodiedEntity().GetOriginAnchor().Orientation);
      /* Clear all forces */
      for(size_t i = 0; i < m_vecBodies.size(); ++i) {
         m_vecBodies[i].Body.clearForce();
         m_vecBodies[i].Body.clearTorque();
      }
      /* Update ARGoS entity status */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CPhysXMultiBodyObjectModel::MoveTo(const CVector3& c_position,
                                           const CQuaternion& c_orientation) {
      /* Set target position and orientation */
      physx::PxTransform cBodyTrans;
      CVector3ToPxVec3(c_position, cBodyTrans.p);
      CQuaternionToPxQuat(c_orientation, cBodyTrans.q);
      /* Go through the bodies */
      for(size_t i = 0; i < m_vecBodies.size(); ++i) {
         /* Move the actor */
         m_vecBodies[i].Body.setGlobalPose(cBodyTrans * m_vecBodies[i].Offset);
      }
      /* Update the status of the entity */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CPhysXMultiBodyObjectModel::CalculateBoundingBox() {
      if(m_vecBodies.empty()) return;
      /* Initialize the AABB to be a copy of the AABB of the first actor */
      physx::PxBounds3 cPxAABB(m_vecBodies[0].Body.getWorldBounds());
      /* Go through the bodies and grow the AABB with other bodies */
      for(size_t i = 1; i < m_vecBodies.size(); ++i) {
         cPxAABB.include(m_vecBodies[1].Body.getWorldBounds());
      }
      /* Update the ARGoS bounding box */
      PxVec3ToCVector3(cPxAABB.minimum, GetBoundingBox().MinCorner);
      PxVec3ToCVector3(cPxAABB.maximum, GetBoundingBox().MaxCorner);
   }

   /****************************************/
   /****************************************/

   bool CPhysXMultiBodyObjectModel::IsCollidingWithSomething() const {
      if(m_vecBodies.empty()) return false;
      /* Set query flags to accept any overlap */
      static physx::PxQueryFilterData cQueryFlags(
         physx::PxQueryFlag::eANY_HIT |
         physx::PxQueryFlag::eSTATIC  |
         physx::PxQueryFlag::eDYNAMIC |
         physx::PxQueryFlag::ePREFILTER);
      /*
       * Create an array containing all the shapes of this object
       */
      /* Get total size of array to create */
      size_t unTotShapes = 0;
      for(size_t i = 0; i < m_vecBodies.size(); ++i) {
         unTotShapes += m_vecBodies[i].Body.getNbShapes();
      }
      /* Create the array of shapes */
      physx::PxShape** ppcShapes = new physx::PxShape*[unTotShapes];
      /* Fill the array of shapes */
      size_t unThisBodyShapes, unLastShape = 0;
      for(size_t i = 0; i < m_vecBodies.size(); ++i) {
         unThisBodyShapes = m_vecBodies[i].Body.getNbShapes();
         m_vecBodies[i].Body.getShapes(ppcShapes + unLastShape, unThisBodyShapes);
         unLastShape += unThisBodyShapes;
      }
      /*
       * Create a filter to exclude some shapes from the overlap results
       */
      CPhysXEngine::CQueryIgnoreShapes cIgnoreShapes;
      /* Exclude ground shape */
      cIgnoreShapes.Ignore(&GetPhysXEngine().GetGroundShape());
      /* Exclude this object's shapes */
      cIgnoreShapes.Ignore(ppcShapes, unTotShapes);
      /*
       * Execute overlap queries
       */
      /* Buffer to store query results */
      physx::PxOverlapBuffer cOverlapBuf;
      /* Go through all shapes and check for overlaps */
      for(size_t i = 0; i < unTotShapes; ++i) {
         if(GetPhysXEngine().GetScene().overlap(ppcShapes[i]->getGeometry().any(),
                                                ppcShapes[i]->getActor()->getGlobalPose(),
                                                cOverlapBuf,
                                                cQueryFlags,
                                                &cIgnoreShapes)) {
            delete[] ppcShapes;
            return true;
         }
      }
      delete[] ppcShapes;
      return false;
   }

   /****************************************/
   /****************************************/

   void CPhysXMultiBodyObjectModel::AddBody(physx::PxRigidDynamic& c_body,
                                            const physx::PxTransform& c_trans) {
      /* Set userData pointer */
      c_body.userData = this;
      /* Add body to aggregate */
      m_pcAggregate->addActor(c_body);
      /* Add body to list */
      m_vecBodies.push_back(SBody(c_body, c_trans));
      /* Calculate the bounding box */
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   CPhysXMultiBodyObjectModel::SBody::SBody(physx::PxRigidDynamic& c_body,
                                            const physx::PxTransform& c_offset) :
      Body(c_body),
      Offset(c_offset) {}

   /****************************************/
   /****************************************/

}
