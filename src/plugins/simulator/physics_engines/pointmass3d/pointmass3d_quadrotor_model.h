/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_quadrotor_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef POINTMASS3D_QUADROTOR_H
#define POINTMASS3D_QUADROTOR_H

namespace argos {
   class CPointMass3DEngine;
   class CPointMass3DQuadRotorModel;
   class CQuadRotorEntity;
}

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_model.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/quadrotor_entity.h>

namespace argos {

   class CPointMass3DQuadRotorModel : public CPointMass3DModel {

   public:
      
      CPointMass3DQuadRotorModel(CPointMass3DEngine& c_engine,
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
                                 const CVector3& c_max_force = CVector3(1000.0f, 1000.0f, 1000.0f),
                                 Real f_max_torque = 1000.0f);

      virtual ~CPointMass3DQuadRotorModel() {}
      
      virtual void Reset();

      virtual void UpdateFromEntityStatus();
      virtual void Step();

      virtual void CalculateBoundingBox();

      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const;

      virtual void UpdateOriginAnchor(SAnchor& s_anchor);

   private:

      void PositionalControl();
      void SpeedControl();

      Real PDControl(Real f_cur_error,
                     Real f_k_p,
                     Real f_k_d,
                     Real& f_old_error);
   private:

      /** Reference to the quadrotor entity */
      CQuadRotorEntity& m_cQuadRotorEntity;

      /** The body height */
      Real m_fBodyHeight;

      /** The arm length */
      Real m_fArmLength;

      /** The body mass in kg */
      Real m_fBodyMass;

      /** The body inertia */
      Real m_fBodyInertia;

      /** The coefficient for proportional position control */
      CVector3 m_cPosKP;

      /** The coefficient for derivative position control */
      CVector3 m_cPosKD;
      
      /** The coefficient for proportional yaw control */
      Real m_fYawKP;

      /** The coefficient for derivative yaw control */
      Real m_fYawKD;

      /** The coefficient for proportional velocity control */
      CVector3 m_cVelKP;

      /** The coefficient for derivative velocity control */
      CVector3 m_cVelKD;

      /** The coefficient for proportional rotational speed control */
      Real m_fRotKP;

      /** The coefficient for derivative rotational speed control */
      Real m_fRotKD;

      /** The maximum lift force */
      CVector3 m_cMaxForce;

      /** The maximum torque */
      Real m_fMaxTorque;

      /** The desired positional data */
      CQuadRotorEntity::SPositionControlData m_sDesiredPositionData;

      /** The desired speed data */
      CQuadRotorEntity::SSpeedControlData m_sDesiredSpeedData;

      /** Current yaw of the quadrotor */
      CRadians m_cYaw;

      /** Current rotational speed */
      CRadians m_cRotSpeed;

      /** Current torque */
      CRadians m_cTorque;

      /** Linear control applied to the quadrotor (position or velocity) */
      CVector3 m_cLinearControl;

      /** Rotational control applied to the quadrotor (rotation or rot speed) */
      Real m_fRotationalControl;

      /** Error for linear control */
      Real m_pfLinearError[3];

      /** Error for rotational control */
      Real m_fRotError;
   };

}

#endif
