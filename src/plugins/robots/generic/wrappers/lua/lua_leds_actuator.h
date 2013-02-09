
/**
 * @file <argos3/plugins/robots/generic/wrappers/lua/lua_leds_actuator.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef LUA_LEDS_ACTUATOR
#define LUA_LEDS_ACTUATOR

namespace argos {
   class CLuaLEDsActuator;
}

#include <argos3/core/wrappers/lua/lua_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>

namespace argos {

   class CLuaLEDsActuator : public CLuaActuator<CCI_LEDsActuator> {

   public:

      CLuaLEDsActuator(CCI_LEDsActuator& c_actuator,
                       lua_State* pt_lua_state,
                       const std::string& str_var_name);
      
      virtual ~CLuaLEDsActuator() {}

      virtual void CreateLuaVariables();

      virtual void LuaVariablesToSettings();

   private:

   };

}

#endif
