/**
 * @file <argos3/core/wrappers/lua/lua_quaternion.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "lua_quaternion.h"

#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

#include <argos3/core/wrappers/lua/lua_vector3.h>
#include <argos3/core/wrappers/lua/lua_utility.h>

namespace argos {

   /****************************************/
   /****************************************/

   const std::string CLuaQuaternion::m_strTypeId("argos3.quaternion");

   /****************************************/
   /****************************************/

   void CLuaQuaternion::RegisterType(lua_State* pt_state) {
      /* create a constructor as a global function */
      lua_pushcfunction(pt_state, Create);
      lua_setglobal(pt_state, "quaternion");
      /* create a metatable for quaternions */
      luaL_newmetatable(pt_state, m_strTypeId.c_str());
      /* register metamethods */
      CLuaUtility::AddToTable(pt_state, "__index", Index);
      CLuaUtility::AddToTable(pt_state, "__newindex", NewIndex);
      CLuaUtility::AddToTable(pt_state, "__tostring", ToString);
      CLuaUtility::AddToTable(pt_state, "__eq", Equal);
      CLuaUtility::AddToTable(pt_state, "__mul", Multiply);
      CLuaUtility::AddToTable(pt_state, "normalize", Normalize);
      CLuaUtility::AddToTable(pt_state, "toangleaxis", ToAngleAxis);
      CLuaUtility::AddToTable(pt_state, "toeulerangles", ToEulerAngles);
      CLuaUtility::AddToTable(pt_state, "inverse", Inverse);
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::Create(lua_State* pt_state) {
      /* parse arguments */
      switch(lua_gettop(pt_state)) {
      case 0:
         PushQuaternion(pt_state);
         /* default values */
         break;
      case 1:
         /* copy constructor */
         PushQuaternion(pt_state, ToQuaternion(pt_state, 1));
         break;
      case 2:
         /* angle-axis constructor */
         if(lua_isnumber(pt_state, 1)) {
            CRadians cAngle(lua_tonumber(pt_state, 1));
            const CVector3& cAxis = CLuaVector3::ToVector3(pt_state, 2);
            PushQuaternion(pt_state, cAngle, cAxis);
         }
         else {
            lua_pushstring(pt_state, "invalid arguments to quaternion");
            lua_error(pt_state);
         }
         break;
      case 4:
         /* value constructor */
         if(lua_isnumber(pt_state, 1) &&
            lua_isnumber(pt_state, 2) &&
            lua_isnumber(pt_state, 3) &&
            lua_isnumber(pt_state, 4)) {
            PushQuaternion(pt_state,
                           lua_tonumber(pt_state, 1),
                           lua_tonumber(pt_state, 2),
                           lua_tonumber(pt_state, 3),
                           lua_tonumber(pt_state, 4));
         }
         else {
            lua_pushstring(pt_state, "invalid arguments to quaternion");
            lua_error(pt_state);
         }
         break;
      default:
         lua_pushstring(pt_state, "invalid arguments to quaternion");
         lua_error(pt_state);
         break;
      }
      return 1;
   }

   /****************************************/
   /****************************************/

   CQuaternion& CLuaQuaternion::ToQuaternion(lua_State* pt_state,
                                             int n_index) {
      /* check type */
      void* pvUserdatum =
         luaL_checkudata(pt_state, n_index, m_strTypeId.c_str());
      /* raise error if required */
      if(pvUserdatum == nullptr) {
         lua_pushstring(pt_state, "quaternion not found at requested index");
         lua_error(pt_state);
      }
      /* return quaternion */
      CQuaternion* pcQuaternion = static_cast<CQuaternion*>(pvUserdatum);
      return *pcQuaternion;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::Index(lua_State* pt_state) {
      if(lua_isuserdata(pt_state, 1) &&
         lua_isstring(pt_state, 2)) {
         size_t unLength = 0;
         const char* pchKey = lua_tolstring(pt_state, 2, &unLength);
         if(unLength == 1) {
            CQuaternion& cQuaternion = ToQuaternion(pt_state, 1);
            switch(pchKey[0]) {
            case 'w':
               lua_pushnumber(pt_state, cQuaternion.GetW());
               break;
            case 'x':
               lua_pushnumber(pt_state, cQuaternion.GetX());
               break;
            case 'y':
               lua_pushnumber(pt_state, cQuaternion.GetY());
               break;
            case 'z':
               lua_pushnumber(pt_state, cQuaternion.GetZ());
               break;
            default:
               lua_pushstring(pt_state, "invalid key for quaternion");
               lua_error(pt_state);
            }
            return 1;
         }
         else {
            luaL_getmetatable(pt_state, m_strTypeId.c_str());
            lua_pushvalue(pt_state, 2);
            lua_gettable(pt_state, -2);
            return 1;
         }
      }
      lua_pushstring(pt_state, "invalid operation on quaternion");
      lua_error(pt_state);
      return 0;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::NewIndex(lua_State* pt_state) {
      if(lua_isuserdata(pt_state, 1) &&
         lua_isstring(pt_state, 2) &&
         lua_isnumber(pt_state, 3)) {
         size_t unLength = 0;
         const char* pchKey = lua_tolstring(pt_state, 2, &unLength);
         if(unLength == 1) {
            CQuaternion& cQuaternion = ToQuaternion(pt_state, 1);
            switch(pchKey[0]) {
            case 'w':
               cQuaternion.SetW(lua_tonumber(pt_state, 3));
               break;
            case 'x':
               cQuaternion.SetX(lua_tonumber(pt_state, 3));
               break;
            case 'y':
               cQuaternion.SetY(lua_tonumber(pt_state, 3));
               break;
            case 'z':
               cQuaternion.SetZ(lua_tonumber(pt_state, 3));
               break;
            default:
               lua_pushstring(pt_state, "invalid key for quaternion");
               lua_error(pt_state);
            }
            return 0;
         }
      }
      lua_pushstring(pt_state, "invalid operation on quaternion");
      lua_error(pt_state);
      return 0;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::Equal(lua_State* pt_state) {
      bool bEqual = 
         (ToQuaternion(pt_state, 1) == ToQuaternion(pt_state, 2));
      /* push the result onto the stack and return it */
      lua_pushboolean(pt_state, bEqual);
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::Multiply(lua_State* pt_state) {
      /* perform the multiplication */
      PushQuaternion(pt_state,
                     ToQuaternion(pt_state, 1) *
                     ToQuaternion(pt_state, 2));
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::Normalize(lua_State* pt_state) {
      ToQuaternion(pt_state, 1).Normalize();
      /* result is already on the stack, return it */
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::Inverse(lua_State* pt_state) {
      /* push the result onto the stack and return it */
      PushQuaternion(pt_state, ToQuaternion(pt_state, 1).Inverse());
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::ToAngleAxis(lua_State* pt_state) {
      CRadians cAngle;
      CVector3 cAxis;
      /* get a reference to the operand from the stack */
      const CQuaternion& cQuaternion = ToQuaternion(pt_state, 1);
      /* do conversion */
      cQuaternion.ToAngleAxis(cAngle, cAxis);
      /* push the results onto the stack and return them */
      lua_pushnumber(pt_state, cAngle.GetValue());
      CLuaVector3::PushVector3(pt_state, cAxis);
      return 2;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::ToEulerAngles(lua_State* pt_state) {
      CRadians cZ, cY, cX;
      /* get a reference to the operand from the stack */
      const CQuaternion& cQuaternion = ToQuaternion(pt_state, 1);
      /* do conversion */
      cQuaternion.ToEulerAngles(cZ, cY, cX);
      /* push the results onto the stack and return them */
      lua_pushnumber(pt_state, cZ.GetValue());
      lua_pushnumber(pt_state, cY.GetValue());
      lua_pushnumber(pt_state, cX.GetValue());
      return 3;
   }

   /****************************************/
   /****************************************/

   int CLuaQuaternion::ToString(lua_State* pt_state) {
      /* get a reference to the operand from the stack */
      const CQuaternion& cQuaternion = ToQuaternion(pt_state, 1);
      /* convert it to a string */
      std::ostringstream ossOutput;
      ossOutput << cQuaternion;
      /* push the string onto the stack and return it */
      lua_pushstring(pt_state, ossOutput.str().c_str());
      return 1;
   }

   /****************************************/
   /****************************************/

}
