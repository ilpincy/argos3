/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_differential_drive_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_pipuck_differential_drive_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckDifferentialDriveSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "differential_drive");
      CLuaUtility::StartTable(pt_lua_state, "encoders");
      CLuaUtility::AddToTable(pt_lua_state, "left", GetLeftVelocity());
      CLuaUtility::AddToTable(pt_lua_state, "right", GetRightVelocity());
      CLuaUtility::EndTable(pt_lua_state);      
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckDifferentialDriveSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "differential_drive");
      lua_getfield(pt_lua_state, -1, "encoders");
      CLuaUtility::AddToTable(pt_lua_state, "left", GetLeftVelocity());
      CLuaUtility::AddToTable(pt_lua_state, "right", GetRightVelocity());
      lua_pop(pt_lua_state, 1);
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}
