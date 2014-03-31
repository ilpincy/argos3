/**
 * @file <argos3/plugins/robots/foot-bot/simulator/pointmass3d_eyebot_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "pointmass3d_eyebot_model.h"
#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_engine.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   static const Real     BODY_HEIGHT  = 0.566f;
   static const Real     BODY_RADIUS  = 0.25f;
   static const Real     BODY_MASS    = 1.00f;
   static const Real     BODY_INERTIA = 0.01f;
   static const CVector3 POS_K_P(20.0f, 20.0f, 20.0f);
   static const CVector3 POS_K_D(10.0f, 10.0f, 10.0f);
   static const Real     ORIENT_K_P = 0.5f;
   static const Real     ORIENT_K_D = 0.1f;
   static const Real     MAX_FORCE_X = 1000.0f;
   static const Real     MAX_FORCE_Y = 1000.0f;
   static const Real     MAX_FORCE_Z = 1000.0f;
   static const Real     MAX_TORQUE = 1.0f;

   /****************************************/
   /****************************************/

   static Real Clamp(Real f_max,
                     Real f_value) {
      if(f_value >  f_max) return  f_max;
      if(f_value < -f_max) return -f_max;
      return f_value;
   }

   /****************************************/
   /****************************************/

   CPointMass3DEyeBotModel::CPointMass3DEyeBotModel(CPointMass3DEngine& c_engine,
                                                    CEyeBotEntity& c_eyebot) :
      CPointMass3DModel(c_engine, c_eyebot.GetEmbodiedEntity()),
      m_cEyeBotEntity(c_eyebot),
      m_cQuadRotorEntity(c_eyebot.GetQuadRotorEntity()) {
      Reset();
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DEyeBotModel::MoveTo(const CVector3& c_position,
                                        const CQuaternion& c_orientation,
                                        bool b_check_only) {
      if(! b_check_only) {
         m_cEyeBotEntity.GetEmbodiedEntity().SetPosition(c_position);
         m_cEyeBotEntity.GetEmbodiedEntity().SetOrientation(c_orientation);
         CalculateBoundingBox();
      }
      return true;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEyeBotModel::Reset() {
      CalculateBoundingBox();
      m_pfPosError[0] = 0.0f;
      m_pfPosError[1] = 0.0f;
      m_pfPosError[2] = 0.0f;
      m_fOrientError = 0.0f;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEyeBotModel::UpdateFromEntityStatus() {
       m_sDesiredPositionData = m_cQuadRotorEntity.GetPositionControlData();
    }

   /****************************************/
   /****************************************/

   void CPointMass3DEyeBotModel::Step() {
       /*
       * Update positional information
       */
      /* Integration step */
      m_cEmbodiedEntity.SetPosition(
         CVector3(
            m_cEmbodiedEntity.GetPosition().GetX() + m_cLinearVelocity.GetX() * m_cPM3DEngine.GetPhysicsClockTick(),
            m_cEmbodiedEntity.GetPosition().GetY() + m_cLinearVelocity.GetY() * m_cPM3DEngine.GetPhysicsClockTick(),
            m_cEmbodiedEntity.GetPosition().GetZ() + m_cLinearVelocity.GetZ() * m_cPM3DEngine.GetPhysicsClockTick()));
      m_cEmbodiedEntity.SetOrientation(
         m_cEmbodiedEntity.GetOrientation() *
         CQuaternion(
            m_cRotationalVelocity * m_cPM3DEngine.GetPhysicsClockTick(),
            CVector3::Z));
      /* Prevent the robot from getting out of the arena */
      //m_cPM3DEngine.ClampPosition(m_cEmbodiedEntity.GetPosition());
      /*
       * Update velocity information
       */
      m_cLinearVelocity += (m_cPM3DEngine.GetPhysicsClockTick() / BODY_MASS) * m_cLinearAcceleration;
      m_cRotationalVelocity += (m_cPM3DEngine.GetPhysicsClockTick() / BODY_INERTIA) * m_cRotationalAcceleration;
      /*
       * Update control information
       */
      /* Linear control */
      m_cLinearControl.Set(
         Clamp(MAX_FORCE_X, PDControl(
            m_sDesiredPositionData.Position.GetX(),
            m_cEmbodiedEntity.GetPosition().GetX(),
            POS_K_P.GetX(),
            POS_K_D.GetX(),
            m_pfPosError[0])),
         Clamp(MAX_FORCE_Y, PDControl(
            m_sDesiredPositionData.Position.GetY(),
            m_cEmbodiedEntity.GetPosition().GetY(),
            POS_K_P.GetY(),
            POS_K_D.GetY(),
            m_pfPosError[1])),
         Clamp(MAX_FORCE_Z, PDControl(
            m_sDesiredPositionData.Position.GetZ(),
            m_cEmbodiedEntity.GetPosition().GetZ(),
            POS_K_P.GetZ(),
            POS_K_D.GetZ(),
            m_pfPosError[2]) - BODY_MASS * m_cPM3DEngine.GetGravity()));
      /* Rotational control */
      CRadians cZAngle, cYAngle, cXAngle;
      m_cEmbodiedEntity.GetOrientation().ToEulerAngles(cZAngle, cYAngle, cXAngle);
      m_fRotationalControl =
         Clamp(MAX_TORQUE, PDControl(
            m_sDesiredPositionData.Yaw.GetValue(),
            cZAngle.GetValue(),
            ORIENT_K_P,
            ORIENT_K_D,
            m_fOrientError));
      /*
       * Update force/torque information
       */
      m_cLinearAcceleration.SetX(m_cLinearControl.GetX());
      m_cLinearAcceleration.SetY(m_cLinearControl.GetY());
      m_cLinearAcceleration.SetZ(m_cLinearControl.GetZ() + BODY_MASS * m_cPM3DEngine.GetGravity());
      m_cRotationalAcceleration.SetValue(m_fRotationalControl);
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEyeBotModel::UpdateEntityStatus() {
      /* Update bounding box */
      CalculateBoundingBox();
      /* Update the components */
      m_cEyeBotEntity.UpdateComponents();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEyeBotModel::CalculateBoundingBox() {
      GetBoundingBox().MinCorner.Set(
         m_cEmbodiedEntity.GetPosition().GetX() - BODY_RADIUS,
         m_cEmbodiedEntity.GetPosition().GetY() - BODY_RADIUS,
         m_cEmbodiedEntity.GetPosition().GetZ());
      GetBoundingBox().MaxCorner.Set(
         m_cEmbodiedEntity.GetPosition().GetX() + BODY_RADIUS,
         m_cEmbodiedEntity.GetPosition().GetY() + BODY_RADIUS,
         m_cEmbodiedEntity.GetPosition().GetZ() + BODY_HEIGHT);
   }

   /****************************************/
   /****************************************/

   Real CPointMass3DEyeBotModel::PDControl(Real f_desired,
                                           Real f_current,
                                           Real f_k_p,
                                           Real f_k_d,
                                           Real& f_old_error) {
      Real f_error = f_desired - f_current;
      Real f_output =
         f_k_p * f_error +                                                      /* proportional term */
         f_k_d * (f_error - f_old_error) / m_cPM3DEngine.GetPhysicsClockTick(); /* derivative term */
      f_old_error = f_error;
      return f_output;
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_POINTMASS3D_OPERATIONS_ON_ENTITY(CEyeBotEntity, CPointMass3DEyeBotModel);

   /****************************************/
   /****************************************/

}
