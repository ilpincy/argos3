#include "ci_ring_proximity_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_RingProximitySensor::CreateLuaVariables(lua_State* pt_lua_state) {
      lua_pushstring(pt_lua_state, "proximity");
      lua_newtable  (pt_lua_state);
      for(size_t i = 0; i < GetReadings().size(); ++i) {
         lua_pushnumber(pt_lua_state, i+1                      );
         lua_newtable  (pt_lua_state                           );
         lua_pushstring(pt_lua_state, "angle"                  );
         lua_pushnumber(pt_lua_state, GetAngles()[i].GetValue());
         lua_settable  (pt_lua_state, -3                       );
         lua_pushstring(pt_lua_state, "value"                  );
         lua_pushnumber(pt_lua_state, GetReadings()[i]         );
         lua_settable  (pt_lua_state, -3                       );
         lua_settable  (pt_lua_state, -3                       );
      }
      lua_settable(pt_lua_state, -3);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_RingProximitySensor::ReadingsToLuaVariables(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "proximity");
      for(size_t i = 0; i < GetReadings().size(); ++i) {
         lua_pushnumber(pt_lua_state, i+1             );
         lua_gettable  (pt_lua_state, -2              );
         lua_pushnumber(pt_lua_state, GetReadings()[i]);
         lua_setfield  (pt_lua_state, -2, "value"     );
         lua_pop(pt_lua_state, 1);
      }
      lua_pop(pt_lua_state, 1);
   }
#endif


   /****************************************/
   /****************************************/

}
