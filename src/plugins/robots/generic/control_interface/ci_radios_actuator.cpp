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
   int LuaRadioActuatorSend(lua_State* pt_lua_state) {
      /* get a reference to the interface */     
      CCI_RadiosActuator::SInterface* ps_interface = 
         reinterpret_cast<CCI_RadiosActuator::SInterface*>(lua_touserdata(pt_lua_state, lua_upvalueindex(1)));
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         std::string strErrMsg = "robot.radios." + ps_interface->Id + ".send() requires 1 argument";
         return luaL_error(pt_lua_state, strErrMsg.c_str());
      }
      luaL_checktype(pt_lua_state, 1, LUA_TTABLE);
      /* serialize the table into a CByteArray */
      CByteArray cMessage;
      CLuaUtility::LuaSerializeTable(cMessage, pt_lua_state, 1);
      ps_interface->Messages.emplace_back(cMessage);
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
         /* push a pointer to each interface onto the lua stack */
         lua_pushstring(pt_lua_state, "send");
         lua_pushlightuserdata(pt_lua_state, &s_interface);
         lua_pushcclosure(pt_lua_state, &LuaRadioActuatorSend, 1);
         lua_settable(pt_lua_state, -3);
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif


}
