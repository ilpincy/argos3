/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_flight_system_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Sinan Oguz - <soguz.ankara@gmail.com>
 */

#include "ci_drone_flight_system_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#include <argos3/core/wrappers/lua/lua_vector3.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have two values in this order:
    * 1. arm (true) or disarm (false) (boolean)
    * 2. bypass safety checks (boolean)
    */
   int LuaSetDroneFlightSystemArmed(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 2) {
         return luaL_error(pt_lua_state, "robot.flight_system.set_armed() expects 2 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TBOOLEAN);
      luaL_checktype(pt_lua_state, 2, LUA_TBOOLEAN);
      /* Get actuator instance */
      CCI_DroneFlightSystemActuator* pcFlightSystemActuator =
         CLuaUtility::GetDeviceInstance<CCI_DroneFlightSystemActuator>(pt_lua_state, "flight_system");
      /* Update actuator */
      pcFlightSystemActuator->Arm(lua_toboolean(pt_lua_state, 1), lua_toboolean(pt_lua_state, 2));
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have one value:
    * 1. off-board mode enabled (true) or disabled (false) (boolean)
    */
   int LuaSetDroneFlightSystemOffboardMode(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.flight_system.set_offboard_mode() expects 1 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TBOOLEAN);
      /* Get actuator instance */
      CCI_DroneFlightSystemActuator* pcFlightSystemActuator =
         CLuaUtility::GetDeviceInstance<CCI_DroneFlightSystemActuator>(pt_lua_state, "flight_system");
      /* Update actuator */
      pcFlightSystemActuator->SetOffboardMode(lua_toboolean(pt_lua_state, 1));
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have two values in this order:
    * 1. target position (vector3)
    * 2. target yaw angle (number)
    */
   int LuaSetDroneFlightSystemTargetPose(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 2) {
         return luaL_error(pt_lua_state, "robot.flight_system.set_target_pose() expects 2 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TUSERDATA);
      luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
      /* Get actuator instance */
      CCI_DroneFlightSystemActuator* pcFlightSystemActuator =
         CLuaUtility::GetDeviceInstance<CCI_DroneFlightSystemActuator>(pt_lua_state, "flight_system");
      /* Update actuator */
      const CVector3& cTargetPosition = CLuaVector3::ToVector3(pt_lua_state, 1);
      pcFlightSystemActuator->SetTargetPosition(cTargetPosition);
      pcFlightSystemActuator->SetTargetYawAngle(lua_tonumber(pt_lua_state, 2));
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have zero values
    */
   int LuaIsDroneFlightSystemReady(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 0) {
         return luaL_error(pt_lua_state, "robot.flight_system.ready() expects 0 arguments");
      }
      /* Get actuator instance */
      CCI_DroneFlightSystemActuator* pcFlightSystemActuator =
         CLuaUtility::GetDeviceInstance<CCI_DroneFlightSystemActuator>(pt_lua_state, "flight_system");
      /* Read actuator */
      lua_pushboolean(pt_lua_state, pcFlightSystemActuator->Ready());
      return 1;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DroneFlightSystemActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "flight_system");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state,
                              "ready",
                              &LuaIsDroneFlightSystemReady);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_armed",
                              &LuaSetDroneFlightSystemArmed);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_offboard_mode",
                              &LuaSetDroneFlightSystemOffboardMode);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_target_pose",
                              &LuaSetDroneFlightSystemTargetPose);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

}
