/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "ci_positioning_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   const CCI_PositioningSensor::SReading& CCI_PositioningSensor::GetReading() const {
     return m_sReading;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PositioningSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "positioning");
      CLuaUtility::AddToTable(pt_lua_state, "position",    m_sReading.Position);
      CLuaUtility::AddToTable(pt_lua_state, "orientation", m_sReading.Orientation);
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PositioningSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
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
