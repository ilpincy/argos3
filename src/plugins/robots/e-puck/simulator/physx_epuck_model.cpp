/**
 * @file <argos3/plugins/ropucks/e-puck/simulator/physx_epuck_model.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "physx_epuck_model.h"
#include <argos3/plugins/ropucks/e-puck/simulator/epuck_entity.h>

namespace argos {

   /****************************************/
   /****************************************/
   
   static const Real EPUCK_MASS                = 0.4f;

   static const Real EPUCK_RADIUS              = 0.035f;
   static const Real EPUCK_INTERWHEEL_DISTANCE = 0.053f;
   static const Real EPUCK_HEIGHT              = 0.086f;

   /****************************************/
   /****************************************/
   
   CPhysXEPuckModel::CPhysXEPuckModel(CPhysXEngine& c_engine,
                                      CEPuckEntity& c_entity) :
      CPhysXSingleBodyObjectModel(c_engine, c_entity),
      m_cEPuckEntity(c_entity),
      m_fCurrentWheelVelocity(m_cWheeledEntity.GetWheelVelocities() {
      /* Calculate base center */
      SetARGoSReferencePoint(physx::PxVec3(0.0f, 0.0f, -EPUCK_HEIGHT * 0.5f));
      /* Get position and orientation in this engine's representation */
      physx::PxVec3 cPos;
      CVector3ToPxVec3(GetEmbodiedEntity().GetOriginAnchor().Position, cPos);
      physx::PxQuat cOrient;
      CQuaternionToPxQuat(GetEmbodiedEntity().GetOriginAnchor().Orientation, cOrient);
      /* Create the transform
       * 1. a translation up by half body height
       * 2. a rotation around the base
       * 3. a translation to the final position
       */
      physx::PxTransform cTranslation1(-GetARGoSReferencePoint());
      physx::PxTransform cRotation(cOrient);
      physx::PxTransform cTranslation2(cPos);
      physx::PxTransform cFinalTrans = cTranslation2 * cRotation * cTranslation1;
      /* Create cylinder geometry */
      physx::PxConvexMeshGeometry* pcGeometry =
         CreateCylinderGeometry(c_engine,
                                EPUCK_RADIUS,
                                EPUCK_HEIGHT);
      /* Create the body in its initial position and orientation */
      physx::PxRigidDynamic* pcBody =
         GetPhysXEngine().GetPhysics().createRigidDynamic(cFinalTrans);
      /* Enable CCD on the body */
      pcBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
      /* Create the shape */
      physx::PxShape* pcShape =
         pcBody->createShape(*pcGeometry,
                             GetPhysXEngine().GetDefaultMaterial());
      pcShape->userData = this;
      /* Set body mass */
      physx::PxRigidBodyExt::setMassAndUpdateInertia(*pcBody, 0.4f);
      /* Add body to the scene */
      GetPhysXEngine().GetScene().addActor(*pcBody);
      /* Set this as the body for the base class */
      SetupBody(pcBody);
      /* Cleanup */
      delete pcGeometry;
   }

   /****************************************/
   /****************************************/

   void CPhysXEPuckModel::UpdateFromEntityStatus() {
      /* Do we want to move? */
      if((m_fCurrentWheelVelocity[EPUCK_LEFT_WHEEL] != 0.0f) ||
         (m_fCurrentWheelVelocity[EPUCK_RIGHT_WHEEL] != 0.0f)) {
         m_cDiffSteering.SetWheelVelocity(m_fCurrentWheelVelocity[EPUCK_LEFT_WHEEL],
                                          m_fCurrentWheelVelocity[EPUCK_RIGHT_WHEEL]);
      }
      else {
         /* No, we don't want to move - zero all speeds */
         m_cDiffSteering.Reset();
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_PHYSX_OPERATIONS_ON_ENTITY(CEPuckEntity, CPhysXEPuckModel);

   /****************************************/
   /****************************************/

}
