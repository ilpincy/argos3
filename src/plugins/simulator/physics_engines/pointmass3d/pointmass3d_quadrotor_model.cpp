/**
 * @file <argos3/plugins/rorotors/foot-rotor/simulator/pointmass3d_quadrotor_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "pointmass3d_quadrotor_model.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/cylinder.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_engine.h>

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

   static Real SymmetricClamp(Real f_max,
                              Real f_value) {
      if(f_value >  f_max) return  f_max;
      if(f_value < -f_max) return -f_max;
      return f_value;
   }

   /****************************************/
   /****************************************/

   CPointMass3DQuadRotorModel::CPointMass3DQuadRotorModel(CPointMass3DEngine& c_engine,
                                                          CEmbodiedEntity& c_body,
                                                          CQuadRotorEntity& c_quadrotor) :
      CPointMass3DModel(c_engine, c_body),
      m_cQuadRotorEntity(c_quadrotor) {
      Reset();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DQuadRotorModel::Reset() {
      CPointMass3DModel::Reset();
      m_pfPosError[0] = 0.0f;
      m_pfPosError[1] = 0.0f;
      m_pfPosError[2] = 0.0f;
      m_fOrientError = 0.0f;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DQuadRotorModel::UpdateFromEntityStatus() {
      m_sDesiredPositionData = m_cQuadRotorEntity.GetPositionControlData();
   }

   /****************************************/
   /****************************************/

   void CPointMass3DQuadRotorModel::Step() {
      /*
       * Update positional information
       */
      /* Integration step */
      m_cPosition += m_cVelocity        * m_cPM3DEngine.GetPhysicsClockTick();
      m_cYaw      += m_cRotationalSpeed * m_cPM3DEngine.GetPhysicsClockTick();
      /* Limit the quad-rotor position within the arena limits */
      const CRange<CVector3>& cLimits = CSimulator::GetInstance().GetSpace().GetArenaLimits();
      m_cPosition.SetX(
         Min(Max(m_cPosition.GetX(),
                 cLimits.GetMin().GetX() + BODY_RADIUS),
             cLimits.GetMax().GetX() - BODY_RADIUS));
      m_cPosition.SetY(
         Min(Max(m_cPosition.GetY(),
                 cLimits.GetMin().GetY() + BODY_RADIUS),
             cLimits.GetMax().GetY() - BODY_RADIUS));
      m_cPosition.SetZ(
         Min(Max(m_cPosition.GetZ(),
                 cLimits.GetMin().GetZ()),
             cLimits.GetMax().GetZ() - BODY_HEIGHT));
      /* Normalize the yaw */
      m_cYaw.UnsignedNormalize();
      /*
       * Update velocity information
       */
      m_cVelocity        += (m_cPM3DEngine.GetPhysicsClockTick() / BODY_MASS)    * m_cAcceleration;
      m_cRotationalSpeed += (m_cPM3DEngine.GetPhysicsClockTick() / BODY_INERTIA) * m_cTorque;
      /*
       * Update control information
       */
      /* Linear control */
      m_cLinearControl.Set(
         SymmetricClamp(MAX_FORCE_X, PDControl(
                           m_sDesiredPositionData.Position.GetX() - m_cPosition.GetX(),
                           POS_K_P.GetX(),
                           POS_K_D.GetX(),
                           m_pfPosError[0])),
         SymmetricClamp(MAX_FORCE_Y, PDControl(
                           m_sDesiredPositionData.Position.GetY() - m_cPosition.GetY(),
                           POS_K_P.GetY(),
                           POS_K_D.GetY(),
                           m_pfPosError[1])),
         SymmetricClamp(MAX_FORCE_Z, PDControl(
                           m_sDesiredPositionData.Position.GetZ() - m_cPosition.GetZ(),
                           POS_K_P.GetZ(),
                           POS_K_D.GetZ(),
                           m_pfPosError[2]) - BODY_MASS * m_cPM3DEngine.GetGravity()));
      /* Rotational control */
      m_fRotationalControl =
         SymmetricClamp(MAX_TORQUE, PDControl(
                           (m_sDesiredPositionData.Yaw - m_cYaw).SignedNormalize().GetValue(),
                           ORIENT_K_P,
                           ORIENT_K_D,
                           m_fOrientError));
      /*
       * Update force/torque information
       */
      m_cAcceleration.SetX(m_cLinearControl.GetX());
      m_cAcceleration.SetY(m_cLinearControl.GetY());
      m_cAcceleration.SetZ(m_cLinearControl.GetZ() + BODY_MASS * m_cPM3DEngine.GetGravity());
      m_cTorque.SetValue(m_fRotationalControl);
   }

   /****************************************/
   /****************************************/

   void CPointMass3DQuadRotorModel::CalculateBoundingBox() {
      GetBoundingBox().MinCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() - BODY_RADIUS,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() - BODY_RADIUS,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ());
      GetBoundingBox().MaxCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() + BODY_RADIUS,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() + BODY_RADIUS,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ() + BODY_HEIGHT);
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DQuadRotorModel::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                             const CRay3& c_ray) const {
      CCylinder m_cShape(BODY_RADIUS,
                         BODY_HEIGHT,
                         m_cPosition,
                         CVector3::Z);
      return m_cShape.Intersects(f_t_on_ray, c_ray);
   }
   
   /****************************************/
   /****************************************/

   Real CPointMass3DQuadRotorModel::PDControl(Real f_cur_error,
                                              Real f_k_p,
                                              Real f_k_d,
                                              Real& f_old_error) {
      Real fOutput =
         f_k_p * f_cur_error +                                                      /* proportional term */
         f_k_d * (f_cur_error - f_old_error) / m_cPM3DEngine.GetPhysicsClockTick(); /* derivative term */
      f_old_error = f_cur_error;
      return fOutput;
   }

   /****************************************/
   /****************************************/

}
