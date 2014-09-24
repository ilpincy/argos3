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
   }

   /****************************************/
   /****************************************/

   void CPhysXMultiBodyObjectModel::MoveTo(const CVector3& c_position,
                                           const CQuaternion& c_orientation) {
   }

   /****************************************/
   /****************************************/

   void CPhysXMultiBodyObjectModel::CalculateBoundingBox() {
   }

   /****************************************/
   /****************************************/

   bool CPhysXMultiBodyObjectModel::IsCollidingWithSomething() const {
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
      /* Calculate the bounding box */
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

}
