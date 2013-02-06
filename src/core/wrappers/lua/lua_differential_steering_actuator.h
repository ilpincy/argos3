#ifndef LUA_DIFFERENTIAL_STEERING_ACTUATOR_H
#define LUA_DIFFERENTIAL_STEERING_ACTUATOR_H

namespace argos {
   class CLuaDifferentialSteeringActuator;
}

#include <argos3/core/wrappers/lua/lua_actuator.h>
#include <argos3/core/control_interface/ci_differential_steering_actuator.h>

namespace argos {

   class CLuaDifferentialSteeringActuator : public CLuaActuator<CCI_DifferentialSteeringActuator> {

   public:

      CLuaDifferentialSteeringActuator(CCI_DifferentialSteeringActuator& c_actuator,
                                       lua_State* pt_lua_state,
                                       const std::string& str_var_name);
      
      virtual ~CLuaDifferentialSteeringActuator() {}

      virtual void CreateLuaVariables();

      virtual void LuaVariablesToSettings();

   private:

   };

}

#endif
