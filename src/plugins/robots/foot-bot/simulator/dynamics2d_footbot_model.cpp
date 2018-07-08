/**
 * @file <argos3/plugins/robots/foot-bot/simulator/dynamics2d_footbot_model.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "dynamics2d_footbot_model.h"
#include "footbot_turret_entity.h"
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_gripping.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.h>

namespace argos {

   /****************************************/
   /****************************************/

   /* P and D constants of the PD controller used for the turret position control. */
   static const Real PD_P_CONSTANT = 0.4;
   static const Real PD_D_CONSTANT = 0.2;

   static const Real FOOTBOT_RADIUS                   = 0.085036758f;
   static const Real FOOTBOT_INTERWHEEL_DISTANCE      = 0.14f;
   static const Real FOOTBOT_HEIGHT                   = 0.146899733f;

   static const Real FOOTBOT_MAX_FORCE                = 15.f;
   static const Real FOOTBOT_MAX_TORQUE               = 150.f;

   enum FOOTBOT_WHEELS {
      FOOTBOT_LEFT_WHEEL = 0,
      FOOTBOT_RIGHT_WHEEL = 1
   };

   enum ETurretModes {
      MODE_OFF,
      MODE_PASSIVE,
      MODE_SPEED_CONTROL,
      MODE_POSITION_CONTROL,
   };

   /****************************************/
   /****************************************/

   CDynamics2DFootBotModel::CDynamics2DFootBotModel(CDynamics2DEngine& c_engine,
                                                    CFootBotEntity& c_entity) :
      CDynamics2DMultiBodyObjectModel(c_engine, c_entity),
      m_cFootBotEntity(c_entity),
      m_cWheeledEntity(m_cFootBotEntity.GetWheeledEntity()),
      m_cGripperEntity(c_entity.GetGripperEquippedEntity()),
      m_cDiffSteering(c_engine,
                      FOOTBOT_MAX_FORCE,
                      FOOTBOT_MAX_TORQUE,
                      FOOTBOT_INTERWHEEL_DISTANCE,
                      c_entity.GetConfigurationNode()),
      m_pcGripper(NULL),
      m_pcGrippable(NULL),
      m_fMass(1.6f),
      m_fCurrentWheelVelocity(m_cWheeledEntity.GetWheelVelocities()),
      m_unLastTurretMode(m_cFootBotEntity.GetTurretEntity().GetMode()) {
      RegisterAnchorMethod<CDynamics2DFootBotModel>(
         GetEmbodiedEntity().GetOriginAnchor(),
         &CDynamics2DFootBotModel::UpdateOriginAnchor);
      RegisterAnchorMethod<CDynamics2DFootBotModel>(
         GetEmbodiedEntity().GetAnchor("turret"),
         &CDynamics2DFootBotModel::UpdateTurretAnchor);
      RegisterAnchorMethod<CDynamics2DFootBotModel>(
         GetEmbodiedEntity().GetAnchor("perspective_camera"),
         &CDynamics2DFootBotModel::UpdatePerspectiveCameraAnchor);
      /* Create the actual body with initial position and orientation */
      m_ptActualBaseBody =
         cpSpaceAddBody(GetDynamics2DEngine().GetPhysicsSpace(),
                        cpBodyNew(m_fMass,
                                  cpMomentForCircle(m_fMass,
                                                    0.0f,
                                                    FOOTBOT_RADIUS + FOOTBOT_RADIUS,
                                                    cpvzero)));
      const CVector3& cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
      m_ptActualBaseBody->p = cpv(cPosition.GetX(), cPosition.GetY());
      CRadians cXAngle, cYAngle, cZAngle;
      GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpBodySetAngle(m_ptActualBaseBody, cZAngle.GetValue());
      /* Create the actual body shape */
      m_ptBaseShape =
         cpSpaceAddShape(GetDynamics2DEngine().GetPhysicsSpace(),
                         cpCircleShapeNew(m_ptActualBaseBody,
                                          FOOTBOT_RADIUS,
                                          cpvzero));
      m_ptBaseShape->e = 0.0; // No elasticity
      m_ptBaseShape->u = 0.7; // Lots of friction
      /* This shape is grippable */
      m_pcGrippable = new CDynamics2DGrippable(GetEmbodiedEntity(),
                                               m_ptBaseShape);
      /* Constrain the actual base body to follow the diff steering control */
      m_cDiffSteering.AttachTo(m_ptActualBaseBody);
      /* Add the body so that the default methods work as expected */
      AddBody(m_ptActualBaseBody, cpvzero, 0, FOOTBOT_HEIGHT);
      /* Create the gripper body */      
      m_ptActualGripperBody =
         cpSpaceAddBody(GetDynamics2DEngine().GetPhysicsSpace(),
                        cpBodyNew(m_fMass / 20.0,
                                  cpMomentForCircle(m_fMass,
                                                    0.0f,
                                                    FOOTBOT_RADIUS + FOOTBOT_RADIUS,
                                                    cpvzero)));
      m_ptActualGripperBody->p = cpv(cPosition.GetX(), cPosition.GetY());
      cpBodySetAngle(m_ptActualGripperBody,
                     cZAngle.GetValue() +
                     m_cFootBotEntity.GetTurretEntity().GetRotation().GetValue());
      /* Create the gripper shape */
      cpShape* ptGripperShape = 
         cpSpaceAddShape(GetDynamics2DEngine().GetPhysicsSpace(),
                         cpCircleShapeNew(m_ptActualGripperBody,
                                          0.01f,
                                          cpv(FOOTBOT_RADIUS, 0.0f)));
      m_pcGripper = new CDynamics2DGripper(GetDynamics2DEngine(),
                                           m_cGripperEntity,
                                           ptGripperShape);
      /* Constrain the actual gripper body to follow the actual base body */
      m_ptBaseGripperLinearMotion =
         cpSpaceAddConstraint(GetDynamics2DEngine().GetPhysicsSpace(),
                              cpPivotJointNew2(m_ptActualBaseBody,
                                               m_ptActualGripperBody,
                                               cpvzero,
                                               cpvzero));
      m_ptBaseGripperAngularMotion = cpSpaceAddConstraint(GetDynamics2DEngine().GetPhysicsSpace(),
                                                          cpGearJointNew(m_ptActualBaseBody,
                                                                         m_ptActualGripperBody,
                                                                         0.0f,
                                                                         1.0f));
      m_ptBaseGripperAngularMotion->maxBias = 0.0f; /* disable joint correction */
      m_ptBaseGripperAngularMotion->maxForce = FOOTBOT_MAX_TORQUE; /* limit the dragging torque */
      /* Add the gripper body so that the default methods work as expected */
      AddBody(m_ptActualGripperBody, cpvzero, 0, FOOTBOT_HEIGHT);
      /* Switch to active mode if necessary */
      if(m_unLastTurretMode == MODE_SPEED_CONTROL ||
         m_unLastTurretMode == MODE_POSITION_CONTROL) {
         TurretActiveToPassive();
      }
   }

   /****************************************/
   /****************************************/

   CDynamics2DFootBotModel::~CDynamics2DFootBotModel() {
      delete m_pcGripper;
      delete m_pcGrippable;
      switch(m_unLastTurretMode) {
         case MODE_OFF:
         case MODE_PASSIVE:
            cpSpaceRemoveConstraint(GetDynamics2DEngine().GetPhysicsSpace(), m_ptBaseGripperLinearMotion);
            cpSpaceRemoveConstraint(GetDynamics2DEngine().GetPhysicsSpace(), m_ptBaseGripperAngularMotion);
            cpConstraintFree(m_ptBaseGripperLinearMotion);
            cpConstraintFree(m_ptBaseGripperAngularMotion);
            break;
         case MODE_POSITION_CONTROL:
         case MODE_SPEED_CONTROL:
            cpSpaceRemoveConstraint(GetDynamics2DEngine().GetPhysicsSpace(), m_ptBaseGripperLinearMotion);
            cpSpaceRemoveConstraint(GetDynamics2DEngine().GetPhysicsSpace(), m_ptGripperControlAngularMotion);
            cpConstraintFree(m_ptBaseGripperLinearMotion);
            cpConstraintFree(m_ptGripperControlAngularMotion);
            cpBodyFree(m_ptControlGripperBody);
            break;
      }
      m_cDiffSteering.Detach();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotModel::MoveTo(const CVector3& c_position,
                                        const CQuaternion& c_orientation) {
      /* Release grippers and grippees */
      m_pcGripper->Release();
      m_pcGrippable->ReleaseAll();
      /* Move robot */
      CDynamics2DMultiBodyObjectModel::MoveTo(c_position,
                                              c_orientation);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotModel::Reset() {
      /* Zero speed and applied forces of base control body */
      m_cDiffSteering.Reset();
      /* Release grippers and gripees */
      m_pcGripper->Release();
      m_pcGrippable->ReleaseAll();
      /* Switch to turret passive mode if needed */
      if(m_unLastTurretMode == MODE_SPEED_CONTROL ||
         m_unLastTurretMode == MODE_POSITION_CONTROL) {
         TurretActiveToPassive();
         m_unLastTurretMode = MODE_OFF;
         GetEmbodiedEntity().DisableAnchor("turret");
      }
      /* Reset the rest */
      CDynamics2DMultiBodyObjectModel::Reset();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotModel::CalculateBoundingBox() {
      GetBoundingBox().MinCorner.SetX(m_ptBaseShape->bb.l);
      GetBoundingBox().MinCorner.SetY(m_ptBaseShape->bb.b);
      GetBoundingBox().MinCorner.SetZ(GetDynamics2DEngine().GetElevation());
      GetBoundingBox().MaxCorner.SetX(m_ptBaseShape->bb.r);
      GetBoundingBox().MaxCorner.SetY(m_ptBaseShape->bb.t);
      GetBoundingBox().MaxCorner.SetZ(GetDynamics2DEngine().GetElevation() + FOOTBOT_HEIGHT);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotModel::UpdateFromEntityStatus() {
      /* Do we want to move? */
      if((m_fCurrentWheelVelocity[FOOTBOT_LEFT_WHEEL] != 0.0f) ||
         (m_fCurrentWheelVelocity[FOOTBOT_RIGHT_WHEEL] != 0.0f)) {
         m_cDiffSteering.SetWheelVelocity(m_fCurrentWheelVelocity[FOOTBOT_LEFT_WHEEL],
                                          m_fCurrentWheelVelocity[FOOTBOT_RIGHT_WHEEL]);
      }
      else {
         /* No, we don't want to move - zero all speeds */
         m_cDiffSteering.Reset();
      }
      /* Update turret structures if the state changed state in the last step */
      if(m_cFootBotEntity.GetTurretEntity().GetMode() != m_unLastTurretMode) {
         /* Enable or disable the anchor */
         if(m_cFootBotEntity.GetTurretEntity().GetMode() != MODE_OFF) {
            GetEmbodiedEntity().EnableAnchor("turret");
         }
         else {
            GetEmbodiedEntity().DisableAnchor("turret");
         }
         /* Manage the thing like a state machine */
         switch(m_unLastTurretMode) {
            case MODE_OFF:
            case MODE_PASSIVE:
               switch(m_cFootBotEntity.GetTurretEntity().GetMode()) {
                  case MODE_POSITION_CONTROL:
                  case MODE_SPEED_CONTROL:
                     TurretPassiveToActive();
                     break;
                  case MODE_OFF:
                  case MODE_PASSIVE:
                     break;
               }
               break;
            case MODE_SPEED_CONTROL:
            case MODE_POSITION_CONTROL:
               switch(m_cFootBotEntity.GetTurretEntity().GetMode()) {
                  case MODE_OFF:
                  case MODE_PASSIVE:
                     TurretActiveToPassive();
                     break;
                  case MODE_POSITION_CONTROL:
                  case MODE_SPEED_CONTROL:
                     break;
               }
               break;
         }
         /* Save the current mode for the next time step */
         m_unLastTurretMode = m_cFootBotEntity.GetTurretEntity().GetMode();
      }
      /* Update the turret data */
      switch(m_unLastTurretMode) {
         /* Position control mode is implemented using a PD controller */
         case MODE_POSITION_CONTROL: {
            Real fCurRotErr = NormalizedDifference(
               m_cFootBotEntity.GetTurretEntity().GetDesiredRotation(),
               NormalizedDifference(
                  CRadians(m_ptActualGripperBody->a),
                  CRadians(m_ptActualBaseBody->a))).GetValue();
            m_ptControlGripperBody->w =
               m_cDiffSteering.GetAngularVelocity() +
               (PD_P_CONSTANT * fCurRotErr +
                PD_D_CONSTANT * (fCurRotErr - m_fPreviousTurretAngleError) * GetDynamics2DEngine().GetInverseSimulationClockTick());
            m_fPreviousTurretAngleError = fCurRotErr;
            break;
         }
         case MODE_SPEED_CONTROL:
            m_ptControlGripperBody->w =
               m_cDiffSteering.GetAngularVelocity() +
               m_cFootBotEntity.GetTurretEntity().GetDesiredRotationSpeed();
            break;
         case MODE_OFF:
         case MODE_PASSIVE:
            if(m_cGripperEntity.IsGripping() &&
               m_cGripperEntity.IsLocked()) {
               m_ptBaseGripperAngularMotion->maxForce = 0.0001f; /* limit the dragging torque */
            }
            else {
               m_ptBaseGripperAngularMotion->maxForce = FOOTBOT_MAX_TORQUE; /* limit the dragging torque */
            }
            break;
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotModel::TurretPassiveToActive() {
      /* Delete constraints to actual base body */
      cpSpaceRemoveConstraint(GetDynamics2DEngine().GetPhysicsSpace(), m_ptBaseGripperAngularMotion);
      cpConstraintFree(m_ptBaseGripperAngularMotion);
      /* Create gripper control body */
      m_ptControlGripperBody = cpBodyNew(INFINITY, INFINITY);
      /* Create angular constraint from gripper control body to gripper actual body */
      m_ptGripperControlAngularMotion = cpSpaceAddConstraint(GetDynamics2DEngine().GetPhysicsSpace(),
                                                             cpGearJointNew(m_ptActualGripperBody,
                                                                            m_ptControlGripperBody,
                                                                            0.0f,
                                                                            1.0f));
      m_ptGripperControlAngularMotion->maxBias = 0.0f; /* disable joint correction */
      m_ptGripperControlAngularMotion->maxForce = FOOTBOT_MAX_TORQUE; /* limit the dragging torque */
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotModel::TurretActiveToPassive() {
      /* Delete constraint from actual gripper body to gripper control body */
      cpSpaceRemoveConstraint(GetDynamics2DEngine().GetPhysicsSpace(), m_ptGripperControlAngularMotion);
      cpConstraintFree(m_ptGripperControlAngularMotion);
      /* Delete control body */
      cpBodyFree(m_ptControlGripperBody);
      /* Create constraints from actual gripper body to actual base body */
      m_ptBaseGripperAngularMotion = cpSpaceAddConstraint(GetDynamics2DEngine().GetPhysicsSpace(),
                                                          cpGearJointNew(m_ptActualBaseBody,
                                                                         m_ptActualGripperBody,
                                                                         0.0f,
                                                                         1.0f));
      m_ptBaseGripperAngularMotion->maxBias = 0.0f; /* disable joint correction */
      m_ptBaseGripperAngularMotion->maxForce = FOOTBOT_MAX_TORQUE; /* limit the dragging torque */
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotModel::UpdateOriginAnchor(SAnchor& s_anchor) {
      s_anchor.Position.SetX(m_ptActualBaseBody->p.x);
      s_anchor.Position.SetY(m_ptActualBaseBody->p.y);
      s_anchor.Orientation.FromAngleAxis(CRadians(m_ptActualBaseBody->a), CVector3::Z);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotModel::UpdateTurretAnchor(SAnchor& s_anchor) {
      s_anchor.Position.SetX(m_ptActualGripperBody->p.x);
      s_anchor.Position.SetY(m_ptActualGripperBody->p.y);
      s_anchor.Orientation.FromAngleAxis(CRadians(m_ptActualGripperBody->a), CVector3::Z);
      s_anchor.OffsetOrientation.FromAngleAxis(
         NormalizedDifference(
            CRadians(m_ptActualGripperBody->a),
            CRadians(m_ptActualBaseBody->a)),
         CVector3::Z);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotModel::UpdatePerspectiveCameraAnchor(SAnchor& s_anchor) {
      s_anchor.Position.SetX(m_ptActualBaseBody->p.x + s_anchor.OffsetPosition.GetX());
      s_anchor.Position.SetY(m_ptActualBaseBody->p.y + s_anchor.OffsetPosition.GetY());
      s_anchor.Orientation =
         s_anchor.OffsetOrientation *
         CQuaternion(CRadians(m_ptActualBaseBody->a), CVector3::Z);
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS2D_OPERATIONS_ON_ENTITY(CFootBotEntity, CDynamics2DFootBotModel);

   /****************************************/
   /****************************************/

}
