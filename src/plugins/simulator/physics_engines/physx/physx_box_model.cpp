/**
 * @file <argos3/plugins/simulator/physics_engines/physx/physx_box_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "physx_box_model.h"

#include <typeinfo>

namespace argos {

   /****************************************/
   /****************************************/

   CPhysXBoxModel::CPhysXBoxModel(CPhysXEngine& c_engine,
                                  CBoxEntity& c_entity) :
      CPhysXModel(c_engine, c_entity.GetEmbodiedEntity()),
      m_cBoxEntity(c_entity),
      m_fMass(c_entity.GetMass()) {
      /* Get the half size of the entity */
      physx::PxVec3 cHalfSize(c_entity.GetSize().GetY() * 0.5f,
                              c_entity.GetSize().GetZ() * 0.5f,
                              c_entity.GetSize().GetX() * 0.5f);
      m_cBaseCenterLocal.x = 0.0f;
      m_cBaseCenterLocal.y = -cHalfSize.y;
      m_cBaseCenterLocal.z = 0.0f;
      /* Get position and orientation in this engine's representation */
      physx::PxVec3 cPos;
      CVector3ToPxVec3(GetEmbodiedEntity().GetPosition(), cPos);
      physx::PxQuat cOrient;
      CQuaternionToPxQuat(GetEmbodiedEntity().GetOrientation(), cOrient);
      /* Create the transform
       * 1. a translation up by half size on y
       * 2. a rotation around the box base
       * 3. a translation to the final position
       */
      physx::PxTransform cTranslation1(physx::PxVec3(0.0f, cHalfSize.y, 0.0f));
      physx::PxTransform cRotation(cOrient);
      physx::PxTransform cTranslation2(cPos);
      physx::PxTransform cFinalTrans = cTranslation2 * cRotation * cTranslation1;
      /* Create the box geometry */
      physx::PxBoxGeometry cBoxGeom(cHalfSize);
      /* Create the box body */
      if(c_entity.GetEmbodiedEntity().IsMovable()) {
         /*
          * The box is movable
          */
         /* Create the body in its initial position and orientation */
         m_pcDynamicBody = c_engine.GetPhysics().createRigidDynamic(cFinalTrans);
         /* Create the shape */
         physx::PxShape* pcShape = m_pcDynamicBody->createShape(cBoxGeom,
                                                                c_engine.GetDefaultMaterial());
         /* Switch continuous collision detection (CCD) on */
         pcShape->setFlag(physx::PxShapeFlag::eUSE_SWEPT_BOUNDS, true);
         /* Set body mass */
         physx::PxRigidBodyExt::setMassAndUpdateInertia(*m_pcDynamicBody, m_fMass);
         /* Add body to the scene */
         c_engine.GetScene().addActor(*m_pcDynamicBody);
      }
      else {
         /*
          * The box is not movable
          */
         /* Create the body in its initial position and orientation */
         m_pcStaticBody = c_engine.GetPhysics().createRigidStatic(cFinalTrans);
         /* Create the shape */
         m_pcStaticBody->createShape(cBoxGeom,
                                     c_engine.GetDefaultMaterial());
         /* Add body to the scene */
         c_engine.GetScene().addActor(*m_pcStaticBody);
      }
      /* Calculate bounding box */
      CalculateBoundingBox();
   }
   
   /****************************************/
   /****************************************/

   CPhysXBoxModel::~CPhysXBoxModel() {
      if(m_cBoxEntity.GetEmbodiedEntity().IsMovable()) {
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

   bool CPhysXBoxModel::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                 const CRay3& c_ray) const {
      return false;
   }

   /****************************************/
   /****************************************/

   bool CPhysXBoxModel::MoveTo(const CVector3& c_position,
                               const CQuaternion& c_orientation,
                               bool b_check_only) {
      return false;
   }

   /****************************************/
   /****************************************/

   void CPhysXBoxModel::Reset() {
   }

   /****************************************/
   /****************************************/

   void CPhysXBoxModel::CalculateBoundingBox() {
      physx::PxBounds3 cPxAABB;
      if(m_cBoxEntity.GetEmbodiedEntity().IsMovable()) {
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

   void CPhysXBoxModel::UpdateEntityStatus() {
      /* Update components */
      if(GetEmbodiedEntity().IsMovable()) {
         /* Box is movable, must be updated */
         /* Update bounding box */
         CalculateBoundingBox();
         /* Get the global pose of the box */
         physx::PxTransform cTrans = m_pcDynamicBody->getGlobalPose();
         fprintf(stderr, "[DEBUG] UPDATE cTrans.p = <%f,%f,%f>\n",
                 cTrans.p.x, cTrans.p.y, cTrans.p.z);
         fprintf(stderr, "[DEBUG] UPDATE cTrans.q = <%f,%f,%f,%f>\n",
                 cTrans.q.w, cTrans.q.x, cTrans.q.y, cTrans.q.z);
         /* Vector to the box base */
         /* Transform base */
         physx::PxVec3 cBaseGlobal(cTrans.rotate(m_cBaseCenterLocal));
         cBaseGlobal += cTrans.p;
         /* Set box position into ARGoS space */
         CVector3 cPos;
         PxVec3ToCVector3(cBaseGlobal, cPos);
         GetEmbodiedEntity().SetPosition(cPos);
         fprintf(stderr, "[DEBUG] UPDATE cPos = <%f,%f,%f>\n",
                 cPos.GetX(), cPos.GetY(), cPos.GetZ());
         CQuaternion cOrient;
         PxQuatToCQuaternion(cTrans.q, cOrient);
         GetEmbodiedEntity().SetOrientation(cOrient);
         fprintf(stderr, "[DEBUG] UPDATE cOrient = <%f,%f,%f,%f>\n\n",
                 cOrient.GetW(), cOrient.GetX(), cOrient.GetY(), cOrient.GetZ());
      }
      m_cBoxEntity.UpdateComponents();
   }

   /****************************************/
   /****************************************/

   bool CPhysXBoxModel::IsCollidingWithSomething() const {
      return false;
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_PHYSX_OPERATIONS_ON_ENTITY(CBoxEntity, CPhysXBoxModel);

   /****************************************/
   /****************************************/

}
