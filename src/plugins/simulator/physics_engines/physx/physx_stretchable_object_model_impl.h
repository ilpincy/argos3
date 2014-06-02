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
      /* Moving is allowed only if the object is movable */
      if(! GetEmbodiedEntity().IsMovable()) {
         return false;
      }
      else {
         /* Save old body pose */
         physx::PxTransform cOldPose(m_pcDynamicBody->getGlobalPose());
         /* Set target position and orientation */
         physx::PxVec3 cPos;
         physx::PxQuat cOrient;
         CVector3ToPxVec3(c_position, cPos);
         CQuaternionToPxQuat(c_orientation, cOrient);
         /* Create the transform
          * 1. a translation from m_cBaseCenterLocal to center of mass
          * 2. a rotation around the box base
          * 3. a translation to the final position
          */
         physx::PxTransform cTranslation1(-m_cBaseCenterLocal);
         physx::PxTransform cRotation(cOrient);
         physx::PxTransform cTranslation2(cPos);
         physx::PxTransform cFinalTrans = cTranslation2 * cRotation * cTranslation1;
         /* Make the body into a kinematic actor to move it */
         m_pcDynamicBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
         /* Move the body to the target position */
         m_pcDynamicBody->setGlobalPose(cFinalTrans);
         /* Check whether the body is colliding with something */
         bool bIsColliding = IsCollidingWithSomething();
         /* If this was only a check, or a collision is detected, restore the old pose */
         if(b_check_only || bIsColliding) {
            m_pcDynamicBody->setGlobalPose(cOldPose);
         }
         else {
            UpdateEntityStatus();
         }
         /* Reset the body into a dynamic actor */
         m_pcDynamicBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
         /* Return true if the move was without collisions */
         return !bIsColliding;
      }
   }

   /****************************************/
   /****************************************/

   template<class ENTITY>
   void CPhysXStretchableObjectModel<ENTITY>::Reset() {
      /* Resetting is necessary only if the object is movable */
      if(GetEmbodiedEntity().IsMovable()) {
         /* Initial position and orientation from embodied entity */
         physx::PxVec3 cInitPos;
         physx::PxQuat cInitOrient;
         CVector3ToPxVec3(GetEmbodiedEntity().GetInitPosition(), cInitPos);
         CQuaternionToPxQuat(GetEmbodiedEntity().GetInitOrientation(), cInitOrient);
         /* Create the transform
          * 1. a translation from m_cBaseCenterLocal to center of mass
          * 2. a rotation around the box base
          * 3. a translation to the final position
          */
         physx::PxTransform cTranslation1(-m_cBaseCenterLocal);
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
      /* Create the filter to exclude this object's shape and the ground
         from the overlap results */
      CPhysXEngine::CQueryIgnoreShapes cIgnoreShapes;
      cIgnoreShapes.Ignore(m_pcShape);
      physx::PxShape** ppcGroundShapes = new physx::PxShape*[1];
      const_cast<CPhysXEngine&>(GetPhysXEngine()).GetGround().getShapes(ppcGroundShapes, 1);
      cIgnoreShapes.Ignore(ppcGroundShapes[0]);
      delete[] ppcGroundShapes;
      /* Perform the query - it returns true if anything is overlapping this object */
      return GetPhysXEngine().GetScene().overlap(*m_pcGeometry,
                                                 cTrans,
                                                 cOverlapBuf,
                                                 cQueryFlags,
                                                 &cIgnoreShapes);
   }

   /****************************************/
   /****************************************/

}
