/**
 * @file <argos3/plugins/robots/prototype/control_interface/ci_robot_joints_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "ci_prototype_joints_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaSetTarget(lua_State* pt_lua_state) {
      /* get a reference to the actuator */     
      CCI_PrototypeJointsActuator::SActuator* pc_actuator = 
         reinterpret_cast<CCI_PrototypeJointsActuator::SActuator*>(lua_touserdata(pt_lua_state, lua_upvalueindex(1)));
      /* check number of arguments */
      if(lua_gettop(pt_lua_state) != 1) {
         std::string strErrMsg = "robot.joints." + pc_actuator->Id + ".set_target() requires 1 argument";
         return luaL_error(pt_lua_state, strErrMsg.c_str());
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      /* Perform action */
      pc_actuator->Target = lua_tonumber(pt_lua_state, 1);
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PrototypeJointsActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "joints");
      for(SActuator* pc_actuator : m_vecActuators) {
         CLuaUtility::StartTable(pt_lua_state, pc_actuator->Id);
         /* push a pointer to each actuator onto the lua stack */
         lua_pushstring(pt_lua_state, "set_target");
         lua_pushlightuserdata(pt_lua_state, pc_actuator);
         lua_pushcclosure(pt_lua_state, &LuaSetTarget, 1);
         lua_settable(pt_lua_state, -3);
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif


}
