/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_imu_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_pipuck_imu_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckImuSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "imu");
      CLuaUtility::AddToTable(pt_lua_state, "accelerometer", GetAccelerometer());
      CLuaUtility::AddToTable(pt_lua_state, "gyroscope", GetGryoscope());
      CLuaUtility::AddToTable(pt_lua_state, "magnetometer", GetMagnetometer());
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckImuSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "imu");
      CLuaUtility::AddToTable(pt_lua_state, "accelerometer", GetAccelerometer());
      CLuaUtility::AddToTable(pt_lua_state, "gyroscope", GetGryoscope());
      CLuaUtility::AddToTable(pt_lua_state, "magnetometer", GetMagnetometer());
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}
