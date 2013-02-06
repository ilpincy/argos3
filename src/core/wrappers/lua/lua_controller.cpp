#include "lua_controller.h"
#include <argos3/core/utility/logging/argos_log.h>

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}

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
         LoadScript(strScriptFileName);
         /* Create and set variables */
         CreateLuaVariables();
         SensorReadingsToLuaVariables();
         /* Execute script init function */
         CallLuaFunction("init");
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
      CallLuaFunction("step");
      /* Set actuator variables */
      LuaVariablesToActuatorSettings();
      DumpGlobals();
   }

   /****************************************/
   /****************************************/

   void CLuaController::Reset() {
      /* Execute script reset function */
      CallLuaFunction("reset");
   }

   /****************************************/
   /****************************************/

   void CLuaController::Destroy() {
      /* Execute script destroy function */
      CallLuaFunction("destroy");
      /* Close Lua */
      lua_close(m_ptLuaState);
   }

   /****************************************/
   /****************************************/

   void CLuaController::LoadScript(const std::string& str_filename) {
      if(luaL_loadfile(m_ptLuaState, str_filename.c_str())) {
         THROW_ARGOSEXCEPTION("Could not load Lua script file \"" << lua_tostring(m_ptLuaState, -1) << "\"");
      }
      if(lua_pcall(m_ptLuaState, 0, 0, 0)) {
         THROW_ARGOSEXCEPTION("Error starting Lua script \"" << str_filename << "\": " << lua_tostring(m_ptLuaState, -1));
      }
   }

   /****************************************/
   /****************************************/

   void CLuaController::CallLuaFunction(const std::string& str_function) {
      lua_getglobal(m_ptLuaState, str_function.c_str());
      if(lua_pcall(m_ptLuaState, 0, 0, 0)) {
         THROW_ARGOSEXCEPTION("Could not execute Lua script file: " << lua_tostring(m_ptLuaState, -1));
      }
   }

   /****************************************/
   /****************************************/

   void PrintLuaStackEntry(CARGoSLog& c_log, lua_State* pt_state, SInt32 n_index) {
      switch(lua_type(pt_state, n_index)) {
         case LUA_TBOOLEAN: c_log << lua_toboolean(pt_state, n_index); break;
         case LUA_TNUMBER: c_log << lua_tonumber(pt_state, n_index); break;
         case LUA_TSTRING: c_log << lua_tostring(pt_state, n_index); break;
         default: break;
      }
   }

   void PrintLuaStack(lua_State* pt_state) {
      size_t unTop = lua_gettop(pt_state);
      LOG << "Elements in stack: " << unTop << std::endl;
      for(size_t i = unTop; i > 0; --i) {
         LOG << "[" << lua_typename(pt_state, lua_type(pt_state, i)) << "] ";
         PrintLuaStackEntry(LOG, pt_state, i);
         LOG << std::endl;
      }
      LOG.Flush();
   }

   void RecursiveDumpGlobals(lua_State* pt_state,
                             size_t un_depth) {
      for(size_t i = 0; i < un_depth; ++i) {
         LOGERR << " ";
      }
      LOGERR << "[" << lua_typename(pt_state, lua_type(pt_state, -1)) << "] ";
      LOGERR.Flush();
      if(lua_istable(pt_state, -1)) {
         PrintLuaStackEntry(LOGERR, pt_state, -2);
         LOGERR << std::endl;
         LOGERR.Flush();
         lua_pushnil(pt_state);
         while(lua_next(pt_state, -2)) {
            if(lua_type(pt_state, -1) != LUA_TFUNCTION &&
               (lua_type(pt_state, -2) != LUA_TSTRING ||
                (std::string(lua_tostring(pt_state, -2)) != "_G" &&
                 std::string(lua_tostring(pt_state, -2)) != "package" &&
                 std::string(lua_tostring(pt_state, -2)) != "string" &&
                 std::string(lua_tostring(pt_state, -2)) != "os" &&
                 std::string(lua_tostring(pt_state, -2)) != "io" &&
                 std::string(lua_tostring(pt_state, -2)) != "math" &&
                 std::string(lua_tostring(pt_state, -2)) != "debug" &&
                 std::string(lua_tostring(pt_state, -2)) != "coroutine" &&
                 std::string(lua_tostring(pt_state, -2)) != "table"))) {
               RecursiveDumpGlobals(pt_state, un_depth+1);
            }
            lua_pop(pt_state, 1);
         }
      }
      else {
         PrintLuaStackEntry(LOGERR, pt_state, -1);
         LOGERR << std::endl;
         LOGERR.Flush();
      }
   }

   void CLuaController::DumpGlobals() {
      LOGERR << "*** LUA START ***" << std::endl;
      lua_getglobal(m_ptLuaState, "_G");
      RecursiveDumpGlobals(m_ptLuaState, 0);
      lua_pop(m_ptLuaState, 1);
      LOGERR << "*** LUA END ***" << std::endl;
   }

   /****************************************/
   /****************************************/

}
