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
      /* Get position and orientation in this engine's representation */
      physx::PxTransform cBodyTrans;
      CVector3ToPxVec3(GetEmbodiedEntity().GetOriginAnchor().Position, cBodyTrans.p);
      CQuaternionToPxQuat(GetEmbodiedEntity().GetOriginAnchor().Orientation, cBodyTrans.q);
      /* Create the shape transform */
      physx::PxTransform cShapeTrans(physx::PxVec3(0.0f, 0.0f, cHalfSize.z));
      /* Create the box geometry */
      physx::PxBoxGeometry cGeometry(cHalfSize);
      /* Create the box body */
      if(GetEmbodiedEntity().IsMovable()) {
         /*
          * The box is movable
          */
         /* Create the body in its initial position and orientation */
         physx::PxRigidDynamic* pcBody =
            GetPhysXEngine().GetPhysics().createRigidDynamic(cBodyTrans);
         /* Enable CCD on the body */
         pcBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
         /* Create the shape */
         physx::PxShape* pcShape =
            pcBody->createShape(cGeometry,
                                GetPhysXEngine().GetDefaultMaterial());
         pcShape->setLocalPose(cShapeTrans);
         /* Set body mass */
         physx::PxRigidBodyExt::setMassAndUpdateInertia(*pcBody, m_fMass);
         /* Setup the body */
         SetBody(*pcBody);
      }
      else {
         /*
          * The box is not movable
          */
         /* Create the body in its initial position and orientation */
         physx::PxRigidStatic* pcBody =
            GetPhysXEngine().GetPhysics().createRigidStatic(cBodyTrans);
         /* Create the shape */
         physx::PxShape* pcShape =
            pcBody->createShape(cGeometry,
                                GetPhysXEngine().GetDefaultMaterial());
         pcShape->setLocalPose(cShapeTrans);
         /* Setup the body */
         SetBody(*pcBody);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_PHYSX_OPERATIONS_ON_ENTITY(CBoxEntity, CPhysXBoxModel);

   /****************************************/
   /****************************************/

}
