/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_leds_actuator.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_pipuck_leds_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have two values in this order:
    * 1. led index (a number)
    * 2. configuration on/off (a boolean)
    */
   int LuaSetPiPuckLEDsActuatorRingLEDIndex(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 2) {
         return luaL_error(pt_lua_state, "robot.leds.set_ring_led_index() expects 2 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 2, LUA_TBOOLEAN);
      /* Get actuator instance */
      CCI_PiPuckLEDsActuator* pcLEDsActuator =
         CLuaUtility::GetDeviceInstance<CCI_PiPuckLEDsActuator>(pt_lua_state, "leds");
      /* Update actuator */
      UInt32 unIndex = lua_tonumber(pt_lua_state, 1) - 1;
      bool bOn = lua_toboolean(pt_lua_state, 2);
      pcLEDsActuator->SetRingLEDIndex(unIndex, bOn);
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have a single value:
    * 1. configuration on/off (a boolean)
    */
   int LuaSetPiPuckLEDsActuatorRingLEDs(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.leds.set_ring_leds() expects 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TBOOLEAN);
      /* Get actuator instance */
      CCI_PiPuckLEDsActuator* pcLEDsActuator =
         CLuaUtility::GetDeviceInstance<CCI_PiPuckLEDsActuator>(pt_lua_state, "leds");
      /* Update actuator */
      pcLEDsActuator->SetRingLEDs(lua_toboolean(pt_lua_state, 1));
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have a single value:
    * 1. configuration on/off (a boolean)
    */
   int LuaSetPiPuckLEDsActuatorFrontLED(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.leds.set_front_led() expects 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TBOOLEAN);
      /* Get actuator instance */
      CCI_PiPuckLEDsActuator* pcLEDsActuator =
         CLuaUtility::GetDeviceInstance<CCI_PiPuckLEDsActuator>(pt_lua_state, "leds");
      /* Update actuator */
      pcLEDsActuator->SetFrontLED(lua_toboolean(pt_lua_state, 1));
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have a single value:
    * 1. configuration on/off (a boolean)
    */
   int LuaSetPiPuckLEDsActuatorBodyLED(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.leds.set_body_led() expects 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TBOOLEAN);
      /* Get actuator instance */
      CCI_PiPuckLEDsActuator* pcLEDsActuator =
         CLuaUtility::GetDeviceInstance<CCI_PiPuckLEDsActuator>(pt_lua_state, "leds");
      /* Update actuator */
      pcLEDsActuator->SetBodyLED(lua_toboolean(pt_lua_state, 1));
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/
#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckLEDsActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "leds");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_ring_led_index",
                              &LuaSetPiPuckLEDsActuatorRingLEDIndex);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_ring_leds",
                              &LuaSetPiPuckLEDsActuatorRingLEDs);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_front_led",
                              &LuaSetPiPuckLEDsActuatorFrontLED);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_body_led",
                              &LuaSetPiPuckLEDsActuatorBodyLED);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
