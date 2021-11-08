/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_flight_system_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Sinan Oguz - <soguz.ankara@gmail.com>
 */

#include "ci_drone_flight_system_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DroneFlightSystemSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "flight_system");
      CLuaUtility::AddToTable(pt_lua_state, "position", GetPosition());
      CLuaUtility::AddToTable(pt_lua_state, "orientation", GetOrientation());
      CLuaUtility::AddToTable(pt_lua_state, "target_orientation", GetTargetOrientation());
      CLuaUtility::AddToTable(pt_lua_state, "velocity", GetVelocity());
      CLuaUtility::AddToTable(pt_lua_state, "angular_velocity", GetAngularVelocity());
      CLuaUtility::AddToTable(pt_lua_state, "target_position", GetTargetPosition());
      CLuaUtility::AddToTable(pt_lua_state, "height", GetHeight());
      CLuaUtility::AddToTable(pt_lua_state, "battery_voltage", GetBatteryVoltage());
      CLuaUtility::AddToTable(pt_lua_state, "temperature", GetTemperature());
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DroneFlightSystemSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "flight_system");
      CLuaUtility::AddToTable(pt_lua_state, "position", GetPosition());
      CLuaUtility::AddToTable(pt_lua_state, "orientation", GetOrientation());
      CLuaUtility::AddToTable(pt_lua_state, "target_orientation", GetTargetOrientation());
      CLuaUtility::AddToTable(pt_lua_state, "velocity", GetVelocity());
      CLuaUtility::AddToTable(pt_lua_state, "angular_velocity", GetAngularVelocity());
      CLuaUtility::AddToTable(pt_lua_state, "target_position", GetTargetPosition());
      CLuaUtility::AddToTable(pt_lua_state, "height", GetHeight());
      CLuaUtility::AddToTable(pt_lua_state, "battery_voltage", GetBatteryVoltage());
      CLuaUtility::AddToTable(pt_lua_state, "temperature", GetTemperature());
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}
