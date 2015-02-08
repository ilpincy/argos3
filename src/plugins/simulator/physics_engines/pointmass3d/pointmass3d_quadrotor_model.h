/**
 * @file <argos3/plugins/simulator/pointmass3d/pointmass3d_quadrotor_model.h>
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
                                 CQuadRotorEntity& c_quadrotor);
      virtual ~CPointMass3DQuadRotorModel() {}
      
      virtual void Reset();

      virtual void UpdateFromEntityStatus();
      virtual void Step();

      virtual void CalculateBoundingBox();

      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const;

   private:

      Real PDControl(Real f_cur_error,
                     Real f_k_p,
                     Real f_k_d,
                     Real& f_old_error);

   private:

      /** Reference to the quadrotor entity */
      CQuadRotorEntity& m_cQuadRotorEntity;
      
      /** The desired positional data */
      CQuadRotorEntity::SPositionControlData m_sDesiredPositionData;

      /** Current yaw of the quadrotor */
      CRadians m_cYaw;

      /** Current rotational speed */
      CRadians m_cRotationalSpeed;

      /** Current torque */
      CRadians m_cTorque;

      /** Linear control applied on the quadrotor */
      CVector3 m_cLinearControl;

      /** Rotational control applied on the quadrotor */
      Real m_fRotationalControl;

      Real m_pfPosError[3];
      Real m_fOrientError;
   };

}

#endif
