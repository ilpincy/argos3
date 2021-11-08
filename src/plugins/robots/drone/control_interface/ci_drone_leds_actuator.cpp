/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_leds_actuator.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_drone_leds_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

#include <argos3/core/utility/datatypes/color.h>

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have two values in this order:
    * 1. led index (a number)
    * 2. configuration on/off (a boolean)
    */
   int LuaSetDroneLEDsActuatorLEDIndex(lua_State* pt_lua_state) {
      /* Get actuator instance */
      CCI_DroneLEDsActuator* pcLEDsActuator =
         CLuaUtility::GetDeviceInstance<CCI_DroneLEDsActuator>(pt_lua_state, "leds");
      /* Check parameters */
      SInt32 nArgs = lua_gettop(pt_lua_state);
      if(nArgs != 2 && nArgs != 4) {
         return luaL_error(pt_lua_state, "robot.leds.set_led_index() expects 2 or 4 arguments");
      }
      /* Get index */
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      UInt32 unIndex = lua_tonumber(pt_lua_state, 1) - 1;
      /* Get color */
      CColor cColor;
      if(nArgs == 2) {
         luaL_checktype(pt_lua_state, 2, LUA_TSTRING);
         try {
            cColor.Set(lua_tostring(pt_lua_state, 2));
         }
         catch(CARGoSException& ex) {
            return luaL_error(pt_lua_state, ex.what());
         }
      }
      else {
         luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 3, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 4, LUA_TNUMBER);
         cColor.Set(lua_tonumber(pt_lua_state, 2),
                    lua_tonumber(pt_lua_state, 3),
                    lua_tonumber(pt_lua_state, 4));
      }
      /* Update actuator */
      pcLEDsActuator->SetLEDIndex(unIndex, cColor);
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaSetDroneLEDsActuatorLEDs(lua_State* pt_lua_state) {
      /* Get actuator instance */
      CCI_DroneLEDsActuator* pcLEDsActuator =
         CLuaUtility::GetDeviceInstance<CCI_DroneLEDsActuator>(pt_lua_state, "leds");
      /* Check parameters */
      SInt32 nArgs = lua_gettop(pt_lua_state);
      if(nArgs != 1 && nArgs != 3) {
         return luaL_error(pt_lua_state, "robot.leds.set_leds() expects 1 or 3 arguments");
      }
      /* Get color */
      CColor cColor;
      if(nArgs == 1) {
         luaL_checktype(pt_lua_state, 1, LUA_TSTRING);
         try {
            cColor.Set(lua_tostring(pt_lua_state, 1));
         }
         catch(CARGoSException& ex) {
            return luaL_error(pt_lua_state, ex.what());
         }
      }
      else {
         luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 3, LUA_TNUMBER);
         cColor.Set(lua_tonumber(pt_lua_state, 1),
                    lua_tonumber(pt_lua_state, 2),
                    lua_tonumber(pt_lua_state, 3));
      }
      /* Update actuator */
      pcLEDsActuator->SetLEDs(cColor);
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DroneLEDsActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "leds");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_led_index",
                              &LuaSetDroneLEDsActuatorLEDIndex);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_leds",
                              &LuaSetDroneLEDsActuatorLEDs);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
