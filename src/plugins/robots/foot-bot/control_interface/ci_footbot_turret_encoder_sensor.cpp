/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_turret_encoder_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_footbot_turret_encoder_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {
   
   /****************************************/
    /****************************************/
   
   const CRange<CRadians> CCI_FootBotTurretEncoderSensor::ANGULAR_RANGE(CRadians(-ARGOS_PI), CRadians(ARGOS_PI));
   
   /****************************************/
   /****************************************/
   
   const CRadians& CCI_FootBotTurretEncoderSensor::GetRotation() const {
     return m_cRotation;
   }

   /****************************************/
   /****************************************/
   
#ifdef ARGOS_WITH_LUA
   void CCI_FootBotTurretEncoderSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "turret");
      CLuaUtility::AddToTable(pt_lua_state, "rotation",  m_cRotation);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_FootBotTurretEncoderSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "turret");
      lua_pushnumber(pt_lua_state, m_cRotation.GetValue());
      lua_setfield(pt_lua_state, -2, "rotation");
      lua_pop(pt_lua_state, 1);
   }
#endif


   /****************************************/
   /****************************************/

}
