namespace argos {

   /****************************************/
   /****************************************/

   template<class ENTITY>
   CPhysXStretchableObjectModel<ENTITY>::CPhysXStretchableObjectModel(CPhysXEngine& c_engine,
                                                                      ENTITY& c_entity) :
      CPhysXSingleBodyObjectModel(c_engine, c_entity),
      m_fMass(c_entity.GetMass()) {}

   /****************************************/
   /****************************************/

   template<class ENTITY>
   CPhysXStretchableObjectModel<ENTITY>::~CPhysXStretchableObjectModel() {
      /* Prevent CPhysXSingleBodyObjectModel::~CPhysXSingleBodyObjectModel()
       * from deleting m_pcBody; we are going to manage deletion here
       */
      SetBody(NULL);
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
      /* Can't move a non-movable entity */
      if(! GetEmbodiedEntity().IsMovable()) {
         return false;
      }
      /* The entity is movable, execute parent class logic */
      return CPhysXSingleBodyObjectModel::MoveTo(c_position,
                                                 c_orientation,
                                                 b_check_only);
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CPhysXStretchableObjectModel<ENTITY>::Reset() {
      /* Reset makes sense only for movable entities */
      if(GetEmbodiedEntity().IsMovable()) {
         /* The entity is movable, execute parent class logic */
         CPhysXSingleBodyObjectModel::Reset();
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CPhysXStretchableObjectModel<ENTITY>::CalculateBoundingBox() {
      physx::PxBounds3 cPxAABB(m_pcGenericBody->getWorldBounds());
      PxVec3ToCVector3(cPxAABB.minimum, GetBoundingBox().MinCorner);
      PxVec3ToCVector3(cPxAABB.maximum, GetBoundingBox().MaxCorner);
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CPhysXStretchableObjectModel<ENTITY>::UpdateEntityStatus() {
      /* Can only update the state of a movable entity */
      if(GetEmbodiedEntity().IsMovable()) {
         /* The entity is movable, execute parent class logic */
         CPhysXSingleBodyObjectModel::UpdateEntityStatus();
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   bool CPhysXStretchableObjectModel<ENTITY>::IsCollidingWithSomething() const {
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
      physx::PxShape** ppcGroundShapes = new physx::PxShape*[1];
      /* Exclude ground shape */
      const_cast<CPhysXEngine&>(GetPhysXEngine()).GetGround().getShapes(ppcGroundShapes, 1);
      cIgnoreShapes.Ignore(ppcGroundShapes[0]);
      delete[] ppcGroundShapes;
      /* Exclude this object's shapes */
      for(size_t i = 0; i < GetShapes().size(); ++i) {
         cIgnoreShapes.Ignore(GetShapes()[i]);
      }
      /* Perform the query - it returns true if anything is overlapping this object */
      for(size_t i = 0; i < GetGeometries().size(); ++i) {
         if(GetPhysXEngine().GetScene().overlap(*GetGeometries()[i],
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
