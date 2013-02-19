
/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "ci_differential_steering_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DifferentialSteeringActuator::CreateLuaVariables(lua_State* pt_lua_state) {
      lua_pushstring (pt_lua_state, "wheels");
      lua_newtable   (pt_lua_state          );
      lua_pushstring (pt_lua_state, "left"  );
      lua_pushnumber (pt_lua_state, 0       );
      lua_settable   (pt_lua_state, -3      );
      lua_pushstring (pt_lua_state, "right" );
      lua_pushnumber (pt_lua_state, 0       );
      lua_settable   (pt_lua_state, -3      );
      lua_settable   (pt_lua_state, -3      );
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DifferentialSteeringActuator::LuaVariablesToSettings(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "wheels");
      lua_getfield(pt_lua_state, -1, "left");
      lua_getfield(pt_lua_state, -2, "right");
      SetLinearVelocity(lua_tonumber(pt_lua_state, -2),
                        lua_tonumber(pt_lua_state, -1));
      lua_pop(pt_lua_state, 3);
   }
#endif

   /****************************************/
   /****************************************/

}
