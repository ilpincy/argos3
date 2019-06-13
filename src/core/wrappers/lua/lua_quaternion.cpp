/**
 * @file <argos3/core/wrappers/lua/lua_quaternion.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "lua_quaternion.h"

#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/wrappers/lua/lua_vector3.h>
#include <argos3/core/wrappers/lua/lua_utility.h>

#include <exception>

namespace argos {

   /****************************************/
   /****************************************/

   const std::string CLuaQuaternion::m_strMetatableKey("__metatable_quaternion");

   /****************************************/
   /****************************************/

   void CLuaQuaternion::RegisterMetatable(lua_State* pt_state) {
      /* create a constructor as a global function */
      lua_pushcfunction(pt_state, Create);
      lua_setglobal(pt_state, "quaternion");
      /* create a metatable for quaternions */
      luaL_newmetatable(pt_state, m_strMetatableKey.c_str());     
      lua_pushvalue(pt_state, -1);
      lua_setfield(pt_state, -2, "__index");
      /* set the type */
      lua_pushinteger(pt_state,
         static_cast<lua_Integer>(CLuaUtility::EARGoSType::CQuaternion));
      lua_setfield(pt_state, -2, "__type");
      /* register metamethods */
      CLuaUtility::AddToTable(pt_state, "__tostring", ToString);
      CLuaUtility::AddToTable(pt_state, "__eq", Equal);
      CLuaUtility::AddToTable(pt_state, "__mul", Multiply);
      CLuaUtility::AddToTable(pt_state, "normalize", Normalize);
      CLuaUtility::AddToTable(pt_state, "toangleaxis", ToAngleAxis);
      CLuaUtility::AddToTable(pt_state, "inverse", Inverse);
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::Create(lua_State* pt_state) {
      /* create vector using default constructor */
      CQuaternion cQuaternion;
      /* parse arguments */
      switch(lua_gettop(pt_state)) {
      case 0:
         /* default values */
         break;
      case 1:
         /* copy constructor */
         FromLuaState(pt_state, 1, cQuaternion);
         break;
      case 2:
         /* angle-axis constructor */
         if(lua_isnumber(pt_state, 1)) {
            CRadians cAngle(lua_tonumber(pt_state, 1));
            CVector3 cAxis;
            CLuaVector3::FromLuaState(pt_state, 2, cAxis);
            cQuaternion.FromAngleAxis(cAngle, cAxis);
         }
         else {
            lua_pushstring(pt_state, "invalid arguments to quaternion");
            return lua_error(pt_state);
         }
         break;
      case 4:
         /* value constructor */
         if(lua_isnumber(pt_state, 1) &&
            lua_isnumber(pt_state, 2) &&
            lua_isnumber(pt_state, 3) &&
            lua_isnumber(pt_state, 4)) {
            cQuaternion.Set(lua_tonumber(pt_state, 1),
                            lua_tonumber(pt_state, 2),
                            lua_tonumber(pt_state, 3),
                            lua_tonumber(pt_state, 4));
         }
         else {
            lua_pushstring(pt_state, "invalid arguments to quaternion");
            return lua_error(pt_state);
         }
         break;
      default:
         lua_pushstring(pt_state, "invalid arguments to quaternion");
         return lua_error(pt_state);
         break;
      }
      /* create a new quaternion table */
      ToLuaState(pt_state, cQuaternion);
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::FromLuaState(lua_State* pt_state,
                                    int n_index,
                                    CQuaternion& c_quaternion) {
      if(lua_type(pt_state, n_index) != LUA_TTABLE) {
         lua_pushstring(pt_state, "quaternion not found at requested index");
         return lua_error(pt_state);
      }
      if(luaL_getmetafield(pt_state, n_index, "__type") == 0) {
         lua_pushstring(pt_state, "quaternion not found at requested index");
         return lua_error(pt_state);
      }
      else {
         /* check type */
         lua_Integer nType = lua_tointeger(pt_state, -1);
         lua_pop(pt_state, 1);
         if(nType != static_cast<lua_Integer>(CLuaUtility::EARGoSType::CQuaternion)) {
            lua_pushstring(pt_state, "quaternion not found at requested index");
            return lua_error(pt_state);
         }
      }
      /* copy the table to the top of the stack */
      lua_pushvalue(pt_state, n_index);
      /* push the w,x,y,z components of the table onto the stack */
      lua_getfield(pt_state, -1, "w");
      lua_getfield(pt_state, -2, "x");
      lua_getfield(pt_state, -3, "y");
      lua_getfield(pt_state, -4, "z");
      /* set the quaternion */
      c_quaternion.Set(lua_tonumber(pt_state, -4),
                       lua_tonumber(pt_state, -3),
                       lua_tonumber(pt_state, -2),
                       lua_tonumber(pt_state, -1));
      /* clean up the stack (pop w,x,y,z values and the copy of the table) */
      lua_pop(pt_state, 5);
      return 0;
   }

   /****************************************/
   /****************************************/

   void CLuaQuaternion::ToLuaState(lua_State* pt_state,
                                   const CQuaternion& c_quaternion) {
      /* create a new quaternion table */
      lua_newtable(pt_state);
      luaL_getmetatable(pt_state, m_strMetatableKey.c_str());
      lua_setmetatable(pt_state, -2);
      /* set fields */
      ToLuaState(pt_state, -1, c_quaternion);
   }

   /****************************************/
   /****************************************/

   void CLuaQuaternion::ToLuaState(lua_State* pt_state,
                                   int n_index,
                                   const CQuaternion& c_quaternion) {
      /* copy the table to the top of the stack */
      lua_pushvalue(pt_state, n_index);
      /* set fields */
      CLuaUtility::AddToTable(pt_state, "w", c_quaternion.GetW());
      CLuaUtility::AddToTable(pt_state, "x", c_quaternion.GetX());
      CLuaUtility::AddToTable(pt_state, "y", c_quaternion.GetY());
      CLuaUtility::AddToTable(pt_state, "z", c_quaternion.GetZ());
      /* pop the copy of the table off the stack */
      lua_pop(pt_state, 1);
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::Equal(lua_State* pt_state) {
      CQuaternion cLeftQuaternion;
      CQuaternion cRightQuaternion;
      /* pop the operands from the stack */
      FromLuaState(pt_state, 1, cLeftQuaternion);
      FromLuaState(pt_state, 2, cRightQuaternion);
      /* push the result onto the stack */
      lua_pushboolean(pt_state, cLeftQuaternion == cRightQuaternion);
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::Multiply(lua_State* pt_state) {
      CQuaternion cLeftQuaternion;
      CQuaternion cRightQuaternion;
      /* pop the operands from the stack */
      FromLuaState(pt_state, 1, cLeftQuaternion);
      FromLuaState(pt_state, 2, cRightQuaternion);
      /* perform the multiplication */
      cLeftQuaternion *= cRightQuaternion;
      /* push the result onto the stack */
      ToLuaState(pt_state, cLeftQuaternion);
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::Normalize(lua_State* pt_state) {
      CQuaternion cQuaternion;
      /* pop the operand from the stack */
      FromLuaState(pt_state, 1, cQuaternion);
      /* apply operation */
      cQuaternion.Normalize();
      /* push the result (the operand) onto the stack */
      ToLuaState(pt_state, 1, cQuaternion);
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::Inverse(lua_State* pt_state) {
      CQuaternion cQuaternion;
      /* pop the operand from the stack */
      FromLuaState(pt_state, 1, cQuaternion);
      /* push the result onto the stack */
      ToLuaState(pt_state, cQuaternion.Inverse());
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::ToAngleAxis(lua_State* pt_state) {
      CQuaternion cQuaternion;
      CRadians cAngle;
      CVector3 cAxis;
      /* pop the operand from the stack */
      FromLuaState(pt_state, 1, cQuaternion);
      /* apply operation */
      cQuaternion.ToAngleAxis(cAngle, cAxis);
      /* push the results onto the stack */
      lua_pushnumber(pt_state, cAngle.GetValue());
      CLuaVector3::ToLuaState(pt_state, cAxis);
      return 2;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::ToString(lua_State* pt_state) {
      CQuaternion cQuaternion;
      /* pop the operand from the stack */
      FromLuaState(pt_state, 1, cQuaternion);
      /* convert to string */
      std::ostringstream ossOutput;
      ossOutput << cQuaternion;
      /* push the string onto the stack */
      lua_pushstring(pt_state, ossOutput.str().c_str());
      return 1;
   }

   /****************************************/
   /****************************************/

}
