/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_lift_system_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "ci_builderbot_lift_system_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have one value:
    * 1. position (a number): the position to which to move the lift end effector
    */
   int LuaSetBuilderBotLiftSystemActuatorPosition(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.lift_system.set_position() expects 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_BuilderBotLiftSystemActuator>(pt_lua_state, "lift_system")->
         SetPosition(lua_tonumber(pt_lua_state, 1));
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaStopBuilderBotLiftSystemActuator(lua_State* pt_lua_state) {
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_BuilderBotLiftSystemActuator>(pt_lua_state, "lift_system")->Stop();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaCalibrateBuilderBotLiftSystemActuator(lua_State* pt_lua_state) {
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_BuilderBotLiftSystemActuator>(pt_lua_state, "lift_system")->Calibrate();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/
#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotLiftSystemActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "lift_system");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_position",
                              &LuaSetBuilderBotLiftSystemActuatorPosition);
      CLuaUtility::AddToTable(pt_lua_state,
                              "stop",
                              &LuaStopBuilderBotLiftSystemActuator);
      CLuaUtility::AddToTable(pt_lua_state,
                              "calibrate",
                              &LuaCalibrateBuilderBotLiftSystemActuator);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
