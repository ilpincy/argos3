#ifndef LUA_QUATERNION_H
#define LUA_QUATERNION_H

/**
 * @file <argos3/core/wrappers/lua/lua_quaternion.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/math/quaternion.h>

#include <string>
#include <utility>

namespace argos {

   class CLuaQuaternion {

   public:

      static void RegisterType(lua_State* pt_state);

      static const std::string& GetTypeId() {
         return m_strTypeId;
      }

      static int Create(lua_State* pt_state);
      
      template<class... TArguments>
      static void PushQuaternion(lua_State* pt_state, TArguments&&... t_arguments) {
         /* create a new quaternion */
         void* pvUserdatum = 
            lua_newuserdata(pt_state, sizeof(CQuaternion));
         /* run the constructor on the allocated memory */
         new (pvUserdatum) CQuaternion(std::forward<TArguments>(t_arguments)...);
         /* set the metatable for the userdatum */
         luaL_getmetatable(pt_state, m_strTypeId.c_str());
         lua_setmetatable(pt_state, -2);
      }

      static CQuaternion& ToQuaternion(lua_State* pt_state, int n_index);

      static int Index(lua_State* pt_state);

      static int NewIndex(lua_State* pt_state);

      static int ToString(lua_State* pt_state);

      static int Equal(lua_State* pt_state);

      static int Multiply(lua_State* pt_state);

      static int Normalize(lua_State* pt_state);

      static int Inverse(lua_State* pt_state);

      static int ToAngleAxis(lua_State* pt_state);

      static int ToEulerAngles(lua_State* pt_state);


   private:

      static const std::string m_strTypeId;

   };

}

#endif
