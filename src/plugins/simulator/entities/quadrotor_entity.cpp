/**
 * @file <argos3/plugins/simulator/entities/quadrotor_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "quadrotor_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CQuadRotorEntity::SPositionControlData::SPositionControlData(
      const CVector3& c_position,
      const CRadians& c_yaw) :
      Position(c_position),
      Yaw(c_yaw) {}

   /****************************************/
   /****************************************/

   CQuadRotorEntity::SSpeedControlData::SSpeedControlData(
      const CVector3& c_velocity,
      const CRadians& c_rot_speed) :
      Velocity(c_velocity),
      RotSpeed(c_rot_speed) {}

   /****************************************/
   /****************************************/

   CQuadRotorEntity::CQuadRotorEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent),
      m_eControlMethod(NO_CONTROL) {
      Reset();
   }

   /****************************************/
   /****************************************/

   CQuadRotorEntity::CQuadRotorEntity(CComposableEntity* pc_parent,
                                      const std::string& str_id) :
      CEntity(pc_parent, str_id),
      m_eControlMethod(NO_CONTROL) {
      Reset();
   }

   /****************************************/
   /****************************************/

   void CQuadRotorEntity::Reset() {
      m_sPositionControlData.Position.Set(0.0f, 0.0f, 0.0f);
      m_sPositionControlData.Yaw.SetValue(0.0f);
      m_sSpeedControlData.Velocity.Set(0.0f, 0.0f, 0.0f);
      m_sSpeedControlData.RotSpeed.SetValue(0.0);
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CQuadRotorEntity);

   /****************************************/
   /****************************************/

}
