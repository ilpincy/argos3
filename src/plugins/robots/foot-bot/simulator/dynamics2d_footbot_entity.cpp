/**
 * @file <argos3/plugins/robots/foot-bot/simulator/dynamics2d_footbot_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "dynamics2d_footbot_entity.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/gripper_equipped_entity.h>
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

   static const Real FOOTBOT_MAX_FORCE                = 1.5f;
   static const Real FOOTBOT_MAX_TORQUE               = 1.5f;

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

   CDynamics2DFootBotEntity::CDynamics2DFootBotEntity(CDynamics2DEngine& c_engine,
                                                      CFootBotEntity& c_entity) :
      CDynamics2DEntity(c_engine, c_entity.GetEmbodiedEntity()),
      m_cFootBotEntity(c_entity),
      m_cWheeledEntity(m_cFootBotEntity.GetWheeledEntity()),
      m_cGripperEntity(c_entity.GetGripperEquippedEntity()),
      m_fMass(1.6f),
      m_unLastTurretMode(m_cFootBotEntity.GetTurretMode()) {
      /* Create the actual body with initial position and orientation */
      m_ptActualBaseBody =
         cpSpaceAddBody(m_cDyn2DEngine.GetPhysicsSpace(),
                        cpBodyNew(m_fMass,
                                  cpMomentForCircle(m_fMass,
                                                    0.0f,
                                                    FOOTBOT_RADIUS + FOOTBOT_RADIUS,
                                                    cpvzero)));
      const CVector3& cPosition = GetEmbodiedEntity().GetPosition();
      m_ptActualBaseBody->p = cpv(cPosition.GetX(), cPosition.GetY());
      CRadians cXAngle, cYAngle, cZAngle;
      GetEmbodiedEntity().GetOrientation().ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpBodySetAngle(m_ptActualBaseBody, cZAngle.GetValue());
      /* Create the control body with initial position and orientation */
      m_ptControlBaseBody = cpBodyNew(INFINITY, INFINITY);
      /* Create the actual body geometry */
      m_ptBaseShape =
         cpSpaceAddShape(m_cDyn2DEngine.GetPhysicsSpace(),
                         cpCircleShapeNew(m_ptActualBaseBody,
                                          FOOTBOT_RADIUS,
                                          cpvzero));
      /* This object is grippable */
      m_ptBaseShape->collision_type = CDynamics2DEngine::SHAPE_GRIPPABLE;
      m_ptBaseShape->data = reinterpret_cast<void*>(&c_entity.GetEmbodiedEntity());
      /* No elasticity */
      m_ptBaseShape->e = 0.0;
      /* Lots of friction */
      m_ptBaseShape->u = 0.7;
      /* Constrain the actual base body to follow the control base body */
      m_ptBaseControlLinearMotion =
         cpSpaceAddConstraint(m_cDyn2DEngine.GetPhysicsSpace(),
                              cpPivotJointNew2(m_ptActualBaseBody,
                                               m_ptControlBaseBody,
                                               cpvzero,
                                               cpvzero));
      m_ptBaseControlLinearMotion->maxBias = 0.0f; /* disable joint correction */
      m_ptBaseControlLinearMotion->maxForce = FOOTBOT_MAX_FORCE; /* limit the dragging force */
      m_ptBaseControlAngularMotion = cpSpaceAddConstraint(m_cDyn2DEngine.GetPhysicsSpace(),
                                               cpGearJointNew(m_ptActualBaseBody,
                                                              m_ptControlBaseBody,
                                                              0.0f,
                                                              1.0f));
      m_ptBaseControlAngularMotion->maxBias = 0.0f; /* disable joint correction */
      m_ptBaseControlAngularMotion->maxForce = FOOTBOT_MAX_TORQUE; /* limit the dragging torque */
      /* Zero the wheel velocity */
      m_fCurrentWheelVelocityFromSensor[FOOTBOT_LEFT_WHEEL] = 0.0f;
      m_fCurrentWheelVelocityFromSensor[FOOTBOT_RIGHT_WHEEL] = 0.0f;
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
                     m_cFootBotEntity.GetTurretRotation().GetValue());
      /* Create the gripper shape */
      m_ptGripperShape = 
         cpSpaceAddShape(m_cDyn2DEngine.GetPhysicsSpace(),
                         cpCircleShapeNew(m_ptActualGripperBody,
                                          0.02f,
                                          cpv(FOOTBOT_RADIUS, 0.0f)));
      m_ptGripperShape->sensor = 1;
      /* This object is a gripper */
      m_ptGripperShape->collision_type = CDynamics2DEngine::SHAPE_MAGNETIC_GRIPPER;
      /* Set the data associated to this gripper */
      m_psGripperData = new SDynamics2DEngineGripperData(m_cDyn2DEngine.GetPhysicsSpace(),
                                                         m_cGripperEntity,
                                                         cpv(FOOTBOT_RADIUS, 0.0f));
      m_ptGripperShape->data = reinterpret_cast<void*>(m_psGripperData);
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
      /* Switch to active mode if necessary */
      if(m_unLastTurretMode == MODE_SPEED_CONTROL ||
         m_unLastTurretMode == MODE_POSITION_CONTROL) {
         TurretActiveToPassive();
      }
      /* Precalculate Z-axis range of the bounding box */
      GetBoundingBox().MinCorner.SetZ(GetEmbodiedEntity().GetPosition().GetZ());
      GetBoundingBox().MaxCorner.SetZ(GetEmbodiedEntity().GetPosition().GetZ() + FOOTBOT_HEIGHT);
   }

   /****************************************/
   /****************************************/

   CDynamics2DFootBotEntity::~CDynamics2DFootBotEntity() {
      switch(m_unLastTurretMode) {
         case MODE_OFF:
         case MODE_PASSIVE:
            delete m_psGripperData;
            cpSpaceRemoveConstraint(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseGripperLinearMotion);
            cpSpaceRemoveConstraint(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseGripperAngularMotion);
            cpSpaceRemoveBody(m_cDyn2DEngine.GetPhysicsSpace(), m_ptActualGripperBody);
            cpSpaceRemoveShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptGripperShape);
            cpConstraintFree(m_ptBaseGripperLinearMotion);
            cpConstraintFree(m_ptBaseGripperAngularMotion);
            cpBodyFree(m_ptActualGripperBody);
            cpShapeFree(m_ptGripperShape);
            break;
         case MODE_POSITION_CONTROL:
         case MODE_SPEED_CONTROL:
            delete m_psGripperData;
            cpSpaceRemoveConstraint(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseGripperLinearMotion);
            cpSpaceRemoveConstraint(m_cDyn2DEngine.GetPhysicsSpace(), m_ptGripperControlAngularMotion);
            cpSpaceRemoveBody(m_cDyn2DEngine.GetPhysicsSpace(), m_ptActualGripperBody);
            cpSpaceRemoveShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptGripperShape);
            cpConstraintFree(m_ptBaseGripperLinearMotion);
            cpConstraintFree(m_ptGripperControlAngularMotion);
            cpBodyFree(m_ptActualGripperBody);
            cpBodyFree(m_ptControlGripperBody);
            cpShapeFree(m_ptGripperShape);
            break;
      }
      cpSpaceRemoveConstraint(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseControlLinearMotion);
      cpSpaceRemoveConstraint(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseControlAngularMotion);
      cpSpaceRemoveBody(m_cDyn2DEngine.GetPhysicsSpace(), m_ptActualBaseBody);
      cpSpaceRemoveShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseShape);
      cpConstraintFree(m_ptBaseControlLinearMotion);
      cpConstraintFree(m_ptBaseControlAngularMotion);
      cpBodyFree(m_ptActualBaseBody);
      cpBodyFree(m_ptControlBaseBody);
      cpShapeFree(m_ptBaseShape);
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DFootBotEntity::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                           const CRay3& c_ray) const {
      cpSegmentQueryInfo tInfo;
      if(cpShapeSegmentQuery(m_ptBaseShape,
                             cpv(c_ray.GetStart().GetX(), c_ray.GetStart().GetY()),
                             cpv(c_ray.GetEnd().GetX()  , c_ray.GetEnd().GetY()  ),
                             &tInfo)) {
      	 CVector3 cIntersectionPoint;
      	 c_ray.GetPoint(cIntersectionPoint, tInfo.t);
      	 if((cIntersectionPoint.GetZ() >= GetEmbodiedEntity().GetPosition().GetZ()                 ) &&
      			(cIntersectionPoint.GetZ() <= GetEmbodiedEntity().GetPosition().GetZ() + FOOTBOT_HEIGHT) ) {
            f_t_on_ray = tInfo.t;
            return true;
      	 }
      	 else {
            return false;
      	 }
      }
      else {
         return false;
      }
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DFootBotEntity::MoveTo(const CVector3& c_position,
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
      if(b_check_only || nCollision) {
         /* Restore old body state if there was a collision or
            it was only a check for movement */
         m_ptActualBaseBody->p = tOldPos;
         cpBodySetAngle(m_ptActualBaseBody, fOldA);
      }
      else {
         /* Zero speed and applied forces of base control body */
         m_ptControlBaseBody->v = cpvzero;
         m_ptControlBaseBody->w = 0.0f;
         cpBodyResetForces(m_ptControlBaseBody);
         /* Update the active space hash if the movement is actual */
         cpSpaceReindexShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseShape);
      }
      /* The movement is allowed if there is no collision */
      return !nCollision;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotEntity::Reset() {
      /* Reset body position */
      const CVector3& cPosition = GetEmbodiedEntity().GetInitPosition();
      m_ptActualBaseBody->p = cpv(cPosition.GetX(), cPosition.GetY());
      /* Reset body orientation */
      CRadians cXAngle, cYAngle, cZAngle;
      GetEmbodiedEntity().GetInitOrientation().ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpBodySetAngle(m_ptActualBaseBody, cZAngle.GetValue());
      /* Zero speed and applied forces of actual base body */
      m_ptActualBaseBody->v = cpvzero;
      m_ptActualBaseBody->w = 0.0f;
      cpBodyResetForces(m_ptActualBaseBody);
      /* Zero speed and applied forces of base control body */
      m_ptControlBaseBody->v = cpvzero;
      m_ptControlBaseBody->w = 0.0f;
      cpBodyResetForces(m_ptControlBaseBody);
      /* Release gripped objects */
      m_psGripperData->ClearConstraints();
      /* Zero speed and applied forces of actual gripper body */
      cpBodySetAngle(m_ptActualGripperBody, cZAngle.GetValue());
      m_ptActualGripperBody->v = cpvzero;
      m_ptActualGripperBody->w = 0.0f;
      cpBodyResetForces(m_ptActualGripperBody);
      /* Switch to turret passive mode if needed */
      if(m_unLastTurretMode == MODE_SPEED_CONTROL ||
         m_unLastTurretMode == MODE_POSITION_CONTROL) {
         TurretActiveToPassive();
         m_unLastTurretMode = MODE_OFF;
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotEntity::CalculateBoundingBox() {
      GetBoundingBox().MinCorner.SetX(m_ptBaseShape->bb.l);
      GetBoundingBox().MinCorner.SetY(m_ptBaseShape->bb.b);
      GetBoundingBox().MaxCorner.SetX(m_ptBaseShape->bb.r);
      GetBoundingBox().MaxCorner.SetY(m_ptBaseShape->bb.t);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotEntity::UpdateEntityStatus() {
      /* Update bounding box */
      CalculateBoundingBox();
      /* Update foot-bot body position */
      m_cDyn2DEngine.PositionPhysicsToSpace(m_cSpacePosition, GetEmbodiedEntity().GetPosition(), m_ptActualBaseBody);
      GetEmbodiedEntity().SetPosition(m_cSpacePosition);
      /* Update foot-bot body orientation */
      m_cDyn2DEngine.OrientationPhysicsToSpace(m_cSpaceOrientation, m_ptActualBaseBody);
      GetEmbodiedEntity().SetOrientation(m_cSpaceOrientation);
      /* Update foot-bot turret rotation */
      m_cFootBotEntity.SetTurretRotation(CRadians(m_ptActualGripperBody->a - m_ptActualBaseBody->a));
      /* Update foot-bot components */
      m_cFootBotEntity.UpdateComponents();
      /* Check whether a transfer is necessary */
      if(m_cDyn2DEngine.IsEntityTransferActive()) {
         std::string strEngineId;
         if(m_cDyn2DEngine.CalculateTransfer(GetEmbodiedEntity().GetPosition().GetX(),
                                        GetEmbodiedEntity().GetPosition().GetY(),
                                        strEngineId)) {
            m_cDyn2DEngine.ScheduleEntityForTransfer(m_cFootBotEntity, strEngineId);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DFootBotEntity::UpdateFromEntityStatus() {
      /* Get wheel speeds from entity */
      m_cWheeledEntity.GetSpeed(m_fCurrentWheelVelocityFromSensor);
      /* Do we want to move? */
      if((m_fCurrentWheelVelocityFromSensor[FOOTBOT_LEFT_WHEEL] != 0.0f) ||
         (m_fCurrentWheelVelocityFromSensor[FOOTBOT_RIGHT_WHEEL] != 0.0f)) {
         /* Yeah, we do */
         /*
          * THE DIFFERENTIAL STEERING SYSTEM
          *
          * check http://rossum.sourceforge.net/papers/DiffSteer/
          * for details
          *
          * Equations:
          *
          * w = (Vr - Vl) / b
          * v = [ ((Vr + Vl) / 2) cos(a),
          *       ((Vr + Vl) / 2) sin(a) ]
          *
          * where:
          *        a  = body orientation
          *        w  = body angular velocity
          *        v  = body center linear velocity
          *        Vr = right wheel velocity
          *        Vl = left wheel velocity
          *        b  = length of wheel axis
          */
         m_ptControlBaseBody->w =
            (m_fCurrentWheelVelocityFromSensor[FOOTBOT_RIGHT_WHEEL] -
             m_fCurrentWheelVelocityFromSensor[FOOTBOT_LEFT_WHEEL]) / 
            FOOTBOT_INTERWHEEL_DISTANCE;
         m_ptControlBaseBody->v =
            cpvrotate(m_ptActualBaseBody->rot,
                      cpv((m_fCurrentWheelVelocityFromSensor[FOOTBOT_LEFT_WHEEL] +
                           m_fCurrentWheelVelocityFromSensor[FOOTBOT_RIGHT_WHEEL]) *
                          0.5f,
                          0.0f));
      }
      else {
         /* No, we don't want to move - zero all speeds */
         m_ptControlBaseBody->w = 0.0f;
         m_ptControlBaseBody->v = cpvzero;
      }
      /* Is the gripper unlocked? */
      if(m_psGripperData->GripperEntity.IsUnlocked()) {
         /* The gripper is locked. If it was gripping an object,
          * release it. Then, process the collision normally */
      	 if(m_psGripperData->GripperEntity.IsGripping()) {
            m_psGripperData->ClearConstraints();
      	 }
      }
      /* Update turret structures if the state changed state in the last step */
      if(m_cFootBotEntity.GetTurretMode() != m_unLastTurretMode) {
         /* Manage the thing like a state machine */
         switch(m_unLastTurretMode) {
            case MODE_OFF:
            case MODE_PASSIVE:
               switch(m_cFootBotEntity.GetTurretMode()) {
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
               switch(m_cFootBotEntity.GetTurretMode()) {
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
         m_unLastTurretMode = m_cFootBotEntity.GetTurretMode();
      }
      /* Update the turret data */
      switch(m_unLastTurretMode) {

         /* Position control mode is implemented using a PD controller */
         case MODE_POSITION_CONTROL:
            m_ptControlGripperBody->w =
               m_ptControlBaseBody->w +
               (PD_P_CONSTANT * (m_cFootBotEntity.GetTurretRotation().GetValue() - (m_ptActualGripperBody->a - m_ptActualBaseBody->a))
                + PD_D_CONSTANT * (m_cFootBotEntity.GetTurretRotation().GetValue() - (m_ptActualGripperBody->a - m_ptActualBaseBody->a) - m_fPreviousTurretAngleError) )*
               m_cDyn2DEngine.GetInverseSimulationClockTick();
            m_fPreviousTurretAngleError = m_cFootBotEntity.GetTurretRotation().GetValue() - (m_ptActualGripperBody->a - m_ptActualBaseBody->a);
            break;
         case MODE_SPEED_CONTROL:
            m_ptControlGripperBody->w =
               m_ptControlBaseBody->w +
               m_cFootBotEntity.GetTurretRotationSpeed();
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

   void CDynamics2DFootBotEntity::TurretPassiveToActive() {
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

   void CDynamics2DFootBotEntity::TurretActiveToPassive() {
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

   REGISTER_STANDARD_DYNAMICS2D_OPERATIONS_ON_ENTITY(CFootBotEntity, CDynamics2DFootBotEntity);

   /****************************************/
   /****************************************/

}

