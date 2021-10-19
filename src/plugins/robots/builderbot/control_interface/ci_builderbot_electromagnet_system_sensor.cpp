/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_electromagnet_system_sensor.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_builderbot_electromagnet_system_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   Real CCI_BuilderBotElectromagnetSystemSensor::GetVoltage() const {
      return m_fVoltage;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotElectromagnetSystemSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "electromagnet_system");
      CLuaUtility::AddToTable(pt_lua_state, "voltage", m_fVoltage);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotElectromagnetSystemSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "electromagnet_system");
      CLuaUtility::AddToTable(pt_lua_state, "voltage", m_fVoltage);
      lua_pop(pt_lua_state, 1);
   }
#endif


   /****************************************/
   /****************************************/

}
