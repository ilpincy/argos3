
/**
 * @file <argos3/core/wrappers/lua/lua_actuator.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef LUA_ACTUATOR_H
#define LUA_ACTUATOR_H

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <string>

namespace argos {

   template <class ACTUATOR>
   class CLuaActuator {

   public:

      CLuaActuator(ACTUATOR& c_actuator,
                   lua_State* pt_lua_state,
                   const std::string& str_var_name) :
         m_cActuator(c_actuator),
         m_ptLuaState(pt_lua_state),
         m_strVarName(str_var_name) {}

      virtual ~CLuaActuator() {}

      virtual void CreateLuaVariables() = 0;

      virtual void LuaVariablesToSettings() = 0;

      inline ACTUATOR& GetActuator() {
         return m_cActuator;
      }

      inline lua_State* GetLuaState() {
         return m_ptLuaState;
      }

      inline const std::string& GetVarName() const {
         return m_strVarName;
      }

   private:

      ACTUATOR& m_cActuator;
      lua_State* m_ptLuaState;
      std::string m_strVarName;

   };

}

#endif
