#ifndef LUA_VECTOR3_H
#define LUA_VECTOR3_H

/**
 * @file <argos3/core/wrappers/lua/lua_vector3.h>
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

   class CLuaVector3 {

   public:

      static const std::string& GetMetatableKey() {
         return m_strMetatableKey;
      }

      static void RegisterMetatable(lua_State* pt_state);

      static int Create(lua_State* pt_state);

      static int ToString(lua_State* pt_state);

      static int Equal(lua_State* pt_state);

      static int Add(lua_State* pt_state);

      static int Multiply(lua_State* pt_state);

      static int Subtract(lua_State* pt_state);

      static int UnaryMinus(lua_State* pt_state);

      static int Normalize(lua_State* pt_state);

      static int Length(lua_State* pt_state);

      static int DotProduct(lua_State* pt_state);

      static int CrossProduct(lua_State* pt_state);

      static int Rotate(lua_State* pt_state);

      static int FromLuaState(lua_State* pt_state,
                              int n_index,
                              CVector3& c_vector);

      static void ToLuaState(lua_State* pt_state,
                             int n_index,
                             const CVector3& c_vector);

      static void ToLuaState(lua_State* pt_state,
                             const CVector3& c_vector);

   private:

      static const std::string m_strMetatableKey;

   };

}

#endif
