/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_system_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "ci_drone_system_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DroneSystemSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "system");
      CLuaUtility::AddToTable(pt_lua_state, "time", m_fTime);
      CLuaUtility::StartTable(pt_lua_state, "temperatures");
      for(UInt32 un_index = 0; un_index < m_arrTemperatures.size(); un_index++) {
         CLuaUtility::AddToTable(pt_lua_state, un_index + 1, m_arrTemperatures[un_index]);
      }
      CLuaUtility::EndTable(pt_lua_state);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DroneSystemSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "system");
      CLuaUtility::AddToTable(pt_lua_state, "time", m_fTime);
      lua_getfield(pt_lua_state, -1, "temperatures");
      for(UInt32 un_index = 0; un_index < m_arrTemperatures.size(); un_index++) {
         CLuaUtility::AddToTable(pt_lua_state, un_index, m_arrTemperatures[un_index]);
      }
      lua_pop(pt_lua_state, 1);
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}
