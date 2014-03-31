/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_quadrotor_position_actuator.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "ci_quadrotor_position_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have three values in this order:
    * 1. x coordinate (a number)
    * 2. y coordinate (a number)
    * 3. z coordinate (a number)
    */
   int LuaSetPosition(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 3) {
         return luaL_error(pt_lua_state, "robot.quadrotor.set_position() expects 3 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 3, LUA_TNUMBER);
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_QuadRotorPositionActuator>(pt_lua_state, "quadrotor")->
         SetPosition(CVector3(lua_tonumber(pt_lua_state, 1),
                              lua_tonumber(pt_lua_state, 2),
                              lua_tonumber(pt_lua_state, 3)));
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have one value:
    * 1. yaw angle (in radians)
    */
   int LuaSetYaw(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.quadrotor.set_yaw() expects 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_QuadRotorPositionActuator>(pt_lua_state, "quadrotor")->
         SetYaw(CRadians(lua_tonumber(pt_lua_state, 1) / 180.0f * CRadians::PI));
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_QuadRotorPositionActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "quadrotor");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "set_position", &LuaSetPosition);
      CLuaUtility::AddToTable(pt_lua_state, "set_yaw", &LuaSetYaw);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

}
