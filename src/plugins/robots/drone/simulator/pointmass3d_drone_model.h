/**
 * @file <argos3/plugins/robots/drone/simulator/pointmass3d_drone_model.h>
 *
 * @author Sinan Oguz - <soguz.ankara@gmail.com>
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef POINTMASS3D_DRONE_MODEL_H
#define POINTMASS3D_DRONE_MODEL_H

namespace argos {
   class CPointMass3DEngine;
   class CDroneEntity;
   class CDroneFlightSystemEntity;
}

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_model.h>

#include <argos3/core/utility/math/rng.h>

namespace argos {

   class CPointMass3DDroneModel : public CPointMass3DModel {

   public:
      
      CPointMass3DDroneModel(CPointMass3DEngine& c_engine,
                             CDroneEntity& c_drone);

      virtual ~CPointMass3DDroneModel() {}
      
      virtual void Reset();

      virtual void UpdateEntityStatus();

      virtual void UpdateFromEntityStatus();

      virtual void UpdatePhysics();

      virtual void Step() {}

      virtual void MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation);

      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const;

      virtual void CalculateBoundingBox();

      virtual void UpdateOriginAnchor(SAnchor& s_anchor);

   private:
   
      /* Random number generator */
      CRandom::CRNG* m_pcRNG;
      /* pid controller */
      static Real CalculatePIDResponse(Real  f_cur_error,
                                       Real  f_vel_error,
                                       Real  f_sum_error,
                                       Real  f_k_p,
                                       Real  f_k_i,
                                       Real  f_k_d);

      /* reference to the flight system entity */ 
      CDroneFlightSystemEntity& m_cFlightSystemEntity;
      /* position and yaw input from the controller */ 
      Real m_fInputYawAngle;
      CVector3 m_cInputPosition;
      /* home position and yaw angle */
      CVector3 m_cHomePosition;
      Real m_fHomeYawAngle;
      /* local copy of the drone's position */
      CVector3 m_cPosition;
      /* local copy of the drone's orientation */
      CVector3 m_cOrientation;
      /* velocity of the drone */
      CVector3 m_cVelocity;
      CVector3 m_cVelocityPrev;
      /* angular velocity of the drone */
      CVector3 m_cAngularVelocity;
      CVector3 m_cAngularVelocityPrev;
      /* acceleration of the drone */
      CVector3 m_cAccelerationPrev;
      /* angular acceleration of the drone */
      CVector3 m_cAngularAccelerationPrev;
      /* variables for PID controller */
      CVector3 m_cOrientationTargetPrev;
      CVector3 m_cAngularVelocityCumulativeError;
      Real m_fAltitudeCumulativeError;
      Real m_fTargetPositionZPrev;
      /* sensor noise parameters */
      Real m_fGyroBias;
      Real m_fAccelBias;
      Real m_fAngleRandomWalk;
      Real m_fVelocityRandomWalk;

      const static Real ROOT_TWO;
      /* height of the drone's body */ 
      const static Real HEIGHT;
      /* length of the drone's arms */
      const static Real ARM_LENGTH;
      /* mass of the drone */
      const static Real MASS;
      /* inertia of the drone */      
      const static CVector3 INERTIA;
      /* thrust factor */
      const static Real B;
      /* torque factor */
      const static Real D;
      /* rotor coefficient */ 
      const static Real JR;
      /* position controller output limit */
      const static CRange<Real> ROLL_PITCH_LIMIT;
      /* thrust limit */
      const static CRange<Real> THRUST_LIMIT;
      /* torque limit */
      const static CRange<Real> TORQUE_LIMIT;    
      /* max velocity can be achieved in the XY plane */
      const static Real XY_VEL_MAX;
      /* max velocity can be achieved on the Z axis */
      const static Real Z_VEL_MAX;
      /* position controller parameters*/ 
      const static Real XY_POS_KP;
      const static Real XY_VEL_KP;
      /* PID controller constants */
      const static Real ALTITUDE_KP;     
      const static Real ALTITUDE_KI;
      const static Real ALTITUDE_KD;
      const static Real ROLL_PITCH_KP; 
      const static Real ROLL_PITCH_KI; 
      const static Real ROLL_PITCH_KD;
      const static Real YAW_KP; 
      const static Real YAW_KI; 
      const static Real YAW_KD; 
      /* sensors noise constants*/
      const static Real MEMS_GYRO_NOISE_MEAN;
      const static Real MEMS_GYRO_BIAS_MEAN;
      const static Real MEMS_GYRO_BIAS_STD_DEV;
      const static Real MEMS_GYRO_NOISE_STD_DEV_X;
      const static Real MEMS_GYRO_NOISE_STD_DEV_Y;
      const static Real MEMS_GYRO_NOISE_STD_DEV_Z;
      const static Real MEMS_GYRO_RANDOM_WALK_INIT;
      const static Real MEMS_GYRO_BIAS_INIT;
      const static Real MEMS_ACCEL_NOISE_MEAN;
      const static Real MEMS_ACCEL_BIAS_MEAN;
      const static Real MEMS_ACCEL_BIAS_STD_DEV;
      const static Real MEMS_ACCEL_NOISE_STD_DEV_X;
      const static Real MEMS_ACCEL_NOISE_STD_DEV_Y;
      const static Real MEMS_ACCEL_NOISE_STD_DEV_Z;
      const static Real MEMS_ACCEL_RANDOM_WALK_INIT;
      const static Real MEMS_ACCEL_BIAS_INIT;
   
   };

}

#endif
