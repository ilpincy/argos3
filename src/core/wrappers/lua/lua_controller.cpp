
/**
 * @file <argos3/core/wrappers/lua/lua_controller.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "lua_controller.h"
#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/core/wrappers/lua/lua_utility.h>

namespace argos {

   /****************************************/
   /****************************************/

   CLuaController::CLuaController() :
      m_ptLuaState(NULL) {
   }

   /****************************************/
   /****************************************/

   CLuaController::~CLuaController() {
   }

   /****************************************/
   /****************************************/

   void CLuaController::Init(TConfigurationNode& t_tree) {
      try {
         /* Create a new Lua stack */
         m_ptLuaState = luaL_newstate();
         /* Load the Lua libraries */
         luaL_openlibs(m_ptLuaState);
         /* Load script */
         std::string strScriptFileName;
         GetNodeAttribute(t_tree, "script", strScriptFileName);
         CLuaUtility::LoadScript(m_ptLuaState, strScriptFileName);
         /* Create and set variables */
         CreateLuaVariables();
         SensorReadingsToLuaVariables();
         /* Execute script init function */
         CLuaUtility::CallFunction(m_ptLuaState, "init");
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing Lua controller", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CLuaController::ControlStep() {
      /* Update Lua variables through sensor readings */
      SensorReadingsToLuaVariables();
      /* Execute script step function */
      CLuaUtility::CallFunction(m_ptLuaState, "step");
      /* Set actuator variables */
      LuaVariablesToActuatorSettings();
      CLuaUtility::PrintGlobals(LOGERR, m_ptLuaState);
   }

   /****************************************/
   /****************************************/

   void CLuaController::Reset() {
      /* Execute script reset function */
      CLuaUtility::CallFunction(m_ptLuaState, "reset");
   }

   /****************************************/
   /****************************************/

   void CLuaController::Destroy() {
      /* Execute script destroy function */
      CLuaUtility::CallFunction(m_ptLuaState, "destroy");
      /* Close Lua */
      lua_close(m_ptLuaState);
   }

   /****************************************/
   /****************************************/

}
