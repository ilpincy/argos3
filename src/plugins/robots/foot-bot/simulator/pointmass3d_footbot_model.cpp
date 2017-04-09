/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_differentialsteering_control.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "pointmass3d_footbot_model.h"
#include <argos3/core/utility/math/cylinder.h>

namespace argos {

   static const Real FOOTBOT_RADIUS                   = 0.085036758f;
   static const Real FOOTBOT_INTERWHEEL_DISTANCE      = 0.14f;
   static const Real FOOTBOT_HEIGHT                   = 0.146899733f;

   enum FOOTBOT_WHEELS {
      FOOTBOT_LEFT_WHEEL = 0,
      FOOTBOT_RIGHT_WHEEL = 1
   };

   /****************************************/
   /****************************************/

   CPointMass3DFootBotModel::CPointMass3DFootBotModel(CPointMass3DEngine& c_engine,
                                                      CFootBotEntity& c_footbot) :
      CPointMass3DModel(c_engine, c_footbot.GetEmbodiedEntity()),
      m_cWheeledEntity(c_footbot.GetWheeledEntity()),
      m_fCurrentWheelVelocity(m_cWheeledEntity.GetWheelVelocities()) {
      /* Register the origin anchor update method */
      RegisterAnchorMethod(GetEmbodiedEntity().GetOriginAnchor(),
                           &CPointMass3DFootBotModel::UpdateOriginAnchor);
      /* Get initial rotation */
      CRadians cTmp1, cTmp2;
      GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(m_cYaw, cTmp1, cTmp2);
   }

   /****************************************/
   /****************************************/

   void CPointMass3DFootBotModel::Reset() {
      CPointMass3DModel::Reset();
      CRadians cTmp1, cTmp2;
      GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(m_cYaw, cTmp1, cTmp2);
      m_fAngularVelocity = 0.0;
   }

   /****************************************/
   /****************************************/


   void CPointMass3DFootBotModel::UpdateFromEntityStatus() {
      m_cVelocity.Set((m_fCurrentWheelVelocity[FOOTBOT_RIGHT_WHEEL] + m_fCurrentWheelVelocity[FOOTBOT_LEFT_WHEEL])*0.5, 0.0, 0.0);
      m_cVelocity.RotateZ(m_cYaw);
      m_fAngularVelocity = (m_fCurrentWheelVelocity[FOOTBOT_RIGHT_WHEEL] - m_fCurrentWheelVelocity[FOOTBOT_LEFT_WHEEL]) / FOOTBOT_INTERWHEEL_DISTANCE;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DFootBotModel::Step() {
      m_cPosition += m_cVelocity * m_cPM3DEngine.GetPhysicsClockTick();
      m_cYaw += CRadians(m_fAngularVelocity * m_cPM3DEngine.GetPhysicsClockTick());
   }

   /****************************************/
   /****************************************/

   void CPointMass3DFootBotModel::CalculateBoundingBox() {
      GetBoundingBox().MinCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() - FOOTBOT_RADIUS,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() - FOOTBOT_RADIUS,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ());
      GetBoundingBox().MaxCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() + FOOTBOT_RADIUS,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() + FOOTBOT_RADIUS,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ() + FOOTBOT_HEIGHT);
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DFootBotModel::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                           const CRay3& c_ray) const {
      CCylinder m_cShape(FOOTBOT_RADIUS,
                         FOOTBOT_HEIGHT,
                         m_cPosition,
                         CVector3::Z);
      bool bIntersects = m_cShape.Intersects(f_t_on_ray, c_ray);
      return bIntersects;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DFootBotModel::UpdateOriginAnchor(SAnchor& s_anchor) {
      s_anchor.Position = m_cPosition;
      s_anchor.Orientation = CQuaternion(m_cYaw, CVector3::Z);
   }

   /****************************************/
   /****************************************/
   
   REGISTER_STANDARD_POINTMASS3D_OPERATIONS_ON_ENTITY(CFootBotEntity, CPointMass3DFootBotModel);

   /****************************************/
   /****************************************/

}
