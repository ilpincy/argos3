/**
 * @file <argos3/plugins/robots/foot-bot/simulator/pointmass3d_eyebot_model.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "pointmass3d_eyebot_model.h"
#include <argos3/plugins/simulator/physics_engines/pointmass3d/pointmass3d_engine.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   static const Real SURFACE_AREA = 0.2f;
   static const Real DRAG_COEFFICIENT = 16.6f; // Drag coefficient
   static const Real AIR_DENSITY = 1.205f; // Room Temperature
   static const Real MASS = 0.5f;
   static const Real EYEBOT_HEIGHT = 0.566;

   /****************************************/
   /****************************************/

   CPointMass3DEyeBotModel::CPointMass3DEyeBotModel(CPointMass3DEngine& c_engine,
                                                    CEyeBotEntity& c_eyebot) :
      CPointMass3DModel(c_engine, c_eyebot.GetEmbodiedEntity()),
      m_cEyeBotEntity(c_eyebot),
      m_cQuadRotorEntity(c_eyebot.GetQuadRotorEntity()) {
      DEBUG_FUNCTION_ENTER;
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
      /* Update the components */
      m_cEyeBotEntity.UpdateComponents();
      DEBUG_FUNCTION_EXIT;
   }

   /****************************************/
   /****************************************/

   void CPointMass3DEyeBotModel::CalculateBoundingBox() {
      DEBUG_FUNCTION_ENTER;
      DEBUG_FUNCTION_EXIT;
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_POINTMASS3D_OPERATIONS_ON_ENTITY(CEyeBotEntity, CPointMass3DEyeBotModel);

   /****************************************/
   /****************************************/

}
