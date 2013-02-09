
/**
 * @file <argos3/plugins/robots/generic/wrappers/lua/lua_ring_proximity_sensor.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "lua_ring_proximity_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/
   
   void CLuaRingProximitySensor::CreateLuaVariables() {
      lua_pushstring(GetLuaState(), GetVarName().c_str());
      lua_newtable   (GetLuaState());
      for(size_t i = 0; i < GetSensor().GetReadings().size(); ++i) {
         lua_pushnumber(GetLuaState(), i+1);
         lua_newtable(GetLuaState());
         lua_pushstring(GetLuaState(), "angle");
         lua_pushnumber(GetLuaState(), GetSensor().GetAngles()[i].GetValue());
         lua_settable(GetLuaState(), -3);
         lua_pushstring(GetLuaState(), "value");
         lua_pushnumber(GetLuaState(), GetSensor().GetReadings()[i]);
         lua_settable(GetLuaState(), -3);
         lua_settable(GetLuaState(), -3);
      }
      lua_settable(GetLuaState(), -3);
   }

   /****************************************/
   /****************************************/
   
   void CLuaRingProximitySensor::ReadingsToLuaVariables() {
      lua_getfield(GetLuaState(), -1, GetVarName().c_str());
      for(size_t i = 0; i < GetSensor().GetReadings().size(); ++i) {
         lua_pushnumber(GetLuaState(), i+1);
         lua_gettable  (GetLuaState(), -2);
         lua_pushnumber(GetLuaState(), GetSensor().GetReadings()[i]);
         lua_setfield  (GetLuaState(), -2, "value");
         lua_pop(GetLuaState(), 1);
      }
      lua_pop(GetLuaState(), 1);
   }

   /****************************************/
   /****************************************/
   
}
