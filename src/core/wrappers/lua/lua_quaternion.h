#ifndef LUA_QUATERNION_H
#define LUA_QUATERNION_H

/**
 * @file <argos3/core/wrappers/lua/lua_quaternion.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

namespace argos {
   class CRadians;
   class CVector2;
   class CVector3;
   class CQuaternion;
   class CColor;
}

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <argos3/core/utility/datatypes/datatypes.h>

#include <string>
#include <iostream>

namespace argos {

   class CLuaQuaternion {

   public:

      static const std::string& GetMetatableKey() {
         return m_strMetatableKey;
      }

      static void RegisterMetatable(lua_State* pt_state);

      static int Create(lua_State* pt_state);

      static int ToString(lua_State* pt_state);

      static int Equal(lua_State* pt_state);

      static int Multiply(lua_State* pt_state);

      static int Normalize(lua_State* pt_state);

      static int Inverse(lua_State* pt_state);

      static int ToAngleAxis(lua_State* pt_state);

      static int FromLuaState(lua_State* pt_state,
                              int n_index,
                              CQuaternion& c_quaternion);

      static void ToLuaState(lua_State* pt_state,
                             int n_index,
                             const CQuaternion& c_quaternion);

      static void ToLuaState(lua_State* pt_state,
                             const CQuaternion& c_quaternion);

   private:

      static const std::string m_strMetatableKey;

   };

}

#endif
