/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_propellers_position_actuator.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "ci_propellers_position_actuator.h"

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
         return luaL_error(pt_lua_state, "robot.propellers.set_position() expects 3 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 3, LUA_TNUMBER);
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_PropellersPositionActuator>(pt_lua_state, "propellers")->
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
    * The stack must have three values in this order:
    * 1. yaw angle (in radians)
    * 2. pitch angle (in radians)
    * 3. roll angle (in radians)
    */
   int LuaSetOrientation(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 3) {
         return luaL_error(pt_lua_state, "robot.propellers.set_orientation() expects 3 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 3, LUA_TNUMBER);
      /* Perform action */
      CQuaternion cQuat;
      cQuat.FromEulerAngles(CRadians(lua_tonumber(pt_lua_state, 1)),
                            CRadians(lua_tonumber(pt_lua_state, 2)),
                            CRadians(lua_tonumber(pt_lua_state, 3)));
      CLuaUtility::GetDeviceInstance<CCI_PropellersPositionActuator>(pt_lua_state, "propellers")->
         SetOrientation(cQuat);
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PropellersPositionActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "propellers");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "set_position", &LuaSetPosition);
      CLuaUtility::AddToTable(pt_lua_state, "set_orientation", &LuaSetOrientation);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

}
