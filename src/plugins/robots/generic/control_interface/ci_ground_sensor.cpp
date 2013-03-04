/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_ground_sensor.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "ci_ground_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_GroundSensor::CreateLuaVariables(lua_State* pt_lua_state) {
      lua_pushstring(pt_lua_state, "ground");
      lua_newtable  (pt_lua_state);
      for(size_t i = 0; i < m_tReadings.size(); ++i) {
         lua_pushnumber(pt_lua_state, i+1           );
         lua_pushnumber(pt_lua_state, m_tReadings[i]);
         lua_settable  (pt_lua_state, -3            );
      }
      lua_settable(pt_lua_state, -3);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_GroundSensor::ReadingsToLuaVariables(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "ground");
      for(size_t i = 0; i < m_tReadings.size(); ++i) {
         lua_pushnumber(pt_lua_state, i+1           );
         lua_pushnumber(pt_lua_state, m_tReadings[i]);
         lua_settable  (pt_lua_state, -3            );
      }
      lua_pop(pt_lua_state, 1);
   }
#endif


   /****************************************/
   /****************************************/

}
