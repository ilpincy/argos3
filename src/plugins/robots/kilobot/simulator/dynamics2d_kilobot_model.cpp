/**
 * @file <argos3/plugins/robots/kilobot/simulator/dynamics2d_kilobot_model.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Vito Trianni - <vito.trianni@istc.cnr.it>
 */

#include "dynamics2d_kilobot_model.h"
#include "kilobot_measures.h"
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real KILOBOT_MAX_FORCE                = 0.001f;
   static const Real KILOBOT_MAX_TORQUE               = 0.001f;

   enum KILOBOT_WHEELS {
      KILOBOT_LEFT_WHEEL = 0,
      KILOBOT_RIGHT_WHEEL = 1
   };

   /****************************************/
   /****************************************/

   CDynamics2DKilobotModel::CDynamics2DKilobotModel(CDynamics2DEngine& c_engine,
                                                    CKilobotEntity& c_entity) :
      CDynamics2DSingleBodyObjectModel(c_engine, c_entity),
      m_cKilobotEntity(c_entity),
      m_cWheeledEntity(m_cKilobotEntity.GetWheeledEntity()),
      m_cDiffSteering(c_engine,
                      KILOBOT_MAX_FORCE,
                      KILOBOT_MAX_TORQUE,
                      KILOBOT_INTERPIN_DISTANCE),
      m_fCurrentWheelVelocity(m_cWheeledEntity.GetWheelVelocities()) {
      /* Create the actual body with initial position and orientation */
      cpBody* ptBody =
         cpSpaceAddBody(GetDynamics2DEngine().GetPhysicsSpace(),
                        cpBodyNew(KILOBOT_MASS,
                                  cpMomentForCircle(KILOBOT_MASS,
                                                    0.0f,
                                                    KILOBOT_RADIUS + KILOBOT_RADIUS,
                                                    cpv(KILOBOT_ECCENTRICITY,0))));
      const CVector3& cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
      ptBody->p = cpv(cPosition.GetX(), cPosition.GetY());
      CRadians cXAngle, cYAngle, cZAngle;
      GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpBodySetAngle(ptBody, cZAngle.GetValue());
      /* Create the actual body shape */
      cpShape* ptShape =
         cpSpaceAddShape(GetDynamics2DEngine().GetPhysicsSpace(),
                         cpCircleShapeNew(ptBody,
                                          KILOBOT_RADIUS,
                                          cpv(KILOBOT_ECCENTRICITY,0)));
      ptShape->e = 0.0; // No elasticity
      ptShape->u = 0.7; // Lots of friction
      /* Constrain the body to follow the diff steering control */
      m_cDiffSteering.AttachTo(ptBody);
      /* Set the body so that the default methods work as expected */
      SetBody(ptBody, KILOBOT_HEIGHT);
      /* Set the anchor updaters */
      RegisterAnchorMethod<CDynamics2DKilobotModel>(
         GetEmbodiedEntity().GetAnchor("light"),
         &CDynamics2DKilobotModel::UpdateLightAnchor);
      RegisterAnchorMethod<CDynamics2DKilobotModel>(
         GetEmbodiedEntity().GetAnchor("comm"),
         &CDynamics2DKilobotModel::UpdateCommAnchor);
   }

   /****************************************/
   /****************************************/

   CDynamics2DKilobotModel::~CDynamics2DKilobotModel() {
      m_cDiffSteering.Detach();
   }

  
   /****************************************/
   /****************************************/

   void CDynamics2DKilobotModel::Reset() {
      CDynamics2DSingleBodyObjectModel::Reset();
      m_cDiffSteering.Reset();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DKilobotModel::UpdateFromEntityStatus() {
      /* Do we want to move? */
      if((m_fCurrentWheelVelocity[KILOBOT_LEFT_WHEEL] != 0.0f) ||
         (m_fCurrentWheelVelocity[KILOBOT_RIGHT_WHEEL] != 0.0f)) {
         m_cDiffSteering.SetWheelVelocity(m_fCurrentWheelVelocity[KILOBOT_LEFT_WHEEL],
                                          m_fCurrentWheelVelocity[KILOBOT_RIGHT_WHEEL]);
      }
      else {
         /* No, we don't want to move - zero all speeds */
         m_cDiffSteering.Reset();
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DKilobotModel::UpdateLightAnchor(SAnchor& s_anchor) {
      /* Start in origin, put anchor in offset */
      s_anchor.Position = s_anchor.OffsetPosition;
      /* Rotate anchor by body orientation in world */
      s_anchor.Orientation.FromAngleAxis(CRadians(GetBody()->a), CVector3::Z);
      s_anchor.Position.Rotate(s_anchor.Orientation);
      /* Translate anchor by body position in world */
      s_anchor.Position.SetX(s_anchor.Position.GetX() + GetBody()->p.x);
      s_anchor.Position.SetY(s_anchor.Position.GetY() + GetBody()->p.y);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DKilobotModel::UpdateCommAnchor(SAnchor& s_anchor) {
      /* Start in origin, put anchor in offset */
      s_anchor.Position = s_anchor.OffsetPosition;
      /* Rotate anchor by body orientation in world */
      s_anchor.Orientation.FromAngleAxis(CRadians(GetBody()->a), CVector3::Z);
      s_anchor.Position.Rotate(s_anchor.Orientation);
      /* Translate anchor by body position in world */
      s_anchor.Position.SetX(s_anchor.Position.GetX() + GetBody()->p.x);
      s_anchor.Position.SetY(s_anchor.Position.GetY() + GetBody()->p.y);
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS2D_OPERATIONS_ON_ENTITY(CKilobotEntity, CDynamics2DKilobotModel);

   /****************************************/
   /****************************************/

}
