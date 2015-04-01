/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_altitude_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_altitude_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

  /****************************************/
  /****************************************/

#ifdef ARGOS_WITH_LUA
  void CCI_AltitudeSensor::CreateLuaState(lua_State* pt_lua_state) {
    CLuaUtility::AddToTable(pt_lua_state, "altitude", m_fReading);
  }
#endif

  /****************************************/
  /****************************************/

#ifdef ARGOS_WITH_LUA
  void CCI_AltitudeSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
    lua_pushnumber(pt_lua_state, m_fReading);
    lua_setfield(pt_lua_state, -2, "altitude");
  }
#endif


  /****************************************/
  /****************************************/

  Real CCI_AltitudeSensor::GetReading() const {
    return m_fReading;
  }

}
