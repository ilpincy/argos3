/**
 * @file <argos3/core/wrappers/lua/lua_vector3.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "lua_vector3.h"

#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

#include <argos3/core/wrappers/lua/lua_quaternion.h>
#include <argos3/core/wrappers/lua/lua_utility.h>

namespace argos {

   /****************************************/
   /****************************************/

   const std::string CLuaVector3::m_strTypeId("argos3.vector3");

   /****************************************/
   /****************************************/

   void CLuaVector3::RegisterType(lua_State* pt_state) {
      /* create a constructor as a global function */
      lua_pushcfunction(pt_state, Create);
      lua_setglobal(pt_state, "vector3");
      /* create a metatable for vector3s */
      luaL_newmetatable(pt_state, m_strTypeId.c_str());
      /* register metamethods */
      CLuaUtility::AddToTable(pt_state, "__index", Index);
      CLuaUtility::AddToTable(pt_state, "__newindex", NewIndex);
      CLuaUtility::AddToTable(pt_state, "__tostring", ToString);
      CLuaUtility::AddToTable(pt_state, "__eq", Equal);
      CLuaUtility::AddToTable(pt_state, "__add", Add);
      CLuaUtility::AddToTable(pt_state, "__mul", Multiply);
      CLuaUtility::AddToTable(pt_state, "__sub", Subtract);
      CLuaUtility::AddToTable(pt_state, "__unm", UnaryMinus);
      CLuaUtility::AddToTable(pt_state, "normalize", Normalize);
      CLuaUtility::AddToTable(pt_state, "length", Length);
      CLuaUtility::AddToTable(pt_state, "dot", DotProduct);
      CLuaUtility::AddToTable(pt_state, "cross", CrossProduct);
      CLuaUtility::AddToTable(pt_state, "rotate", Rotate);
   }

   /****************************************/
   /****************************************/

   int CLuaVector3::Create(lua_State* pt_state) {
      /* parse arguments */
      switch(lua_gettop(pt_state)) {
      case 0:
         /* default constructor */
         PushVector3(pt_state);
         break;
      case 1:
         /* copy constructor */
         PushVector3(pt_state, ToVector3(pt_state, 1));
         break;
      case 3:
         /* value constructor */
         if(lua_isnumber(pt_state, 1) &&
            lua_isnumber(pt_state, 2) &&
            lua_isnumber(pt_state, 3)) {
            PushVector3(pt_state,
                        lua_tonumber(pt_state, 1),
                        lua_tonumber(pt_state, 2),
                        lua_tonumber(pt_state, 3));
         }
         else {
            lua_pushstring(pt_state, "invalid arguments to vector3");
            lua_error(pt_state);
         }
         break;
      default:
         lua_pushstring(pt_state, "invalid arguments to vector3");
         lua_error(pt_state);
         break;
      }
      return 1;
   }

   /****************************************/
   /****************************************/

   CVector3& CLuaVector3::ToVector3(lua_State* pt_state,
                                    int n_index) {
      /* check type */
      void* pvUserdatum =
         luaL_checkudata(pt_state, n_index, m_strTypeId.c_str());
      /* raise error if required */
      if(pvUserdatum == nullptr) {
         lua_pushstring(pt_state, "vector3 not found at requested index");
         lua_error(pt_state);
      }
      /* return vector3 */
      CVector3* pcVector3 = static_cast<CVector3*>(pvUserdatum);
      return *pcVector3;
   }

   /****************************************/
   /****************************************/

   int CLuaVector3::Index(lua_State* pt_state) {
      if(lua_isuserdata(pt_state, 1) &&
         lua_isstring(pt_state, 2)) {
         size_t unLength = 0;
         const char* pchKey = lua_tolstring(pt_state, 2, &unLength);
         if(unLength == 1) {
            CVector3& cVector = ToVector3(pt_state, 1);
            switch(pchKey[0]) {
            case 'x':
               lua_pushnumber(pt_state, cVector.GetX());
               break;
            case 'y':
               lua_pushnumber(pt_state, cVector.GetY());
               break;
            case 'z':
               lua_pushnumber(pt_state, cVector.GetZ());
               break;
            default:
               lua_pushstring(pt_state, "invalid key for vector3");
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
      lua_pushstring(pt_state, "invalid operation on vector3");
      lua_error(pt_state);
      return 0;
   }

   /****************************************/
   /****************************************/

   int CLuaVector3::NewIndex(lua_State* pt_state) {
      if(lua_isuserdata(pt_state, 1) &&
         lua_isstring(pt_state, 2) &&
         lua_isnumber(pt_state, 3)) {
         size_t unLength = 0;
         const char* pchKey = lua_tolstring(pt_state, 2, &unLength);
         if(unLength == 1) {
            CVector3& cVector = ToVector3(pt_state, 1);
            switch(pchKey[0]) {
            case 'x':
               cVector.SetX(lua_tonumber(pt_state, 3));
               break;
            case 'y':
               cVector.SetY(lua_tonumber(pt_state, 3));
               break;
            case 'z':
               cVector.SetZ(lua_tonumber(pt_state, 3));
               break;
            default:
               lua_pushstring(pt_state, "invalid key for vector3");
               lua_error(pt_state);
            }
            return 0;
         }
      }
      lua_pushstring(pt_state, "invalid operation on vector3");
      lua_error(pt_state);
      return 0;
   }

   /****************************************/
   /****************************************/

   int CLuaVector3::Equal(lua_State* pt_state) {
      bool bEqual = 
         (ToVector3(pt_state, 1) == ToVector3(pt_state, 2));
      /* push the result onto the stack and return it */
      lua_pushboolean(pt_state, bEqual);
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaVector3::Add(lua_State* pt_state) {
      /* perform the addition */
      PushVector3(pt_state,
                  ToVector3(pt_state, 1) +
                  ToVector3(pt_state, 2));
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaVector3::Multiply(lua_State* pt_state) {
      if(lua_isuserdata(pt_state, 1) && lua_isnumber(pt_state, 2)) {
         CVector3& cVector = ToVector3(pt_state, 1);
         PushVector3(pt_state, cVector * lua_tonumber(pt_state, 2));
      }
      else if(lua_isuserdata(pt_state, 2) && lua_isnumber(pt_state, 1)) {
         CVector3& cVector = ToVector3(pt_state, 2);
         PushVector3(pt_state, cVector * lua_tonumber(pt_state, 1));
      }
      else {
         lua_pushstring(pt_state, "invalid arguments for multiplication by scalar");
         lua_error(pt_state);
      }
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaVector3::Subtract(lua_State* pt_state) {
      /* perform the subtraction */
      PushVector3(pt_state,
                  ToVector3(pt_state, 1) -
                  ToVector3(pt_state, 2));
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaVector3::UnaryMinus(lua_State* pt_state) {
      /* perform the negation */
      PushVector3(pt_state, -ToVector3(pt_state, 1));
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaVector3::Normalize(lua_State* pt_state) {
      ToVector3(pt_state, 1).Normalize();
      /* result is already on the stack, return it */
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaVector3::Length(lua_State* pt_state) {
      /* push the result onto the stack and return it */
      lua_pushnumber(pt_state, ToVector3(pt_state, 1).Length());
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaVector3::CrossProduct(lua_State* pt_state) {
      ToVector3(pt_state, 1).CrossProduct(ToVector3(pt_state, 2));     
      /* pop the second operand from the stack */
      lua_pop(pt_state, 1);
      /* return the first operand (modified) */
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaVector3::DotProduct(lua_State* pt_state) {
      /* calculate the dot product */
      Real fDotProduct =
         ToVector3(pt_state, 1).DotProduct(ToVector3(pt_state, 2));
      /* push the result onto the stack and return it */
      lua_pushnumber(pt_state, fDotProduct);
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaVector3::Rotate(lua_State* pt_state) {
      CVector3& cVector =
         ToVector3(pt_state, 1);     
      CQuaternion& cQuaternion =
         CLuaQuaternion::ToQuaternion(pt_state, 2);
      /* rotate the first operand (the vector) by the second 
         operand (the quaternion) */
      cVector.Rotate(cQuaternion);
      /* pop the second operand (the quaternion) off the stack */
      lua_pop(pt_state, 1);
      /* return the first operand (the rotated vector) */
      return 1;
   }

   /****************************************/
   /****************************************/

   int CLuaVector3::ToString(lua_State* pt_state) {
      /* get a reference to the operand from the stack */
      const CVector3& cVector = ToVector3(pt_state, 1);
      /* convert it to a string */
      std::ostringstream ossOutput;
      ossOutput << cVector;
      /* push the string onto the stack and return it */
      lua_pushstring(pt_state, ossOutput.str().c_str());
      return 1;
   }

   /****************************************/
   /****************************************/

}
