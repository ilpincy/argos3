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
   static const Real FOOTBOT_MAX_TORQUE               = 15.f;

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
      CDynamics2DModel(c_engine, c_entity.GetEmbodiedEntity()),
      m_cFootBotEntity(c_entity),
      m_cWheeledEntity(m_cFootBotEntity.GetWheeledEntity()),
      m_cGripperEntity(c_entity.GetGripperEquippedEntity()),
      m_cDiffSteering(c_engine,
                      FOOTBOT_MAX_FORCE,
                      FOOTBOT_MAX_TORQUE,
                      FOOTBOT_INTERWHEEL_DISTANCE),
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
         &CDynamics2DFootBotModel::UpdateOriginAnchor);
      /* Create the actual body with initial position and orientation */
      m_ptActualBaseBody =
         cpSpaceAddBody(m_cDyn2DEngine.GetPhysicsSpace(),
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
         cpSpaceAddShape(m_cDyn2DEngine.GetPhysicsSpace(),
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
      /* Create the gripper body */      
      m_ptActualGripperBody =
         cpSpaceAddBody(m_cDyn2DEngine.GetPhysicsSpace(),
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
      m_ptGripperShape = 
         cpSpaceAddShape(m_cDyn2DEngine.GetPhysicsSpace(),
                         cpCircleShapeNew(m_ptActualGripperBody,
                                          0.01f,
                                          cpv(FOOTBOT_RADIUS, 0.0f)));
      m_pcGripper = new CDynamics2DGripper(m_cDyn2DEngine,
                                           m_cGripperEntity,
                                           m_ptGripperShape);
      /* Constrain the actual gripper body to follow the actual base body */
      m_ptBaseGripperLinearMotion =
         cpSpaceAddConstraint(m_cDyn2DEngine.GetPhysicsSpace(),
                              cpPivotJointNew2(m_ptActualBaseBody,
                                               m_ptActualGripperBody,
                                               cpvzero,
                                               cpvzero));
      m_ptBaseGripperAngularMotion = cpSpaceAddConstraint(m_cDyn2DEngine.GetPhysicsSpace(),
                                                          cpGearJointNew(m_ptActualBaseBody,
                                                                         m_ptActualGripperBody,
                                                                         0.0f,
                                                                         1.0f));
      m_ptBaseGripperAngularMotion->maxBias = 0.0f; /* disable joint correction */
      m_ptBaseGripperAngularMotion->maxForce = FOOTBOT_MAX_TORQUE; /* limit the dragging torque */
      /* Associate this model to the body data for ray queries */
      m_ptActualBaseBody->data = this;
      m_ptActualGripperBody->data = this;
      /* Switch to active mode if necessary */
      if(m_unLastTurretMode == MODE_SPEED_CONTROL ||
         m_unLastTurretMode == MODE_POSITION_CONTROL) {
         TurretActiveToPassive();
      }
      /* Calculate bounding box */
      GetBoundingBox().MinCorner.SetZ(GetEmbodiedEntity().GetOriginAnchor().Position.GetZ());
      GetBoundingBox().MaxCorner.SetZ(GetEmbodiedEntity().GetOriginAnchor().Position.GetZ() + FOOTBOT_HEIGHT);
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   CDynamics2DFootBotModel::~CDynamics2DFootBotModel() {
      delete m_pcGripper;
      delete m_pcGrippable;
      switch(m_unLastTurretMode) {
         case MODE_OFF:
         case MODE_PASSIVE:
            cpSpaceRemoveConstraint(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseGripperLinearMotion);
            cpSpaceRemoveConstraint(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseGripperAngularMotion);
            cpSpaceRemoveBody(m_cDyn2DEngine.GetPhysicsSpace(), m_ptActualGripperBody);
            cpSpaceRemoveShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptGripperShape);
            cpConstraintFree(m_ptBaseGripperLinearMotion);
            cpConstraintFree(m_ptBaseGripperAngularMotion);
            cpShapeFree(m_ptGripperShape);
            cpBodyFree(m_ptActualGripperBody);
            break;
         case MODE_POSITION_CONTROL:
         case MODE_SPEED_CONTROL:
            cpSpaceRemoveConstraint(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseGripperLinearMotion);
            cpSpaceRemoveConstraint(m_cDyn2DEngine.GetPhysicsSpace(), m_ptGripperControlAngularMotion);
            cpSpaceRemoveBody(m_cDyn2DEngine.GetPhysicsSpace(), m_ptActualGripperBody);
            cpSpaceRemoveShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptGripperShape);
            cpConstraintFree(m_ptBaseGripperLinearMotion);
            cpConstraintFree(m_ptGripperControlAngularMotion);
            cpShapeFree(m_ptGripperShape);
            cpBodyFree(m_ptActualGripperBody);
            cpBodyFree(m_ptControlGripperBody);
            break;
      }
      m_cDiffSteering.Detach();
      cpSpaceRemoveBody(m_cDyn2DEngine.GetPhysicsSpace(), m_ptActualBaseBody);
      cpSpaceRemoveShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseShape);
      cpShapeFree(m_ptBaseShape);
      cpBodyFree(m_ptActualBaseBody);
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DFootBotModel::MoveTo(const CVector3& c_position,
                                         const CQuaternion& c_orientation,
                                         bool b_check_only) {
      /* Save body position and orientation */
      cpVect tOldPos = m_ptActualBaseBody->p;
      cpFloat fOldA = m_ptActualBaseBody->a;
      /* Move the body to the desired position */
      m_ptActualBaseBody->p = cpv(c_position.GetX(), c_position.GetY());
      CRadians cXAngle, cYAngle, cZAngle;
      c_orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpBodySetAngle(m_ptActualBaseBody, cZAngle.GetValue());
      /* Create a shape sensor to test the movement */
      cpShape* ptTestShape = cpCircleShapeNew(m_ptActualBaseBody,
                                              FOOTBOT_RADIUS,
                                              cpvzero);
      /* Check if there is a collision */
      SInt32 nCollision = cpSpaceShapeQuery(m_cDyn2DEngine.GetPhysicsSpace(), ptTestShape, NULL, NULL);
      /* Dispose of the sensor shape */
      cpShapeFree(ptTestShape);
      /* Should we keep this movement? */
      if(b_check_only || nCollision) {
         /*
          * No, because it was only a check or there was a collision
          * Restore old body state
          */
         m_ptActualBaseBody->p = tOldPos;
         cpBodySetAngle(m_ptActualBaseBody, fOldA);
      }
      else {
         /*
          * It wasn't a check and there were no collisions
          * Keep the movement and move the gripper body too
          */
         m_ptActualGripperBody->p = cpv(c_position.GetX(), c_position.GetY());
         cpBodySetAngle(m_ptActualGripperBody,
                        cZAngle.GetValue() + m_cFootBotEntity.GetTurretEntity().GetRotation().GetValue());
         /* Release grippers and gripees */
         m_pcGripper->Release();
         m_pcGrippable->ReleaseAll();
         /* Update the active space hash */
         cpSpaceReindexShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseShape);
         /* Update anchors */
         CalculateAnchors();
         /* Update bounding box */
         CalculateBoundingBox();
      }
      /* The movement is allowed if there is no collision */
      return !nCollision;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotModel::Reset() {
      /* Reset body position */
      const CVector3& cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
      m_ptActualBaseBody->p = cpv(cPosition.GetX(), cPosition.GetY());
      m_ptActualGripperBody->p = cpv(cPosition.GetX(), cPosition.GetY());
      /* Reset body orientation */
      CRadians cXAngle, cYAngle, cZAngle;
      GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpBodySetAngle(m_ptActualBaseBody, cZAngle.GetValue());
      cpBodySetAngle(m_ptActualGripperBody, cZAngle.GetValue());
      /* Zero speed and applied forces of actual base body */
      m_ptActualBaseBody->v = cpvzero;
      m_ptActualBaseBody->w = 0.0f;
      cpBodyResetForces(m_ptActualBaseBody);
      /* Zero speed and applied forces of base control body */
      m_cDiffSteering.Reset();
      /* Release grippers and gripees */
      m_pcGripper->Release();
      m_pcGrippable->ReleaseAll();
      /* Zero speed and applied forces of actual gripper body */
      m_ptActualGripperBody->v = cpvzero;
      m_ptActualGripperBody->w = 0.0f;
      cpBodyResetForces(m_ptActualGripperBody);
      /* Switch to turret passive mode if needed */
      if(m_unLastTurretMode == MODE_SPEED_CONTROL ||
         m_unLastTurretMode == MODE_POSITION_CONTROL) {
         TurretActiveToPassive();
         m_unLastTurretMode = MODE_OFF;
      }
      /* Update bounding box */
      cpShapeCacheBB(m_ptBaseShape);
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotModel::CalculateBoundingBox() {
      GetBoundingBox().MinCorner.SetX(m_ptBaseShape->bb.l);
      GetBoundingBox().MinCorner.SetY(m_ptBaseShape->bb.b);
      GetBoundingBox().MaxCorner.SetX(m_ptBaseShape->bb.r);
      GetBoundingBox().MaxCorner.SetY(m_ptBaseShape->bb.t);
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
               m_cFootBotEntity.GetTurretEntity().GetRotation(),
               NormalizedDifference(
                  CRadians(m_ptActualGripperBody->a),
                  CRadians(m_ptActualBaseBody->a))).GetValue();
            m_ptControlGripperBody->w =
               m_cDiffSteering.GetAngularVelocity() +
               (PD_P_CONSTANT * fCurRotErr +
                PD_D_CONSTANT * (fCurRotErr - m_fPreviousTurretAngleError) * m_cDyn2DEngine.GetInverseSimulationClockTick());
            m_fPreviousTurretAngleError = fCurRotErr;
            break;
         }
         case MODE_SPEED_CONTROL:
            m_ptControlGripperBody->w =
               m_cDiffSteering.GetAngularVelocity() +
               m_cFootBotEntity.GetTurretEntity().GetRotationSpeed();
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
      cpSpaceRemoveConstraint(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseGripperAngularMotion);
      cpConstraintFree(m_ptBaseGripperAngularMotion);
      /* Create gripper control body */
      m_ptControlGripperBody = cpBodyNew(INFINITY, INFINITY);
      /* Create angular constraint from gripper control body to gripper actual body */
      m_ptGripperControlAngularMotion = cpSpaceAddConstraint(m_cDyn2DEngine.GetPhysicsSpace(),
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
      cpSpaceRemoveConstraint(m_cDyn2DEngine.GetPhysicsSpace(), m_ptGripperControlAngularMotion);
      cpConstraintFree(m_ptGripperControlAngularMotion);
      /* Delete control body */
      cpBodyFree(m_ptControlGripperBody);
      /* Create constraints from actual gripper body to actual base body */
      m_ptBaseGripperAngularMotion = cpSpaceAddConstraint(m_cDyn2DEngine.GetPhysicsSpace(),
                                                          cpGearJointNew(m_ptActualBaseBody,
                                                                         m_ptActualGripperBody,
                                                                         0.0f,
                                                                         1.0f));
      m_ptBaseGripperAngularMotion->maxBias = 0.0f; /* disable joint correction */
      m_ptBaseGripperAngularMotion->maxForce = FOOTBOT_MAX_TORQUE; /* limit the dragging torque */
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DFootBotModel::IsCollidingWithSomething() const {
      return cpSpaceShapeQuery(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseShape, NULL, NULL) > 0;
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
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS2D_OPERATIONS_ON_ENTITY(CFootBotEntity, CDynamics2DFootBotModel);

   /****************************************/
   /****************************************/

}
