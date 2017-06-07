/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_kilobot_light_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_kilobot_light_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   CCI_KilobotLightSensor::CCI_KilobotLightSensor() :
      m_nReading(-1) {
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_KilobotLightSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "light");
      CLuaUtility::StartTable(pt_lua_state, 1);
      CLuaUtility::AddToTable(pt_lua_state, "value", m_nReading);
      CLuaUtility::EndTable  (pt_lua_state);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_KilobotLightSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "light");
      lua_pushnumber(pt_lua_state, 1);
      lua_gettable  (pt_lua_state, -2);
      lua_pushnumber(pt_lua_state, m_nReading);
      lua_setfield  (pt_lua_state, -2, "value");
      lua_pop(pt_lua_state, 2);
   }
#endif


   /****************************************/
   /****************************************/

}
