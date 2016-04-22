/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_quadrotor_model.cpp>
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
                                                          CQuadRotorEntity& c_quadrotor,
                                                          Real f_body_height,
                                                          Real f_arm_length,
                                                          Real f_body_mass,
                                                          Real f_body_inertia,
                                                          const CVector3& c_pos_kp,
                                                          const CVector3& c_pos_kd,
                                                          Real f_yaw_kp,
                                                          Real f_yaw_kd,
                                                          const CVector3& c_vel_kp,
                                                          const CVector3& c_vel_kd,
                                                          Real f_rot_kp,
                                                          Real f_rot_kd,
                                                          const CVector3& c_max_force,
                                                          Real f_max_torque) :
      CPointMass3DModel(c_engine, c_body),
      m_cQuadRotorEntity(c_quadrotor),
      m_fBodyHeight(f_body_height),
      m_fArmLength(f_arm_length),
      m_fBodyMass(f_body_mass),
      m_fBodyInertia(f_body_inertia),
      m_cPosKP(c_pos_kp),
      m_cPosKD(c_pos_kd),
      m_fYawKP(f_yaw_kp),
      m_fYawKD(f_yaw_kd),
      m_cVelKP(c_vel_kp),
      m_cVelKD(c_vel_kd),
      m_fRotKP(f_rot_kp),
      m_fRotKD(f_rot_kd),
      m_cMaxForce(c_max_force),
      m_fMaxTorque(f_max_torque) {
      Reset();
      /* Register the origin anchor update method */
      RegisterAnchorMethod(GetEmbodiedEntity().GetOriginAnchor(),
                           &CPointMass3DModel::UpdateOriginAnchor);
      /* Get initial rotation */
      CRadians cTmp1, cTmp2;
      GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(m_cYaw, cTmp1, cTmp2);
   }

   /****************************************/
   /****************************************/

   void CPointMass3DQuadRotorModel::Reset() {
      CPointMass3DModel::Reset();
      m_pfLinearError[0] = 0.0f;
      m_pfLinearError[1] = 0.0f;
      m_pfLinearError[2] = 0.0f;
      m_fRotError = 0.0f;
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
      m_cPosition += m_cVelocity * m_cPM3DEngine.GetPhysicsClockTick();
      m_cYaw      += m_cRotSpeed * m_cPM3DEngine.GetPhysicsClockTick();
      /* Limit the quad-rotor position within the arena limits */
      const CRange<CVector3>& cLimits = CSimulator::GetInstance().GetSpace().GetArenaLimits();
      m_cPosition.SetX(
         Min(Max(m_cPosition.GetX(),
                 cLimits.GetMin().GetX() + m_fArmLength),
             cLimits.GetMax().GetX() - m_fArmLength));
      m_cPosition.SetY(
         Min(Max(m_cPosition.GetY(),
                 cLimits.GetMin().GetY() + m_fArmLength),
             cLimits.GetMax().GetY() - m_fArmLength));
      m_cPosition.SetZ(
         Min(Max(m_cPosition.GetZ(),
                 cLimits.GetMin().GetZ()),
             cLimits.GetMax().GetZ() - m_fBodyHeight));
      /* Normalize the yaw */
      m_cYaw.UnsignedNormalize();
      /*
       * Update velocity information
       */
      m_cVelocity += (m_cPM3DEngine.GetPhysicsClockTick() / m_fBodyMass)    * m_cAcceleration;
      m_cRotSpeed += (m_cPM3DEngine.GetPhysicsClockTick() / m_fBodyInertia) * m_cTorque;
      /*
       * Update control information
       */
      if(m_cQuadRotorEntity.GetControlMethod() == CQuadRotorEntity::POSITION_CONTROL)
         PositionalControl();
      else
         SpeedControl();
      /*
       * Update force/torque information
       */
      m_cAcceleration.SetX(m_cLinearControl.GetX());
      m_cAcceleration.SetY(m_cLinearControl.GetY());
      m_cAcceleration.SetZ(m_cLinearControl.GetZ() + m_fBodyMass * m_cPM3DEngine.GetGravity());
      m_cTorque.SetValue(m_fRotationalControl);
   }

   /****************************************/
   /****************************************/

   void CPointMass3DQuadRotorModel::CalculateBoundingBox() {
      GetBoundingBox().MinCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() - m_fArmLength,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() - m_fArmLength,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ());
      GetBoundingBox().MaxCorner.Set(
         GetEmbodiedEntity().GetOriginAnchor().Position.GetX() + m_fArmLength,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetY() + m_fArmLength,
         GetEmbodiedEntity().GetOriginAnchor().Position.GetZ() + m_fBodyHeight);
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DQuadRotorModel::CheckIntersectionWithRay(Real& f_t_on_ray,
                                                             const CRay3& c_ray) const {
      CCylinder m_cShape(m_fArmLength,
                         m_fBodyHeight,
                         m_cPosition,
                         CVector3::Z);
      return m_cShape.Intersects(f_t_on_ray, c_ray);
   }
   
   /****************************************/
   /****************************************/

   void CPointMass3DQuadRotorModel::PositionalControl() {
      /* Linear control */
      m_cLinearControl.Set(
         SymmetricClamp(m_cMaxForce.GetX(), PDControl(
                           m_sDesiredPositionData.Position.GetX() - m_cPosition.GetX(),
                           m_cPosKP.GetX(),
                           m_cPosKD.GetX(),
                           m_pfLinearError[0])),
         SymmetricClamp(m_cMaxForce.GetY(), PDControl(
                           m_sDesiredPositionData.Position.GetY() - m_cPosition.GetY(),
                           m_cPosKP.GetY(),
                           m_cPosKD.GetY(),
                           m_pfLinearError[1])),
         SymmetricClamp(m_cMaxForce.GetZ(), PDControl(
                           m_sDesiredPositionData.Position.GetZ() - m_cPosition.GetZ(),
                           m_cPosKP.GetZ(),
                           m_cPosKD.GetZ(),
                           m_pfLinearError[2]) - m_fBodyMass * m_cPM3DEngine.GetGravity()));
      /* Rotational control */
      m_fRotationalControl =
         SymmetricClamp(m_fMaxTorque, PDControl(
                           (m_sDesiredPositionData.Yaw - m_cYaw).SignedNormalize().GetValue(),
                           m_fYawKP,
                           m_fYawKD,
                           m_fRotError));
   }

   /****************************************/
   /****************************************/

   void CPointMass3DQuadRotorModel::SpeedControl() {
      /* Linear control */
      m_cLinearControl.Set(
         SymmetricClamp(m_cMaxForce.GetX(), PDControl(
                           m_sDesiredSpeedData.Velocity.GetX() - m_cVelocity.GetX(),
                           m_cVelKP.GetX(),
                           m_cVelKD.GetX(),
                           m_pfLinearError[0])),
         SymmetricClamp(m_cMaxForce.GetY(), PDControl(
                           m_sDesiredSpeedData.Velocity.GetY() - m_cVelocity.GetY(),
                           m_cVelKP.GetY(),
                           m_cVelKD.GetY(),
                           m_pfLinearError[1])),
         SymmetricClamp(m_cMaxForce.GetZ(), PDControl(
                           m_sDesiredSpeedData.Velocity.GetZ() - m_cVelocity.GetZ(),
                           m_cVelKP.GetZ(),
                           m_cVelKD.GetZ(),
                           m_pfLinearError[2]) - m_fBodyMass * m_cPM3DEngine.GetGravity()));
      /* Rotational control */
      m_fRotationalControl =
         SymmetricClamp(m_fMaxTorque, PDControl(
                           (m_sDesiredSpeedData.RotSpeed - m_cRotSpeed).GetValue(),
                           m_fRotKP,
                           m_fRotKD,
                           m_fRotError));
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

   void CPointMass3DQuadRotorModel::UpdateOriginAnchor(SAnchor& s_anchor) {
      s_anchor.Position = m_cPosition;
      s_anchor.Orientation = CQuaternion(m_cYaw, CVector3::Z);
   }

   /****************************************/
   /****************************************/

}
