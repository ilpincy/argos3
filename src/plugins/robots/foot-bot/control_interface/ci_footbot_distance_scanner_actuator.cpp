/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_distance_scanner_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_footbot_distance_scanner_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaDistanceScannerEnable(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 0) {
         return luaL_error(pt_lua_state, "robot.distance_scanner.enable() expects no arguments");
      }
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_FootBotDistanceScannerActuator>(pt_lua_state, "distance_scanner")->Enable();
      return 0;
   }

   int LuaDistanceScannerDisable(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 0) {
         return luaL_error(pt_lua_state, "robot.distance_scanner.disable() expects no arguments");
      }
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_FootBotDistanceScannerActuator>(pt_lua_state, "distance_scanner")->Disable();
      return 0;
   }

   int LuaDistanceScannerSetAngle(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.distance_scanner.set_angle() expects 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_FootBotDistanceScannerActuator>(pt_lua_state, "distance_scanner")->SetAngle(CRadians(lua_tonumber(pt_lua_state, 1)));
      return 0;
   }

   int LuaDistanceScannerSetRPM(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.distance_scanner.set_rpm() expects 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_FootBotDistanceScannerActuator>(pt_lua_state, "distance_scanner")->SetRPM(lua_tonumber(pt_lua_state, 1));
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_FootBotDistanceScannerActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable (pt_lua_state, "distance_scanner"                            );
      CLuaUtility::AddToTable          (pt_lua_state, "_instance",       this                       );
      CLuaUtility::AddToTable          (pt_lua_state, "enable",          &LuaDistanceScannerEnable  );
      CLuaUtility::AddToTable          (pt_lua_state, "disable",         &LuaDistanceScannerDisable );
      CLuaUtility::AddToTable          (pt_lua_state, "set_angle",       &LuaDistanceScannerSetAngle);
      CLuaUtility::AddToTable          (pt_lua_state, "set_rpm",         &LuaDistanceScannerSetRPM  );
      CLuaUtility::CloseRobotStateTable(pt_lua_state                                                );
   }
#endif

   /****************************************/
   /****************************************/

}
