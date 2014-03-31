/**
 * @file <argos3/plugins/robots/foot-bot/simulator/pointmass3d_eyebot_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef POINTMASS3D_EYEBOT_H
#define POINTMASS3D_EYEBOT_H

namespace argos {
   class CPointMass3DEngine;
   class CPointMass3DEyeBotModel;
   class CQuadRotorEntity;
}

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_model.h>
#include <argos3/plugins/simulator/entities/quadrotor_entity.h>
#include <argos3/plugins/robots/eye-bot/simulator/eyebot_entity.h>

namespace argos {

   class CPointMass3DEyeBotModel : public CPointMass3DModel {

   public:
      
      CPointMass3DEyeBotModel(CPointMass3DEngine& c_engine,
                              CEyeBotEntity& c_eyebot);
      virtual ~CPointMass3DEyeBotModel() {}
      
      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual void Reset();

      virtual void UpdateFromEntityStatus();
      virtual void Step();
      virtual void UpdateEntityStatus();

      virtual void CalculateBoundingBox();

   private:

      Real PDControl(Real f_cur_error,
                     Real f_k_p,
                     Real f_k_d,
                     Real& f_old_error);
   private:

      CEyeBotEntity& m_cEyeBotEntity;
      CQuadRotorEntity& m_cQuadRotorEntity;
      CQuadRotorEntity::SPositionControlData m_sDesiredPositionData;
      CRadians m_cRotationalVelocity;
      CRadians m_cRotationalAcceleration;

      CVector3 m_cLinearControl;
      Real m_fRotationalControl;

      Real m_pfPosError[3];
      Real m_fOrientError;

   };

}

#endif
