/**
 * @file <argos3/plugins/ropucks/e-puck/simulator/physx_epuck_model.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "physx_epuck_model.h"
#include <argos3/plugins/robots/e-puck/simulator/epuck_entity.h>

namespace argos {

   /****************************************/
   /****************************************/
   
   static const Real EPUCK_INTERWHEEL_DISTANCE = 0.053f;
   static const Real EPUCK_WHEEL_RADIUS        = 0.0205f;
   static const Real EPUCK_WHEEL_THICKNESS     = 0.01f;
   static const Real EPUCK_WHEEL_MASS          = 0.05f;

   static const Real EPUCK_CHASSIS_LENGTH      = 0.055f;
   static const Real EPUCK_CHASSIS_WIDTH       = (EPUCK_INTERWHEEL_DISTANCE - EPUCK_WHEEL_THICKNESS) * 0.9f;
   static const Real EPUCK_CHASSIS_HEIGHT      = EPUCK_WHEEL_RADIUS * 2.0f;
   static const Real EPUCK_CHASSIS_ELEVATION   = 0.005f;
   static const Real EPUCK_CHASSIS_MASS        = 0.3f;

   static const Real EPUCK_BOARD_RADIUS        = 0.035f;
   static const Real EPUCK_BOARD_HEIGHT        = 0.03f;

   enum EPUCK_WHEELS {
      EPUCK_LEFT_WHEEL = 0,
      EPUCK_RIGHT_WHEEL = 1
   };

   /****************************************/
   /****************************************/
   
   CPhysXEPuckModel::CPhysXEPuckModel(CPhysXEngine& c_engine,
                                      CEPuckEntity& c_entity) :
      CPhysXMultiBodyObjectModel(c_engine, c_entity, 3),
      m_fCurrentWheelVelocity(c_entity.GetWheeledEntity().GetWheelVelocities()),
      m_cDiffDrive(*this,
                   c_engine,
                   EPUCK_INTERWHEEL_DISTANCE,
                   EPUCK_WHEEL_RADIUS,
                   EPUCK_WHEEL_THICKNESS,
                   EPUCK_WHEEL_MASS,
                   physx::PxVec3(EPUCK_CHASSIS_LENGTH,
                                 EPUCK_CHASSIS_WIDTH,
                                 EPUCK_CHASSIS_HEIGHT),
                   EPUCK_CHASSIS_ELEVATION,
                   EPUCK_CHASSIS_MASS) {
      /* Get position and orientation in this engine's representation */
      physx::PxTransform cBodyTrans;
      CVector3ToPxVec3(GetEmbodiedEntity().GetOriginAnchor().Position, cBodyTrans.p);
      CQuaternionToPxQuat(GetEmbodiedEntity().GetOriginAnchor().Orientation, cBodyTrans.q);
      /*
       * Top board
       */
      /* Create cylinder geometry */
      physx::PxConvexMeshGeometry* pcBoardGeometry =
         CreateCylinderGeometry(c_engine,
                                EPUCK_BOARD_RADIUS,
                                EPUCK_BOARD_HEIGHT);
      /* Create the shape and attach it to the differential drive component */
      physx::PxShape* pcBoardShape =
         m_cDiffDrive.GetMainBodyActor().createShape(*pcBoardGeometry,
                                                     GetPhysXEngine().GetDefaultMaterial());
      pcBoardShape->userData = this;
      /* Set offset transformation for the board */
      pcBoardShape->setLocalPose(
         physx::PxTransform(0.0f,
                            0.0f,
                            EPUCK_CHASSIS_ELEVATION +
                            EPUCK_CHASSIS_HEIGHT +
                            EPUCK_BOARD_HEIGHT * 0.5f));
      /* Place the differential drive component in its initial position */
      m_cDiffDrive.SetGlobalPose(cBodyTrans);
      /* Calculate the bounding box */
      CalculateBoundingBox();
      /* Cleanup */
      delete pcBoardGeometry;
      /* Register the origin anchor update method */
      RegisterAnchorMethod(GetEmbodiedEntity().GetOriginAnchor(),
                           &CPhysXEPuckModel::UpdateOriginAnchor);
   }

   /****************************************/
   /****************************************/

   void CPhysXEPuckModel::Reset() {
      /* Reset bodies */
      CPhysXMultiBodyObjectModel::Reset();
      /* Zero wheel speeds */
      m_cDiffDrive.SetTargetWheelLinearVelocity(0.0f, 0.0f);
   }

   /****************************************/
   /****************************************/

   void CPhysXEPuckModel::UpdateFromEntityStatus() {
      /* Do we want to move? */
      if((m_fCurrentWheelVelocity[EPUCK_LEFT_WHEEL] != 0.0f) ||
         (m_fCurrentWheelVelocity[EPUCK_RIGHT_WHEEL] != 0.0f)) {
         m_cDiffDrive.SetTargetWheelLinearVelocity
            (m_fCurrentWheelVelocity[EPUCK_LEFT_WHEEL],
             m_fCurrentWheelVelocity[EPUCK_RIGHT_WHEEL]);
      }
      else {
         /* No, we don't want to move - zero all speeds */
         m_cDiffDrive.SetTargetWheelLinearVelocity(0.0f, 0.0f);
      }
   }

   /****************************************/
   /****************************************/

   void CPhysXEPuckModel::UpdateOriginAnchor(SAnchor& s_anchor) {
      /* Get transform of the ARGoS origin anchor */
      physx::PxTransform cBodyTrans =
         m_cDiffDrive.GetMainBodyActor().getGlobalPose() *
         m_cDiffDrive.GetMainBodyOffset().getInverse();
      /* Set object position into ARGoS space */
      PxVec3ToCVector3(cBodyTrans.p, s_anchor.Position);
      /* Set object orientation into ARGoS space */
      PxQuatToCQuaternion(cBodyTrans.q, s_anchor.Orientation);
      physx::PxReal fLeftAngle;
      physx::PxReal fRightAngle;
      m_cDiffDrive.GetCurrentWheelAngles(fLeftAngle, fRightAngle);
      DEBUG("Current wheel rotation: %f, %f\n", fLeftAngle, fRightAngle);
      physx::PxReal fLeftVelocity;
      physx::PxReal fRightVelocity;
      m_cDiffDrive.GetCurrentWheelLinearVelocity(fLeftVelocity, fRightVelocity);
      DEBUG("Current wheel speed: %f, %f\n", fLeftVelocity, fRightVelocity);
      physx::PxReal fLeftTargetVelocity;
      physx::PxReal fRightTargetVelocity;
      m_cDiffDrive.GetTargetWheelLinearVelocity(fLeftTargetVelocity, fRightTargetVelocity);
      DEBUG("Target wheel speed: %f, %f\n\n", fLeftTargetVelocity, fRightTargetVelocity);
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_PHYSX_OPERATIONS_ON_ENTITY(CEPuckEntity, CPhysXEPuckModel);

   /****************************************/
   /****************************************/

}
