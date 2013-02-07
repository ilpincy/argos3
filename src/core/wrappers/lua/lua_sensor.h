
/**
 * @file <argos3/core/wrappers/lua/lua_sensor.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef LUA_SENSOR_H
#define LUA_SENSOR_H

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <string>

namespace argos {

   template <class SENSOR>
   class CLuaSensor {

   public:

      CLuaSensor(SENSOR& c_sensor,
                 lua_State* pt_lua_state,
                 const std::string& str_var_name) :
         m_cSensor(c_sensor),
         m_ptLuaState(pt_lua_state),
         m_strVarName(str_var_name) {}

      virtual ~CLuaSensor() {}

      virtual void CreateLuaVariables() = 0;

      virtual void ReadingsToLuaVariables() = 0;

      inline SENSOR& GetSensor() {
         return m_cSensor;
      }

      inline lua_State* GetLuaState() {
         return m_ptLuaState;
      }

      inline const std::string& GetVarName() const {
         return m_strVarName;
      }

   private:

      SENSOR& m_cSensor;
      lua_State* m_ptLuaState;
      std::string m_strVarName;

   };

}

#endif
