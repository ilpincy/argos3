/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_turret_encoder_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_footbot_turret_encoder_sensor.h"
#include <argos3/core/wrappers/lua/lua_utility.h>

namespace argos {
   
   /****************************************/
    /****************************************/
   
   const CRange<CRadians> CCI_FootBotTurretEncoderSensor::ANGULAR_RANGE(CRadians(-ARGOS_PI), CRadians(ARGOS_PI));
   
   /****************************************/
   /****************************************/
   
#ifdef ARGOS_WITH_LUA
   void CCI_FootBotTurretEncoderSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "turret");
      CLuaUtility::AddToTable(pt_lua_state, "rotation",  m_cRotation);
      CLuaUtility::EndTable(pt_lua_state);
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
