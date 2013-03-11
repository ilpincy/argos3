/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "ci_positioning_sensor.h"
#include <argos3/core/wrappers/lua/lua_utility.h>

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PositioningSensor::CreateLuaVariables(lua_State* pt_lua_state) {
      lua_pushstring(pt_lua_state, "positioning");
      lua_newtable  (pt_lua_state);
      lua_pushstring(pt_lua_state, "position");
      lua_newtable  (pt_lua_state);
      lua_pushstring(pt_lua_state, "x");
      lua_pushnumber(pt_lua_state, m_sReading.Position.GetX());
      lua_settable  (pt_lua_state, -3);
      lua_pushstring(pt_lua_state, "y");
      lua_pushnumber(pt_lua_state, m_sReading.Position.GetY());
      lua_settable  (pt_lua_state, -3);
      lua_pushstring(pt_lua_state, "z");
      lua_pushnumber(pt_lua_state, m_sReading.Position.GetZ());
      lua_settable  (pt_lua_state, -3);
      lua_settable(pt_lua_state, -3);
      m_sReading.Orientation.ToAngleAxis(m_cAngle, m_cAxis);
      lua_pushstring(pt_lua_state, "orientation");
      lua_newtable  (pt_lua_state);
      lua_pushstring(pt_lua_state, "angle");
      lua_pushnumber(pt_lua_state, m_cAngle.GetValue());
      lua_settable  (pt_lua_state, -3);
      lua_pushstring(pt_lua_state, "axis");
      lua_newtable  (pt_lua_state);
      lua_pushstring(pt_lua_state, "x");
      lua_pushnumber(pt_lua_state, m_cAxis.GetX());
      lua_settable  (pt_lua_state, -3);
      lua_pushstring(pt_lua_state, "y");
      lua_pushnumber(pt_lua_state, m_cAxis.GetY());
      lua_settable  (pt_lua_state, -3);
      lua_pushstring(pt_lua_state, "z");
      lua_pushnumber(pt_lua_state, m_cAxis.GetZ());
      lua_settable  (pt_lua_state, -3);
      lua_settable(pt_lua_state, -3);
      lua_settable(pt_lua_state, -3);
      lua_settable(pt_lua_state, -3);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PositioningSensor::ReadingsToLuaVariables(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "positioning");
      lua_getfield(pt_lua_state, -1, "position");
      lua_pushnumber(pt_lua_state, m_sReading.Position.GetX());
      lua_setfield(pt_lua_state, -2, "x");
      lua_pushnumber(pt_lua_state, m_sReading.Position.GetY());
      lua_setfield(pt_lua_state, -2, "y");
      lua_pushnumber(pt_lua_state, m_sReading.Position.GetZ());
      lua_setfield(pt_lua_state, -2, "z");
      m_sReading.Orientation.ToAngleAxis(m_cAngle, m_cAxis);
      lua_getfield  (pt_lua_state, -2, "orientation");
      lua_pushnumber(pt_lua_state, m_cAngle.GetValue());
      lua_setfield  (pt_lua_state, -2, "angle");
      lua_getfield  (pt_lua_state, -1, "axis");
      lua_pushnumber(pt_lua_state, m_cAxis.GetX());
      lua_setfield  (pt_lua_state, -2, "x");
      lua_pushnumber(pt_lua_state, m_cAxis.GetY());
      lua_setfield  (pt_lua_state, -2, "y");
      lua_pushnumber(pt_lua_state, m_cAxis.GetZ());
      lua_setfield  (pt_lua_state, -2, "z");
      lua_pop(pt_lua_state, 4);
   }
#endif


   /****************************************/
   /****************************************/

}
