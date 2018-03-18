/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_radios_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "ci_radios_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   CCI_RadiosActuator::SInterface::TVector& CCI_RadiosActuator::GetInterfaces() {
      return m_vecInterfaces;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaSendData(lua_State* pt_lua_state) {
      /* get a reference to the actuator instance */     
      CCI_RadiosActuator::SInterface* ps_interface = 
         reinterpret_cast<CCI_RadiosActuator::SInterface*>(lua_touserdata(pt_lua_state, lua_upvalueindex(1)));
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         std::string strErrMsg = "robot.radios." + ps_interface->Id + ".tx_data() requires 1 argument";
         return luaL_error(pt_lua_state, strErrMsg.c_str());
      }
      luaL_checktype(pt_lua_state, 1, LUA_TTABLE);
      /* copy the specified data into a CByteArray */
      CByteArray cBuffer(lua_rawlen(pt_lua_state, -1));
      for(size_t i = 0; i < cBuffer.Size(); ++i) {
         lua_pushnumber(pt_lua_state, i + 1);
         lua_gettable(pt_lua_state, -2);
         if(lua_type(pt_lua_state, -1) == LUA_TNUMBER) {
            cBuffer[i] = static_cast<UInt8>(lua_tonumber(pt_lua_state, -1));
            lua_pop(pt_lua_state, 1);
         }
         else {
            return luaL_error(pt_lua_state, "element #%d of the table is not a number", i + 1);
         }
      }
      ps_interface->Data.emplace_back(cBuffer);
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_RadiosActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "radios");
      for(SInterface& s_interface : m_vecInterfaces) {
         CLuaUtility::StartTable(pt_lua_state, s_interface.Id);
         /* push a pointer to each actuator instance onto the lua stack */
         lua_pushstring(pt_lua_state, "tx_data");
         lua_pushlightuserdata(pt_lua_state, &s_interface);
         lua_pushcclosure(pt_lua_state, &LuaSendData, 1);
         lua_settable(pt_lua_state, -3);
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif


}
