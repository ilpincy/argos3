
/**
 * @file <argos3/core/wrappers/lua/lua_leds_actuator.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "lua_leds_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/

   CLuaLEDsActuator::CLuaLEDsActuator(CCI_LEDsActuator& c_actuator,
                                      lua_State* pt_lua_state,
                                      const std::string& str_var_name) :
      CLuaActuator<CCI_LEDsActuator>(c_actuator,
                                     pt_lua_state,
                                     str_var_name) {}
      
   /****************************************/
   /****************************************/

   void CLuaLEDsActuator::CreateLuaVariables() {
      lua_pushstring(GetLuaState(), GetVarName().c_str());
      lua_newtable   (GetLuaState());
      for(size_t i = 0; i < GetActuator().GetNumLEDs(); ++i) {
         lua_pushnumber(GetLuaState(), i+1);
         lua_newtable(GetLuaState());
         lua_pushstring(GetLuaState(), "red");
         lua_pushnumber(GetLuaState(), 0);
         lua_settable(GetLuaState(), -3);
         lua_pushstring(GetLuaState(), "green");
         lua_pushnumber(GetLuaState(), 0);
         lua_settable(GetLuaState(), -3);
         lua_pushstring(GetLuaState(), "blue");
         lua_pushnumber(GetLuaState(), 0);
         lua_settable(GetLuaState(), -3);
         lua_settable(GetLuaState(), -3);
      }
      lua_settable(GetLuaState(), -3);
   }

   /****************************************/
   /****************************************/

   void CLuaLEDsActuator::LuaVariablesToSettings() {
      lua_getfield(GetLuaState(), -1, GetVarName().c_str());
      CCI_LEDsActuator::TSettings tLEDs(GetActuator().GetNumLEDs());
      for(size_t i = 0; i < GetActuator().GetNumLEDs(); ++i) {
         lua_pushnumber(GetLuaState(), i+1);
         lua_gettable  (GetLuaState(), -2);
         lua_getfield  (GetLuaState(), -1, "red");
         lua_getfield  (GetLuaState(), -2, "green");
         lua_getfield  (GetLuaState(), -3, "blue");
         tLEDs[i].Set(lua_tonumber(GetLuaState(), -3),
                      lua_tonumber(GetLuaState(), -2),
                      lua_tonumber(GetLuaState(), -1));
         lua_pop(GetLuaState(), 4);
      }
      lua_pop(GetLuaState(), 1);
      GetActuator().SetAllColors(tLEDs);
   }

   /****************************************/
   /****************************************/

}
