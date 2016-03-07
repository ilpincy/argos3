/**
 * @file <argos3/plugins/robots/foot-bot/simulator/pointmass3d_eyebot_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "pointmass3d_eyebot_model.h"
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
   static const Real     YAW_K_P = 0.5f;
   static const Real     YAW_K_D = 0.1f;
   static const CVector3 VEL_K_P(20.0f, 20.0f, 20.0f);
   static const CVector3 VEL_K_D(10.0f, 10.0f, 10.0f);
   static const Real     ROT_K_P = 0.5f;
   static const Real     ROT_K_D = 0.1f;

   /****************************************/
   /****************************************/

   CPointMass3DEyeBotModel::CPointMass3DEyeBotModel(CPointMass3DEngine& c_engine,
                                                    CEyeBotEntity& c_eyebot) :
      CPointMass3DQuadRotorModel(c_engine,
                                 c_eyebot.GetEmbodiedEntity(),
                                 c_eyebot.GetQuadRotorEntity(),
                                 BODY_HEIGHT,
                                 BODY_RADIUS,
                                 BODY_MASS,
                                 BODY_INERTIA,
                                 POS_K_P,
                                 POS_K_D,
                                 YAW_K_P,
                                 YAW_K_D,
                                 VEL_K_P,
                                 VEL_K_D,
                                 ROT_K_P,
                                 ROT_K_D) {
      RegisterAnchorMethod<CPointMass3DEyeBotModel>(
         GetEmbodiedEntity().GetAnchor("perspective_camera"),
         &CPointMass3DEyeBotModel::UpdatePerspectiveCameraAnchor);
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEyeBotModel::UpdatePerspectiveCameraAnchor(SAnchor& s_anchor) {
      s_anchor.Position = m_cPosition;
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_POINTMASS3D_OPERATIONS_ON_ENTITY(CEyeBotEntity, CPointMass3DEyeBotModel);

   /****************************************/
   /****************************************/

}
