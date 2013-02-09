
/**
 * @file <argos3/plugins/robots/generic/wrappers/lua/lua_differential_steering_actuator.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "lua_differential_steering_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/

   CLuaDifferentialSteeringActuator::CLuaDifferentialSteeringActuator(CCI_DifferentialSteeringActuator& c_actuator,
                                                                      lua_State* pt_lua_state,
                                                                      const std::string& str_var_name) :
      CLuaActuator<CCI_DifferentialSteeringActuator>(c_actuator,
                                                     pt_lua_state,
                                                        str_var_name) {}
      
   /****************************************/
   /****************************************/

   void CLuaDifferentialSteeringActuator::CreateLuaVariables() {
      lua_pushstring(GetLuaState(), GetVarName().c_str());
      lua_newtable   (GetLuaState()                     );
      lua_pushstring(GetLuaState(),               "left");
      lua_pushnumber (GetLuaState(),                   0);
      lua_settable   (GetLuaState(),                  -3);
      lua_pushstring(GetLuaState(),              "right");
      lua_pushnumber (GetLuaState(),                   0);
      lua_settable   (GetLuaState(),                  -3);
      lua_settable   (GetLuaState(),                  -3);
   }

   /****************************************/
   /****************************************/

   void CLuaDifferentialSteeringActuator::LuaVariablesToSettings() {
      lua_getfield(GetLuaState(), -1, GetVarName().c_str());
      lua_getfield(GetLuaState(), -1, "left");
      lua_getfield(GetLuaState(), -2, "right");
      GetActuator().SetLinearVelocity(lua_tonumber(GetLuaState(), -2),
                                      lua_tonumber(GetLuaState(), -1));
      lua_pop(GetLuaState(), 3);
   }

   /****************************************/
   /****************************************/

}
