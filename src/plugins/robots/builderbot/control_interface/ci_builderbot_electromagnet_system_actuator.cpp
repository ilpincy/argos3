/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_electromagnet_system_actuator.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_builderbot_electromagnet_system_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have one value:
    * 1. discharge mode (a string): disable, constructive, destructive
    */
   int LuaSetBuilderBotElectromagnetSystemActuatorDischargeMode(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.electromagnet_system.set_discharge_mode() expects 1 argument");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TSTRING);
      std::string strDischargeMode(lua_tostring(pt_lua_state, 1));

      /* By default disable charging */
      CCI_BuilderBotElectromagnetSystemActuator::EDischargeMode eDischargeMode =
          CCI_BuilderBotElectromagnetSystemActuator::EDischargeMode::DISABLED;
      /* Check if the charge mode is constructive or destructive */
      if(strDischargeMode == "constructive") {
         eDischargeMode = CCI_BuilderBotElectromagnetSystemActuator::EDischargeMode::CONSTRUCTIVE;
      }
      else if(strDischargeMode == "destructive") {
         eDischargeMode = CCI_BuilderBotElectromagnetSystemActuator::EDischargeMode::DESTRUCTIVE;
      }
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_BuilderBotElectromagnetSystemActuator>(pt_lua_state, "electromagnet_system")->
         SetDischargeMode(eDischargeMode);
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotElectromagnetSystemActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "electromagnet_system");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state,
                              "set_discharge_mode",
                              &LuaSetBuilderBotElectromagnetSystemActuatorDischargeMode);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif


   /****************************************/
   /****************************************/

}
