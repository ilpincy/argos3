/**
 * @file <argos3/plugins/simulator/physics_engines/physx/physx_box_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "physx_box_model.h"

namespace argos {

   /****************************************/
   /****************************************/

   CPhysXBoxModel::CPhysXBoxModel(CPhysXEngine& c_engine,
                                  CBoxEntity& c_entity) :
      CPhysXStretchableObjectModel(c_engine, c_entity) {
      /* Get the half size of the entity */
      physx::PxVec3 cHalfSize(c_entity.GetSize().GetX() * 0.5f,
                              c_entity.GetSize().GetY() * 0.5f,
                              c_entity.GetSize().GetZ() * 0.5f);
      SetARGoSReferencePoint(physx::PxVec3(0.0f, 0.0f, -cHalfSize.z));
      /* Get position and orientation in this engine's representation */
      physx::PxVec3 cPos;
      CVector3ToPxVec3(GetEmbodiedEntity().GetOriginAnchor().Position, cPos);
      physx::PxQuat cOrient;
      CQuaternionToPxQuat(GetEmbodiedEntity().GetOriginAnchor().Orientation, cOrient);
      /* Create the transform
       * 1. a translation from m_cBaseCenterLocal to center of mass
       * 2. a rotation around the box base
       * 3. a translation to the final position
       */
      physx::PxTransform cTranslation1(-GetARGoSReferencePoint());
      physx::PxTransform cRotation(cOrient);
      physx::PxTransform cTranslation2(cPos);
      physx::PxTransform cFinalTrans = cTranslation2 * cRotation * cTranslation1;
      /* Create the box geometry */
      physx::PxBoxGeometry cGeometry(cHalfSize);
      /* Create the box body */
      if(GetEmbodiedEntity().IsMovable()) {
         /*
          * The box is movable
          */
         /* Create the body in its initial position and orientation */
         physx::PxRigidDynamic* pcBody =
            GetPhysXEngine().GetPhysics().createRigidDynamic(cFinalTrans);
         /* Enable CCD on the body */
         pcBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
         /* Create the shape */
         physx::PxShape* pcShape =
            pcBody->createShape(cGeometry,
                                GetPhysXEngine().GetDefaultMaterial());
         pcShape->userData = this;
         /* Set body mass */
         physx::PxRigidBodyExt::setMassAndUpdateInertia(*pcBody, m_fMass);
         /* Add body to the scene */
         GetPhysXEngine().GetScene().addActor(*pcBody);
         /* Setup the body */
         SetupBody(pcBody);
      }
      else {
         /*
          * The box is not movable
          */
         /* Create the body in its initial position and orientation */
         physx::PxRigidStatic* pcBody =
            GetPhysXEngine().GetPhysics().createRigidStatic(cFinalTrans);
         /* Create the shape */
         physx::PxShape* pcShape =
            pcBody->createShape(cGeometry,
                                GetPhysXEngine().GetDefaultMaterial());
         pcShape->userData = this;
         /* Add body to the scene */
         GetPhysXEngine().GetScene().addActor(*pcBody);
         /* Setup the body */
         SetupBody(pcBody);
      }
      /* Calculate bounding box */
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_PHYSX_OPERATIONS_ON_ENTITY(CBoxEntity, CPhysXBoxModel);

   /****************************************/
   /****************************************/

}
