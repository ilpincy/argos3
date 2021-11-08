/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_system_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_pipuck_system_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckSystemSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "system");
      CLuaUtility::AddToTable(pt_lua_state, "time", m_fTime);
      CLuaUtility::AddToTable(pt_lua_state, "temperature", m_fTemperature);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckSystemSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "system");
      CLuaUtility::AddToTable(pt_lua_state, "time", m_fTime);
      CLuaUtility::AddToTable(pt_lua_state, "temperature", m_fTemperature);
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}
