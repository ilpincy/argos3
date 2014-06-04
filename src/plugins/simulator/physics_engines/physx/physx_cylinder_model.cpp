/**
 * @file <argos3/plugins/simulator/physics_engines/physx/physx_cylinder_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "physx_cylinder_model.h"

namespace argos {

   /****************************************/
   /****************************************/

   CPhysXCylinderModel::CPhysXCylinderModel(CPhysXEngine& c_engine,
                                            CCylinderEntity& c_entity) :
      CPhysXStretchableObjectModel(c_engine, c_entity) {
      /* Set center of cylinder base */
      SetARGoSReferencePoint(physx::PxVec3(0.0f, 0.0f, -c_entity.GetHeight() * 0.5f));
      /* Get position and orientation in this engine's representation */
      physx::PxVec3 cPos;
      CVector3ToPxVec3(GetEmbodiedEntity().GetPosition(), cPos);
      physx::PxQuat cOrient;
      CQuaternionToPxQuat(GetEmbodiedEntity().GetOrientation(), cOrient);
      /* Create the transform
       * 1. a translation from m_cBaseCenterLocal to center of mass
       * 2. a rotation around the box base
       * 3. a translation to the final position
       */
      physx::PxTransform cTranslation1(-GetARGoSReferencePoint());
      physx::PxTransform cRotation(cOrient);
      physx::PxTransform cTranslation2(cPos);
      physx::PxTransform cFinalTrans = cTranslation2 * cRotation * cTranslation1;
      /* Create cylinder geometry */
      GetGeometries().push_back(
         CreateCylinderGeometry(c_engine,
                                c_entity.GetRadius(),
                                c_entity.GetHeight()));
      /* Create the cylinder body */
      if(GetEmbodiedEntity().IsMovable()) {
         /*
          * The cylinder is movable
          */
         /* Create the body in its initial position and orientation */
         m_pcDynamicBody = GetPhysXEngine().GetPhysics().createRigidDynamic(cFinalTrans);
         /* Enable CCD on the body */
         m_pcDynamicBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
         /* Create the shape */
         GetShapes().push_back(m_pcDynamicBody->createShape(*GetGeometries().back(),
                                                            GetPhysXEngine().GetDefaultMaterial()));
         /* Set body mass */
         physx::PxRigidBodyExt::setMassAndUpdateInertia(*m_pcDynamicBody, m_fMass);
         /* Add body to the scene */
         GetPhysXEngine().GetScene().addActor(*m_pcDynamicBody);
         /* Set this as the body for the base class */
         SetBody(m_pcDynamicBody);
      }
      else {
         /*
          * The cylinder is not movable
          */
         /* Create the body in its initial position and orientation */
         m_pcStaticBody = GetPhysXEngine().GetPhysics().createRigidStatic(cFinalTrans);
         /* Create the shape */
         GetShapes().push_back(m_pcStaticBody->createShape(*GetGeometries().back(),
                                                           GetPhysXEngine().GetDefaultMaterial()));
         /* Add body to the scene */
         GetPhysXEngine().GetScene().addActor(*m_pcStaticBody);
      }
      /* Assign the user data pointer to this model */
      GetShapes().back()->userData = this;
      /* Calculate bounding box */
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_PHYSX_OPERATIONS_ON_ENTITY(CCylinderEntity, CPhysXCylinderModel);

   /****************************************/
   /****************************************/

}
