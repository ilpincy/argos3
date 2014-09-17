/**
 * @file <argos3/plugins/robots/e-puck/simulator/dynamics2d_epuck_model.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "dynamics2d_epuck_model.h"
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_gripping.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real EPUCK_RADIUS                   = 0.035f;
   static const Real EPUCK_INTERWHEEL_DISTANCE      = 0.053f;
   static const Real EPUCK_HEIGHT                   = 0.086f;

   static const Real EPUCK_MAX_FORCE                = 1.5f;
   static const Real EPUCK_MAX_TORQUE               = 1.5f;

   enum EPUCK_WHEELS {
      EPUCK_LEFT_WHEEL = 0,
      EPUCK_RIGHT_WHEEL = 1
   };

   /****************************************/
   /****************************************/

   CDynamics2DEPuckModel::CDynamics2DEPuckModel(CDynamics2DEngine& c_engine,
                                                CEPuckEntity& c_entity) :
      CDynamics2DModel(c_engine, c_entity.GetEmbodiedEntity()),
      m_cEPuckEntity(c_entity),
      m_cWheeledEntity(m_cEPuckEntity.GetWheeledEntity()),
      m_cDiffSteering(c_engine,
                      EPUCK_MAX_FORCE,
                      EPUCK_MAX_TORQUE,
                      EPUCK_INTERWHEEL_DISTANCE),
      m_fMass(0.4f),
      m_fCurrentWheelVelocity(m_cWheeledEntity.GetWheelVelocities()) {
      /* Get origin anchor and register its update method */
      const SAnchor& sOrigin = GetEmbodiedEntity().GetOriginAnchor();
      RegisterAnchorMethod<CDynamics2DEPuckModel>(sOrigin,
                                                  &CDynamics2DEPuckModel::UpdateOriginAnchor);
      /* Create the actual body with initial position and orientation */
      m_ptActualBaseBody =
         cpSpaceAddBody(m_cDyn2DEngine.GetPhysicsSpace(),
                        cpBodyNew(m_fMass,
                                  cpMomentForCircle(m_fMass,
                                                    0.0f,
                                                    EPUCK_RADIUS + EPUCK_RADIUS,
                                                    cpvzero)));
      const CVector3& cPosition = sOrigin.Position;
      m_ptActualBaseBody->p = cpv(cPosition.GetX(), cPosition.GetY());
      CRadians cXAngle, cYAngle, cZAngle;
      sOrigin.Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpBodySetAngle(m_ptActualBaseBody, cZAngle.GetValue());
      /* Associate this model to the body data for ray queries */
      m_ptActualBaseBody->data = this;
      /* Create the actual body shape */
      m_ptBaseShape =
         cpSpaceAddShape(m_cDyn2DEngine.GetPhysicsSpace(),
                         cpCircleShapeNew(m_ptActualBaseBody,
                                          EPUCK_RADIUS,
                                          cpvzero));
      m_ptBaseShape->e = 0.0; // No elasticity
      m_ptBaseShape->u = 0.7; // Lots of friction
      /* Constrain the actual base body to follow the diff steering control */
      m_cDiffSteering.AttachTo(m_ptActualBaseBody);
      /* Calculate bounding box */
      GetBoundingBox().MinCorner.SetZ(sOrigin.Position.GetZ());
      GetBoundingBox().MaxCorner.SetZ(sOrigin.Position.GetZ() + EPUCK_HEIGHT);
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   CDynamics2DEPuckModel::~CDynamics2DEPuckModel() {
      m_cDiffSteering.Detach();
      cpSpaceRemoveBody(m_cDyn2DEngine.GetPhysicsSpace(), m_ptActualBaseBody);
      cpSpaceRemoveShape(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseShape);
      cpShapeFree(m_ptBaseShape);
      cpBodyFree(m_ptActualBaseBody);
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DEPuckModel::MoveTo(const CVector3& c_position,
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
                                              EPUCK_RADIUS,
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
          */
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

   void CDynamics2DEPuckModel::Reset() {
      /* Reset body position */
      const CVector3& cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
      m_ptActualBaseBody->p = cpv(cPosition.GetX(), cPosition.GetY());
      /* Reset body orientation */
      CRadians cXAngle, cYAngle, cZAngle;
      GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpBodySetAngle(m_ptActualBaseBody, cZAngle.GetValue());
      /* Zero speed and applied forces of actual base body */
      m_ptActualBaseBody->v = cpvzero;
      m_ptActualBaseBody->w = 0.0f;
      cpBodyResetForces(m_ptActualBaseBody);
      /* Zero speed and applied forces of base control body */
      m_cDiffSteering.Reset();
      /* Update bounding box */
      cpShapeCacheBB(m_ptBaseShape);
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEPuckModel::CalculateBoundingBox() {
      GetBoundingBox().MinCorner.SetX(m_ptBaseShape->bb.l);
      GetBoundingBox().MinCorner.SetY(m_ptBaseShape->bb.b);
      GetBoundingBox().MaxCorner.SetX(m_ptBaseShape->bb.r);
      GetBoundingBox().MaxCorner.SetY(m_ptBaseShape->bb.t);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEPuckModel::UpdateFromEntityStatus() {
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

   bool CDynamics2DEPuckModel::IsCollidingWithSomething() const {
      return cpSpaceShapeQuery(m_cDyn2DEngine.GetPhysicsSpace(), m_ptBaseShape, NULL, NULL) > 0;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEPuckModel::UpdateOriginAnchor(SAnchor& s_anchor) {
      s_anchor.Position.SetX(m_ptActualBaseBody->p.x);
      s_anchor.Position.SetY(m_ptActualBaseBody->p.y);
      s_anchor.Orientation.FromAngleAxis(CRadians(m_ptActualBaseBody->a), CVector3::Z);
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS2D_OPERATIONS_ON_ENTITY(CEPuckEntity, CDynamics2DEPuckModel);

   /****************************************/
   /****************************************/

}
