/**
 * @file <argos3/plugins/robots/foot-bot/simulator/pointmass3d_eyebot_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "pointmass3d_eyebot_model.h"
#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_engine.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   static const Real BODY_HEIGHT = 0.566f;
   static const Real BODY_RADIUS = 0.25f;

   /****************************************/
   /****************************************/

   CPointMass3DEyeBotModel::CPointMass3DEyeBotModel(CPointMass3DEngine& c_engine,
                                                    CEyeBotEntity& c_eyebot) :
      CPointMass3DModel(c_engine, c_eyebot.GetEmbodiedEntity()),
      m_cEyeBotEntity(c_eyebot),
      m_cQuadRotorEntity(c_eyebot.GetQuadRotorEntity()) {
      DEBUG_FUNCTION_ENTER;
      CalculateBoundingBox();
      DEBUG_FUNCTION_EXIT;
   }

   /****************************************/
   /****************************************/

   bool CPointMass3DEyeBotModel::MoveTo(const CVector3& c_position,
                                        const CQuaternion& c_orientation,
                                        bool b_check_only) {
      DEBUG_FUNCTION_ENTER;
      if(! b_check_only) {
         m_cEyeBotEntity.GetEmbodiedEntity().SetPosition(c_position);
         m_cEyeBotEntity.GetEmbodiedEntity().SetOrientation(c_orientation);
         CalculateBoundingBox();
      }
      DEBUG_FUNCTION_EXIT;
      return true;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEyeBotModel::UpdateFromEntityStatus() {
      DEBUG_FUNCTION_ENTER;
      m_sDesiredPositionData = m_cQuadRotorEntity.GetPositionControlData();
      DEBUG_FUNCTION_EXIT;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEyeBotModel::Step() {
      DEBUG_FUNCTION_ENTER;
      DEBUG_FUNCTION_EXIT;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEyeBotModel::UpdateEntityStatus() {
      DEBUG_FUNCTION_ENTER;
      /* Update bounding box */
      CalculateBoundingBox();
      /* Update the components */
      m_cEyeBotEntity.UpdateComponents();
      DEBUG_FUNCTION_EXIT;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEyeBotModel::CalculateBoundingBox() {
      DEBUG_FUNCTION_ENTER;
      GetBoundingBox().MinCorner.Set(
         m_cEmbodiedEntity.GetPosition().GetX() - BODY_RADIUS,
         m_cEmbodiedEntity.GetPosition().GetY() - BODY_RADIUS,
         m_cEmbodiedEntity.GetPosition().GetZ());
      GetBoundingBox().MaxCorner.Set(
         m_cEmbodiedEntity.GetPosition().GetX() + BODY_RADIUS,
         m_cEmbodiedEntity.GetPosition().GetY() + BODY_RADIUS,
         m_cEmbodiedEntity.GetPosition().GetZ() + BODY_HEIGHT);
      DEBUG_FUNCTION_EXIT;
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_POINTMASS3D_OPERATIONS_ON_ENTITY(CEyeBotEntity, CPointMass3DEyeBotModel);

   /****************************************/
   /****************************************/

}
