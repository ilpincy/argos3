/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_wheel_speed_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_footbot_wheel_speed_sensor.h"
#include <argos3/core/wrappers/lua/lua_utility.h>

namespace argos {
   
   /****************************************/
   /****************************************/
   
#ifdef ARGOS_WITH_LUA
   void CCI_FootBotWheelSpeedSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable (pt_lua_state, "wheels"                                        );
      CLuaUtility::AddToTable          (pt_lua_state, "velocity_left",  m_sReading.LinearVelocityLeft );
      CLuaUtility::AddToTable          (pt_lua_state, "velocity_right", m_sReading.LinearVelocityRight);
      CLuaUtility::CloseRobotStateTable(pt_lua_state                                                  );
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_FootBotWheelSpeedSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "wheels");
      lua_pushnumber(pt_lua_state, m_sReading.LinearVelocityLeft);
      lua_setfield(pt_lua_state, -2, "velocity_left");
      lua_pushnumber(pt_lua_state, m_sReading.LinearVelocityRight);
      lua_setfield(pt_lua_state, -2, "velocity_right");
      lua_pop(pt_lua_state, 1);
   }
#endif


   /****************************************/
   /****************************************/

}
