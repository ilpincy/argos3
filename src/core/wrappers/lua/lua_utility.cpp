
/**
 * @file <argos3/core/wrappers/lua/lua_utility.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "lua_utility.h"
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/utility/datatypes/color.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const CRange<Real> UNIT(0.0f, 1.0f);

   int LuaRNGBernoulli(lua_State* pt_state) {
      /* Check number of parameters */
      if(lua_gettop(pt_state) > 1) {
         return luaL_error(pt_state, "robot.random.bernoulli() expects 0 or 1 arguments");
      }
      /* Get RNG instance */
      CRandom::CRNG* pcRNG = CLuaUtility::GetDeviceInstance<CRandom::CRNG>(pt_state, "random");
      /* Perform wanted action */
      if(lua_gettop(pt_state) == 0) {
         /* Return random number */
         lua_pushnumber(pt_state, pcRNG->Bernoulli());
         return 1;
      }
      else {
         /* Check parameter */
         luaL_checktype(pt_state, 1, LUA_TNUMBER);
         /* Return random number */
         lua_pushnumber(pt_state,
                        pcRNG->Bernoulli(lua_tonumber(pt_state, 1)));
         return 1;
      }
      /* Can't reach this point */
      return 0;
   }

   int LuaRNGUniform(lua_State* pt_state) {
      /* Check number of parameters */
      if(lua_gettop(pt_state) > 2) {
         return luaL_error(pt_state, "robot.random.uniform() expects 0, 1, or 2 arguments");
      }
      /* Get RNG instance */
      CRandom::CRNG* pcRNG = CLuaUtility::GetDeviceInstance<CRandom::CRNG>(pt_state, "random");
      /* Perform wanted action */
      if(lua_gettop(pt_state) == 0) {
         /* Return a number between 0 and 1 */
         lua_pushnumber(pt_state, pcRNG->Uniform(UNIT));
         return 1;
      }
      else if(lua_gettop(pt_state) == 1) {
         /* Check parameter */
         luaL_checktype(pt_state, 1, LUA_TNUMBER);
         /* Return a number between 0 and the max */
         lua_pushnumber(pt_state,
                        pcRNG->Uniform(CRange<Real>(0,
                                                    lua_tonumber(pt_state, 1))));
         return 1;
      }
      else {
         /* Check parameters */
         luaL_checktype(pt_state, 1, LUA_TNUMBER);
         luaL_checktype(pt_state, 2, LUA_TNUMBER);
         /* Return a number between min and max */
         lua_pushnumber(pt_state,
                        pcRNG->Uniform(CRange<Real>(lua_tonumber(pt_state, 1),
                                                    lua_tonumber(pt_state, 2))));
         return 1;
      }
      /* Can't reach this point */
      return 0;
   }

   int LuaRNGUniformInt(lua_State* pt_state) {
      /* Check number of parameters */
      if(lua_gettop(pt_state) > 2) {
         return luaL_error(pt_state, "robot.random.uniform_int() expects 1 or 2 arguments");
      }
      /* Get RNG instance */
      CRandom::CRNG* pcRNG = CLuaUtility::GetDeviceInstance<CRandom::CRNG>(pt_state, "random");
      /* Perform wanted action */
      if(lua_gettop(pt_state) == 1) {
         /* Check parameter */
         luaL_checktype(pt_state, 1, LUA_TNUMBER);
         /* Return a number between 0 and the max */
         lua_pushnumber(pt_state,
                        pcRNG->Uniform(CRange<UInt32>(0,
                                                      Floor(lua_tonumber(pt_state, 1)))));
         return 1;
      }
      else {
         /* Check parameters */
         luaL_checktype(pt_state, 1, LUA_TNUMBER);
         luaL_checktype(pt_state, 2, LUA_TNUMBER);
         /* Return a number between min and max */
         lua_pushnumber(pt_state,
                        pcRNG->Uniform(CRange<SInt32>(Floor(lua_tonumber(pt_state, 1)),
                                                      Floor(lua_tonumber(pt_state, 2)))));
         return 1;
      }
      /* Can't reach this point */
      return 0;
   }

   int LuaRNGExponential(lua_State* pt_state) {
      /* Check number of parameters */
      if(lua_gettop(pt_state) != 1) {
         return luaL_error(pt_state, "robot.random.exponential() expects 1 argument");
      }
      /* Get RNG instance */
      CRandom::CRNG* pcRNG = CLuaUtility::GetDeviceInstance<CRandom::CRNG>(pt_state, "random");
      /* Check parameter */
      luaL_checktype(pt_state, 1, LUA_TNUMBER);
      /* Return random number */
      lua_pushnumber(pt_state,
                     pcRNG->Exponential(lua_tonumber(pt_state, 1)));
      return 1;
   }

   int LuaRNGGaussian(lua_State* pt_state) {
      /* Check number of parameters */
      if(lua_gettop(pt_state) != 1 && lua_gettop(pt_state) != 2) {
         return luaL_error(pt_state, "robot.random.gaussian() expects 1 or 2 arguments");
      }
      /* Get RNG instance */
      CRandom::CRNG* pcRNG = CLuaUtility::GetDeviceInstance<CRandom::CRNG>(pt_state, "random");
      /* Perform wanted action */
      if(lua_gettop(pt_state) == 1) {
         /* Check parameter */
         luaL_checktype(pt_state, 1, LUA_TNUMBER);
         /* Return random number */
         lua_pushnumber(pt_state, pcRNG->Gaussian(lua_tonumber(pt_state, 1)));
         return 1;
      }
      else {
         /* Check parameters */
         luaL_checktype(pt_state, 1, LUA_TNUMBER);
         luaL_checktype(pt_state, 2, LUA_TNUMBER);
         /* Return random number */
         lua_pushnumber(pt_state,
                        pcRNG->Gaussian(lua_tonumber(pt_state, 1),
                                        lua_tonumber(pt_state, 2)));
         return 1;
      }
      /* Can't reach this point */
      return 0;
   }

   /****************************************/
   /****************************************/

   bool CLuaUtility::LoadScript(lua_State* pt_state,
                                const std::string& str_filename) {
      if(luaL_loadfile(pt_state, str_filename.c_str())) {
         return false;
      }
      if(lua_pcall(pt_state, 0, 0, 0)) {
         return false;
      }
      return true;
   }

   /****************************************/
   /****************************************/

   bool CLuaUtility::CallLuaFunction(lua_State* pt_state,
                                     const std::string& str_function) {
      lua_getglobal(pt_state, str_function.c_str());
      if(lua_pcall(pt_state, 0, 0, 0)) {
         return false;
      }
      return true;
   }

   /****************************************/
   /****************************************/

   void PrintStackEntry(CARGoSLog& c_log, lua_State* pt_state, SInt32 n_index) {
      switch(lua_type(pt_state, n_index)) {
         case LUA_TBOOLEAN: c_log << lua_toboolean(pt_state, n_index); break;
         case LUA_TNUMBER: c_log << lua_tonumber(pt_state, n_index); break;
         case LUA_TSTRING: c_log << lua_tostring(pt_state, n_index); break;
         default: c_log << lua_topointer(pt_state, n_index); break;
      }
   }

   void RecursivePrintGlobals(CARGoSLog& c_log,
                              lua_State* pt_state,
                              size_t un_depth) {
      for(size_t i = 0; i < un_depth; ++i) {
         c_log << " ";
      }
      PrintStackEntry(c_log, pt_state, -2);
      c_log << " [" << lua_typename(pt_state, lua_type(pt_state, -1)) << "] ";
      if(lua_istable(pt_state, -1)) {
         c_log << std::endl;
         lua_pushnil(pt_state);
         while(lua_next(pt_state, -2)) {
            if(lua_type(pt_state, -1) != LUA_TFUNCTION &&
               (lua_type(pt_state, -2) != LUA_TSTRING ||
                (std::string(lua_tostring(pt_state, -2)) != "_G" &&
                 std::string(lua_tostring(pt_state, -2)) != "_VERSION" &&
                 std::string(lua_tostring(pt_state, -2)) != "package" &&
                 std::string(lua_tostring(pt_state, -2)) != "string" &&
                 std::string(lua_tostring(pt_state, -2)) != "os" &&
                 std::string(lua_tostring(pt_state, -2)) != "io" &&
                 std::string(lua_tostring(pt_state, -2)) != "math" &&
                 std::string(lua_tostring(pt_state, -2)) != "debug" &&
                 std::string(lua_tostring(pt_state, -2)) != "coroutine" &&
                 std::string(lua_tostring(pt_state, -2)) != "table"))) {
               RecursivePrintGlobals(c_log, pt_state, un_depth+1);
            }
            lua_pop(pt_state, 1);
         }
      }
      else {
         PrintStackEntry(c_log, pt_state, -1);
         c_log << std::endl;
      }
   }

   void CLuaUtility::PrintGlobals(CARGoSLog& c_log,
                                  lua_State* pt_state) {
      c_log << "*** LUA GLOBALS START ***" << std::endl;
      lua_getglobal(pt_state, "_G");
      RecursivePrintGlobals(c_log, pt_state, 0);
      lua_pop(pt_state, 1);
      c_log << "*** LUA GLOBALS END ***" << std::endl;
#ifdef ARGOS_THREADSAFE_LOG
      c_log.Flush();
#endif
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::PrintStack(CARGoSLog& c_log,
                                lua_State* pt_state) {
      c_log << "*** LUA STACK START ***" << std::endl;
      size_t unTop = lua_gettop(pt_state);
      c_log << "Elements in stack: " << unTop << std::endl;
      for(size_t i = unTop; i > 0; --i) {
         c_log << "#" << i << " [" << lua_typename(pt_state, lua_type(pt_state, i)) << "] ";
         PrintStackEntry(c_log, pt_state, i);
         c_log << std::endl;
      }
      c_log << "*** LUA STACK END ***" << std::endl;
#ifdef ARGOS_THREADSAFE_LOG
      c_log.Flush();
#endif
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::RegisterLoggerWrapper(lua_State* pt_state) {
      lua_register(pt_state, "log", LOGWrapper);
      lua_register(pt_state, "logerr", LOGERRWrapper);
   }
   
   /****************************************/
   /****************************************/

   void CLuaUtility::RegisterRNG(lua_State* pt_state,
                                 CRandom::CRNG* pc_rng) {
      pc_rng->Reset();
      OpenRobotStateTable(pt_state, "random");
      AddToTable(pt_state, "_instance", pc_rng);
      AddToTable(pt_state, "bernoulli", &LuaRNGBernoulli);
      AddToTable(pt_state, "uniform", &LuaRNGUniform);
      AddToTable(pt_state, "uniform_int", &LuaRNGUniformInt);
      AddToTable(pt_state, "exponential", &LuaRNGExponential);
      AddToTable(pt_state, "gaussian", &LuaRNGGaussian);
      CloseRobotStateTable(pt_state);
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::OpenRobotStateTable(lua_State* pt_state,
                                         const std::string& str_key) {
      lua_pushstring(pt_state, str_key.c_str());
      lua_rawget(pt_state, -2);
      if(lua_isnil(pt_state, -1)) {
         lua_pop(pt_state, 1);
         StartTable(pt_state, str_key);
         EndTable(pt_state);
         lua_pushstring(pt_state, str_key.c_str());
         lua_rawget(pt_state, -2);
      }
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::CloseRobotStateTable(lua_State* pt_state) {
      lua_pop(pt_state, 1);
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::StartTable(lua_State* pt_state,
                                const std::string& str_key) {
      lua_pushstring(pt_state, str_key.c_str());
      lua_newtable (pt_state);
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::StartTable(lua_State* pt_state,
                                int n_key) {
      lua_pushnumber(pt_state, n_key);
      lua_newtable  (pt_state);
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::EndTable(lua_State* pt_state) {
      lua_settable(pt_state, -3);
   }
   
   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                const std::string& str_key,
                                void* pt_data) {
      lua_pushstring       (pt_state, str_key.c_str());
      lua_pushlightuserdata(pt_state, pt_data        );
      lua_settable         (pt_state, -3             );
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                const std::string& str_key,
                                lua_CFunction t_data) {
      lua_pushstring   (pt_state, str_key.c_str());
      lua_pushcfunction(pt_state, t_data         );
      lua_settable     (pt_state, -3             );
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                const std::string& str_key,
                                Real f_data) {
      lua_pushstring(pt_state, str_key.c_str());
      lua_pushnumber(pt_state, f_data         );
      lua_settable  (pt_state, -3             );
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                const std::string& str_key,
                                const std::string& str_data){
	  lua_pushstring(pt_state, str_key.c_str() );
	  lua_pushstring(pt_state, str_data.c_str());
	  lua_settable  (pt_state, -3              );
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                int n_key,
                                Real f_data) {
      lua_pushnumber(pt_state, n_key );
      lua_pushnumber(pt_state, f_data);
      lua_settable  (pt_state, -3    );
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                const std::string& str_key,
                                const CRadians& c_data) {
      lua_pushstring(pt_state, str_key.c_str()  );
      lua_pushnumber(pt_state, c_data.GetValue());
      lua_settable  (pt_state, -3               );
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                int n_key,
                                const CRadians& c_data) {
      lua_pushnumber(pt_state, n_key            );
      lua_pushnumber(pt_state, c_data.GetValue());
      lua_settable  (pt_state, -3               );
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                const std::string& str_key,
                                const CVector2& c_data) {
      StartTable(pt_state, str_key               );
      AddToTable(pt_state, "_type", TYPE_VECTOR2 );
      AddToTable(pt_state, "x",     c_data.GetX());
      AddToTable(pt_state, "y",     c_data.GetY());
      EndTable  (pt_state                        );
   }
   
   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                int n_key,
                                const CVector2& c_data) {
      StartTable(pt_state, n_key                 );
      AddToTable(pt_state, "_type", TYPE_VECTOR2 );
      AddToTable(pt_state, "x",     c_data.GetX());
      AddToTable(pt_state, "y",     c_data.GetY());
      EndTable  (pt_state                        );
   }
   
   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                const std::string& str_key,
                                const CVector3& c_data) {
      StartTable(pt_state, str_key               );
      AddToTable(pt_state, "_type", TYPE_VECTOR3 );
      AddToTable(pt_state, "x",     c_data.GetX());
      AddToTable(pt_state, "y",     c_data.GetY());
      AddToTable(pt_state, "z",     c_data.GetZ());
      EndTable  (pt_state                        );
   }
   
   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                int n_key,
                                const CVector3& c_data) {
      StartTable(pt_state, n_key                 );
      AddToTable(pt_state, "_type", TYPE_VECTOR3 );
      AddToTable(pt_state, "x",     c_data.GetX());
      AddToTable(pt_state, "y",     c_data.GetY());
      AddToTable(pt_state, "z",     c_data.GetZ());
      EndTable  (pt_state                        );
   }
   
   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                const std::string& str_key,
                                const CQuaternion& c_data) {
      CRadians cAngle;
      CVector3 cAxis;
      c_data.ToAngleAxis(cAngle, cAxis);
      StartTable(pt_state, str_key                 );
      AddToTable(pt_state, "_type", TYPE_QUATERNION);
      AddToTable(pt_state, "angle", cAngle         );
      AddToTable(pt_state, "axis",  cAxis          );
      EndTable  (pt_state                          );
   }
   
   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                int n_key,
                                const CQuaternion& c_data) {
      CRadians cAngle;
      CVector3 cAxis;
      c_data.ToAngleAxis(cAngle, cAxis);
      StartTable(pt_state, n_key                   );
      AddToTable(pt_state, "_type", TYPE_QUATERNION);
      AddToTable(pt_state, "angle", cAngle         );
      AddToTable(pt_state, "axis",  cAxis          );
      EndTable  (pt_state                          );
   }
   
   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                const std::string& str_key,
                                const CColor& c_data) {
      StartTable(pt_state, str_key                   );
      AddToTable(pt_state, "_type", TYPE_COLOR       );
      AddToTable(pt_state, "red",   c_data.GetRed()  );
      AddToTable(pt_state, "green", c_data.GetGreen());
      AddToTable(pt_state, "blue",  c_data.GetBlue() );
      EndTable  (pt_state                            );
   }
   
   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                int n_key,
                                const CColor& c_data) {
      StartTable(pt_state, n_key                     );
      AddToTable(pt_state, "_type", TYPE_COLOR       );
      AddToTable(pt_state, "red",   c_data.GetRed()  );
      AddToTable(pt_state, "green", c_data.GetGreen());
      AddToTable(pt_state, "blue",  c_data.GetBlue() );
      EndTable  (pt_state                            );
   }
   
   /****************************************/
   /****************************************/

   int CLuaUtility::LOGWrapper(lua_State* pt_state) {
      return LoggerWrapper(LOG, pt_state);
   }
      
   /****************************************/
   /****************************************/

   int CLuaUtility::LOGERRWrapper(lua_State* pt_state) {
      return LoggerWrapper(LOGERR, pt_state);
   }

   /****************************************/
   /****************************************/

   int CLuaUtility::LoggerWrapper(CARGoSLog& c_log,
                                  lua_State* pt_state) {
      /* Get number of arguments */
      UInt32 unArgc = lua_gettop(pt_state);
      /* Send arguments to log one by one */
      UInt32 unType;
      for(UInt32 i = 1; i <= unArgc; ++i) {
         unType = lua_type(pt_state, i);
         switch(unType) {
            case LUA_TBOOLEAN: c_log << lua_toboolean(pt_state, i); break;
            case LUA_TNUMBER:  c_log << lua_tonumber (pt_state, i); break;
            case LUA_TSTRING:  c_log << lua_tostring (pt_state, i); break;
            default: c_log << lua_typename (pt_state, unType); break;
         }
      }
      c_log << std::endl;
      /* No result is calculated */
      return 0;
   }

   /****************************************/
   /****************************************/

}
