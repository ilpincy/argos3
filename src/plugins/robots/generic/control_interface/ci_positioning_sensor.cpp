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
      CLuaUtility::AddToTable(pt_lua_state, "position",    m_sReading.Position);
      CLuaUtility::AddToTable(pt_lua_state, "orientation", m_sReading.Orientation);
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}
