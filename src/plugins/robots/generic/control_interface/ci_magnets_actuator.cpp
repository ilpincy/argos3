/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_magnets_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "ci_magnets_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   CCI_MagnetsActuator::SInterface::TVector& CCI_MagnetsActuator::GetInterfaces() {
      return m_vecInterfaces;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaSetCurrent(lua_State* pt_lua_state) {
      /* get a reference to the actuator */     
      CCI_MagnetsActuator::SInterface* ps_interface = 
         reinterpret_cast<CCI_MagnetsActuator::SInterface*>(lua_touserdata(pt_lua_state, lua_upvalueindex(1)));
      /* check number of arguments */
      if(lua_gettop(pt_lua_state) != 1) {
         std::string strErrMsg = "robot.magnets." + ps_interface->Id + ".set_current() requires 1 argument";
         return luaL_error(pt_lua_state, strErrMsg.c_str());
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      /* Perform action */
      ps_interface->Current = lua_tonumber(pt_lua_state, 1);
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_MagnetsActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "magnets");
      for(SInterface& s_interface : m_vecInterfaces) {
         CLuaUtility::StartTable(pt_lua_state, s_interface.Id);
         /* push a pointer to each actuator instance onto the lua stack */
         lua_pushstring(pt_lua_state, "set_current");
         lua_pushlightuserdata(pt_lua_state, &s_interface);
         lua_pushcclosure(pt_lua_state, &LuaSetCurrent, 1);
         lua_settable(pt_lua_state, -3);
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif


}
