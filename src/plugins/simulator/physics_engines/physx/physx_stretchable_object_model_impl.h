namespace argos {

   /****************************************/
   /****************************************/

   template<class ENTITY>
   CPhysXStretchableObjectModel<ENTITY>::CPhysXStretchableObjectModel(CPhysXEngine& c_engine,
                                                                      ENTITY& c_entity) :
      CPhysXModel(c_engine, c_entity.GetEmbodiedEntity()),
      m_cEntity(c_entity),
      m_fMass(c_entity.GetMass()) {}

   /****************************************/
   /****************************************/

   template<class ENTITY>
   CPhysXStretchableObjectModel<ENTITY>::~CPhysXStretchableObjectModel() {
      if(GetEmbodiedEntity().IsMovable()) {
         GetPhysXEngine().GetScene().removeActor(*m_pcDynamicBody);
         m_pcDynamicBody->release();
      }
      else {
         GetPhysXEngine().GetScene().removeActor(*m_pcStaticBody);
         m_pcStaticBody->release();
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   bool CPhysXStretchableObjectModel<ENTITY>::MoveTo(const CVector3& c_position,
                                                     const CQuaternion& c_orientation,
                                                     bool b_check_only) {
      return false;
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CPhysXStretchableObjectModel<ENTITY>::Reset() {
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CPhysXStretchableObjectModel<ENTITY>::CalculateBoundingBox() {
      physx::PxBounds3 cPxAABB;
      if(GetEmbodiedEntity().IsMovable()) {
         cPxAABB = m_pcDynamicBody->getWorldBounds();
      }
      else {
         cPxAABB = m_pcStaticBody->getWorldBounds();
      }
      PxVec3ToCVector3(cPxAABB.minimum, GetBoundingBox().MinCorner);
      PxVec3ToCVector3(cPxAABB.maximum, GetBoundingBox().MaxCorner);
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CPhysXStretchableObjectModel<ENTITY>::UpdateEntityStatus() {
      /* Update components */
      if(GetEmbodiedEntity().IsMovable()) {
         /* Object is movable, must be updated */
         /* Update bounding box */
         CalculateBoundingBox();
         /* Get the global pose of the object */
         physx::PxTransform cTrans = m_pcDynamicBody->getGlobalPose();
         /* Vector to the object base */
         /* Transform base */
         physx::PxVec3 cBaseGlobal(cTrans.rotate(m_cBaseCenterLocal));
         cBaseGlobal += cTrans.p;
         /* Set object position into ARGoS space */
         CVector3 cPos;
         PxVec3ToCVector3(cBaseGlobal, cPos);
         GetEmbodiedEntity().SetPosition(cPos);
         /* Set object orientation into ARGoS space */
         CQuaternion cOrient;
         PxQuatToCQuaternion(cTrans.q, cOrient);
         GetEmbodiedEntity().SetOrientation(cOrient);
      }
      m_cEntity.UpdateComponents();
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   bool CPhysXStretchableObjectModel<ENTITY>::IsCollidingWithSomething() const {
      return false;
   }

   /****************************************/
   /****************************************/

}
