namespace argos {

   /****************************************/
   /****************************************/

   template<class ENTITY>
   CPhysXStretchableObjectModel<ENTITY>::CPhysXStretchableObjectModel(CPhysXEngine& c_engine,
                                                                      ENTITY& c_entity) :
      CPhysXModel(c_engine, c_entity.GetEmbodiedEntity()),
      m_pcGeometry(NULL),
      m_cEntity(c_entity),
      m_fMass(c_entity.GetMass()) {}

   /****************************************/
   /****************************************/

   template<class ENTITY>
   CPhysXStretchableObjectModel<ENTITY>::~CPhysXStretchableObjectModel() {
      delete m_pcGeometry;
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
      cPxAABB = m_pcGenericBody->getWorldBounds();
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
      /* Get transformation */
      physx::PxTransform cTrans(m_pcGenericBody->getGlobalPose());
      /* Create buffer to store the possibly overlapping shapes */
      physx::PxShape* pcOverlappingShapes[16];
      /* Perform the query */
      physx::PxI32 nShapes = GetPhysXEngine().GetScene().overlapMultiple(*m_pcGeometry,
                                                                         cTrans,
                                                                         pcOverlappingShapes,
                                                                         16);
      if((nShapes == 0) ||
         ((nShapes == 1) && (pcOverlappingShapes[0] == m_pcShape))) {
         /* No shape overlaps the geometry, or the only shape
            that does is the object's shape itself */
         return false;
      }
      else {
         /* Other shapes overlap the given geometry, we have a collision */
         return true;
      }
   }

   /****************************************/
   /****************************************/

}
