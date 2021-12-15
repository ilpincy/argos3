/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_simple_radios_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "ci_simple_radios_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_SimpleRadiosSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "simple_radios"); // radios
      for(size_t i = 0; i < m_vecInterfaces.size(); ++i) {
         CLuaUtility::OpenRobotStateTable(pt_lua_state, m_vecInterfaces[i].Id); // interfaces
         CLuaUtility::StartTable(pt_lua_state, "recv");
         CLuaUtility::EndTable(pt_lua_state);
         CLuaUtility::CloseRobotStateTable(pt_lua_state); // interfaces
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state); // radios
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_SimpleRadiosSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "simple_radios"); // radios
      for(size_t i = 0; i < m_vecInterfaces.size(); i++) {
         const std::string& strId = m_vecInterfaces[i].Id;
         std::vector<CByteArray>& vecMessages = m_vecInterfaces[i].Messages;
         lua_getfield(pt_lua_state, -1, strId.c_str()); // interface
         lua_getfield(pt_lua_state, -1, "recv"); // messages
         size_t unLastMessageCount = lua_rawlen(pt_lua_state, -1);
         for(size_t j = 0; j < vecMessages.size(); ++j) {
            CLuaUtility::StartTable(pt_lua_state, j + 1);
            /* Copy CByteArray here since it consumes itself during deserialization */
            CByteArray cMessage(vecMessages[j]);
            CLuaUtility::LuaDeserializeTable(cMessage, pt_lua_state);
            CLuaUtility::EndTable(pt_lua_state);
         }
         if(vecMessages.size() < unLastMessageCount) {
            /* Remove the extra entries from the table */
            for(size_t j = vecMessages.size() + 1; j <= unLastMessageCount; ++j) {
               lua_pushnumber(pt_lua_state, j);
               lua_pushnil(pt_lua_state);
               lua_settable(pt_lua_state, -3);
            }
         }
         lua_pop(pt_lua_state, 1); // messages
         lua_pop(pt_lua_state, 1); // interface       
      }
      lua_pop(pt_lua_state, 1); // radios
   }
#endif

   /****************************************/
   /****************************************/

}
