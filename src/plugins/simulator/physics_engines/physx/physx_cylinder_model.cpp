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
      /* Get position and orientation in this engine's representation */
      physx::PxTransform cBodyTrans;
      CVector3ToPxVec3(GetEmbodiedEntity().GetOriginAnchor().Position, cBodyTrans.p);
      CQuaternionToPxQuat(GetEmbodiedEntity().GetOriginAnchor().Orientation, cBodyTrans.q);
      /* Create the shape transform */
      physx::PxTransform cShapeTrans(physx::PxVec3(0.0f, 0.0f, c_entity.GetHeight() * 0.5f));
      /* Create cylinder geometry */
      physx::PxConvexMeshGeometry* pcGeometry =
         CreateCylinderGeometry(c_engine,
                                c_entity.GetRadius(),
                                c_entity.GetHeight());
      /* Create the cylinder body */
      if(GetEmbodiedEntity().IsMovable()) {
         /*
          * The cylinder is movable
          */
         /* Create the body in its initial position and orientation */
         physx::PxRigidDynamic* pcBody =
            GetPhysXEngine().GetPhysics().createRigidDynamic(cBodyTrans);
         /* Enable CCD on the body */
         pcBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
         /* Create the shape */
         physx::PxShape* pcShape =
            pcBody->createShape(*pcGeometry,
                                GetPhysXEngine().GetDefaultMaterial());
         pcShape->setLocalPose(cShapeTrans);
         /* Set body mass */
         physx::PxRigidBodyExt::setMassAndUpdateInertia(*pcBody, m_fMass);
         /* Set this as the body for the base class */
         SetBody(*pcBody);
      }
      else {
         /*
          * The cylinder is not movable
          */
         /* Create the body in its initial position and orientation */
         physx::PxRigidStatic*
            pcBody = GetPhysXEngine().GetPhysics().createRigidStatic(cBodyTrans);
         /* Create the shape */
         physx::PxShape* pcShape =
            pcBody->createShape(*pcGeometry,
                                GetPhysXEngine().GetDefaultMaterial());
         pcShape->setLocalPose(cShapeTrans);
         /* Set this as the body for the base class */
         SetBody(*pcBody);
      }
      /* Cleanup */
      delete pcGeometry;
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_PHYSX_OPERATIONS_ON_ENTITY(CCylinderEntity, CPhysXCylinderModel);

   /****************************************/
   /****************************************/

}
