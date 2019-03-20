/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_tags_actuator.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_tags_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * This function expects the stack to have two arguments. The first argument 
    * must always be the index of the tag to set. The second argument is a
    * string that represents the payload of the tag.
    */
   int LuaTagSetSinglePayload(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 2) {
         return luaL_error(pt_lua_state, "robot.tags.set_single_payload() expects 2 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      size_t unIdx = lua_tonumber(pt_lua_state, 1);
      /* Get reference to actuator */
      CCI_TagsActuator* pcTagActuator = 
         CLuaUtility::GetDeviceInstance<CCI_TagsActuator>(pt_lua_state, "tags");
      if(unIdx < 1 || unIdx > pcTagActuator->GetNumTags()) {
         return luaL_error(pt_lua_state,
                           "passed index %d out of bounds [1,%d]", 
                           unIdx, pcTagActuator->GetNumTags());
      }
      luaL_checktype(pt_lua_state, 2, LUA_TSTRING);
      /* Perform action */
      pcTagActuator->SetSinglePayload(unIdx - 1, lua_tostring(pt_lua_state, 2));
      return 0;
   }

   /*
    * This function expects the stack to have one arguments, a string
    * that represents the payload of the tag
    */
   int LuaTagSetAllPayloads(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.tags.set_all_payloads() expects 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TSTRING);
      /* Get reference to actuator */
      CCI_TagsActuator* pcTagActuator = 
         CLuaUtility::GetDeviceInstance<CCI_TagsActuator>(pt_lua_state, "tags");
      /* Perform action */
      pcTagActuator->SetAllPayloads(lua_tostring(pt_lua_state, 1));
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

   size_t CCI_TagsActuator::GetNumTags() const {
     return m_tSettings.size();
   }

   /****************************************/
   /****************************************/

   void CCI_TagsActuator::SetSinglePayload(UInt32 un_tag_number,
                                         const std::string& str_payload) {
      m_tSettings[un_tag_number] = str_payload;
   }
      
   /****************************************/
   /****************************************/

   void CCI_TagsActuator::SetAllPayloads(const std::string& str_payload) {
      for(size_t i = 0; i < m_tSettings.size(); ++i) {
         m_tSettings[i] = str_payload;
      }
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_TagsActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "tags");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "set_single_payload", &LuaTagSetSinglePayload);
      CLuaUtility::AddToTable(pt_lua_state, "set_all_payloads", &LuaTagSetAllPayloads);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

}
