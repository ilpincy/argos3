/**
 * @file <argos3/plugins/robots/foot-bot/simulator/pointmass3d_eyebot_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef POINTMASS3D_EYEBOT_H
#define POINTMASS3D_EYEBOT_H

namespace argos {
   class CPointMass3DEngine;
   class CPointMass3DQuadRotorModel;
   class CPointMass3DEyeBotModel;
   class CQuadRotorEntity;
}

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_quadrotor_model.h>
#include <argos3/plugins/robots/eye-bot/simulator/eyebot_entity.h>

namespace argos {

   class CPointMass3DEyeBotModel : public CPointMass3DQuadRotorModel {

   public:
      
      CPointMass3DEyeBotModel(CPointMass3DEngine& c_engine,
                              CEyeBotEntity& c_eyebot);
      virtual ~CPointMass3DEyeBotModel() {}

      void UpdatePerspectiveCameraAnchor(SAnchor& s_anchor);
      
   };

}

#endif
