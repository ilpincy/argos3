/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_lift_system_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "ci_builderbot_lift_system_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotLiftSystemSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "lift_system");
      CLuaUtility::AddToTable(pt_lua_state, "state", m_arrStateStrings[m_unState].c_str());
      CLuaUtility::AddToTable(pt_lua_state, "position", m_fPosition);
      CLuaUtility::StartTable(pt_lua_state, "limit_switches");
      CLuaUtility::AddToTable(pt_lua_state, "top", m_arrLimitSwitches[0]);
      CLuaUtility::AddToTable(pt_lua_state, "bottom", m_arrLimitSwitches[1]);
      CLuaUtility::EndTable(pt_lua_state);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotLiftSystemSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "lift_system");
      CLuaUtility::AddToTable(pt_lua_state, "state", m_arrStateStrings[m_unState].c_str());
      CLuaUtility::AddToTable(pt_lua_state, "position", m_fPosition);
      lua_getfield(pt_lua_state, -1, "limit_switches");
      CLuaUtility::AddToTable(pt_lua_state, "top", m_arrLimitSwitches[0]);
      CLuaUtility::AddToTable(pt_lua_state, "bottom", m_arrLimitSwitches[1]);
      lua_pop(pt_lua_state, 1);
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}
