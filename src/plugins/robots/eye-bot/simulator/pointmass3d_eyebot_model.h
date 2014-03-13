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
}

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_model.h>
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

      virtual void UpdateFromEntityStatus();
      virtual void Step();
      virtual void UpdateEntityStatus();

      virtual void CalculateBoundingBox();

   private:

      CEyeBotEntity& m_cEyeBotEntity;

   };

}

#endif
