/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_battery_sensor.cpp>
 *
 * @author Adhavan Jayabalan <jadhu94@gmail.com>
 */

#include "ci_battery_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   const CCI_BatterySensor::SReading& CCI_BatterySensor::GetReading() const {
     return m_sReading;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BatterySensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "battery");
      CLuaUtility::AddToTable(pt_lua_state, "available_charge", m_sReading.AvailableCharge);
      CLuaUtility::AddToTable(pt_lua_state, "time_left",        m_sReading.TimeLeft);
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BatterySensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "battery");
      lua_pushnumber(pt_lua_state, m_sReading.AvailableCharge);
      lua_setfield(pt_lua_state, -2, "available_charge");
      lua_pushnumber(pt_lua_state, m_sReading.TimeLeft);
      lua_setfield(pt_lua_state, -2, "time_left");
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}
