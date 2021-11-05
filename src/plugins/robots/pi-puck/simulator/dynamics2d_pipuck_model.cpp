/**
 * @file <argos3/plugins/robots/pi-puck/simulator/dynamics2d_pipuck_model.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics2d_pipuck_model.h"
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.h>
#include <argos3/plugins/robots/pi-puck/simulator/pipuck_differential_drive_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   const Real CDynamics2DPiPuckModel::PIPUCK_RADIUS              = 0.0362f;
   const Real CDynamics2DPiPuckModel::PIPUCK_INTERWHEEL_DISTANCE = 0.0565f;
   /* TODO calibrate these values */
   const Real CDynamics2DPiPuckModel::PIPUCK_HEIGHT              = 0.08f;
   const Real CDynamics2DPiPuckModel::PIPUCK_MASS                = 0.4f;
   const Real CDynamics2DPiPuckModel::PIPUCK_MAX_FORCE           = 1.5f;
   const Real CDynamics2DPiPuckModel::PIPUCK_MAX_TORQUE          = 1.5f;

   /****************************************/
   /****************************************/

   CDynamics2DPiPuckModel::CDynamics2DPiPuckModel(CDynamics2DEngine& c_engine,
                                                  CPiPuckEntity& c_entity) :
      CDynamics2DSingleBodyObjectModel(c_engine, c_entity),
      m_cPiPuckEntity(c_entity),
      m_cDifferentialDriveEntity(m_cPiPuckEntity.GetDifferentialDriveEntity()),
      m_cDiffSteering(c_engine,
                      PIPUCK_MAX_FORCE,
                      PIPUCK_MAX_TORQUE,
                      PIPUCK_INTERWHEEL_DISTANCE,
                      c_entity.GetConfigurationNode()) {
      /* Create the body with initial position and orientation */
      cpBody* ptBody =
         cpSpaceAddBody(GetDynamics2DEngine().GetPhysicsSpace(),
                        cpBodyNew(PIPUCK_MASS,
                                  cpMomentForCircle(PIPUCK_MASS,
                                                    0.0f,
                                                    PIPUCK_RADIUS + PIPUCK_RADIUS,
                                                    cpvzero)));
      const CVector3& cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
      ptBody->p = cpv(cPosition.GetX(), cPosition.GetY());
      CRadians cXAngle, cYAngle, cZAngle;
      GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpBodySetAngle(ptBody, cZAngle.GetValue());
      /* Create the body shape */
      cpShape* ptShape =
         cpSpaceAddShape(GetDynamics2DEngine().GetPhysicsSpace(),
                         cpCircleShapeNew(ptBody,
                                          PIPUCK_RADIUS,
                                          cpvzero));
      ptShape->e = 0.0; // No elasticity
      ptShape->u = 0.7; // Lots of friction
      /* Constrain the actual base body to follow the diff steering control */
      m_cDiffSteering.AttachTo(ptBody);
      /* Set the body so that the default methods work as expected */
      SetBody(ptBody, PIPUCK_HEIGHT);
      /* Set up callbacks so that the remaining anchors are updated */
      RegisterAnchorMethod(c_entity.GetEmbodiedEntity().GetAnchor("body"),
                           &CDynamics2DPiPuckModel::UpdateAuxiliaryAnchor);
      RegisterAnchorMethod(c_entity.GetEmbodiedEntity().GetAnchor("left_wheel"),
                           &CDynamics2DPiPuckModel::UpdateAuxiliaryAnchor);
      RegisterAnchorMethod(c_entity.GetEmbodiedEntity().GetAnchor("right_wheel"),
                           &CDynamics2DPiPuckModel::UpdateAuxiliaryAnchor);
   }

   /****************************************/
   /****************************************/

   CDynamics2DPiPuckModel::~CDynamics2DPiPuckModel() {
      m_cDiffSteering.Detach();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DPiPuckModel::Reset() {
      CDynamics2DSingleBodyObjectModel::Reset();
      m_cDiffSteering.Reset();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DPiPuckModel::UpdateFromEntityStatus() {
      /* Do we want to move? */
      Real fTargetVelocityLeft = m_cDifferentialDriveEntity.GetTargetVelocityLeft();
      Real fTargetVelocityRight = m_cDifferentialDriveEntity.GetTargetVelocityRight();
      if((fTargetVelocityLeft != 0.0f) || (fTargetVelocityRight != 0.0f)) {
         m_cDiffSteering.SetWheelVelocity(fTargetVelocityLeft,
                                          fTargetVelocityRight);
      }
      else {
         /* No, we don't want to move - zero all speeds */
         m_cDiffSteering.Reset();
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DPiPuckModel::UpdateAuxiliaryAnchor(SAnchor& s_anchor) {
      const CVector3& cOriginAnchorPosition =
         GetEmbodiedEntity().GetOriginAnchor().Position;
      const CQuaternion& cOriginAnchorOrientation =
         GetEmbodiedEntity().GetOriginAnchor().Orientation;
      s_anchor.Position = s_anchor.OffsetPosition;
      s_anchor.Position.Rotate(cOriginAnchorOrientation);
      s_anchor.Position += cOriginAnchorPosition;
      s_anchor.Orientation = cOriginAnchorOrientation *
         s_anchor.OffsetOrientation;
   }
   
   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS2D_OPERATIONS_ON_ENTITY(CPiPuckEntity, CDynamics2DPiPuckModel);

   /****************************************/
   /****************************************/

}
