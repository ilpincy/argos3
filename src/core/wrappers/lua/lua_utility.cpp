
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
#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/utility/datatypes/color.h>

#include <argos3/core/wrappers/lua/lua_vector2.h>
#include <argos3/core/wrappers/lua/lua_vector3.h>
#include <argos3/core/wrappers/lua/lua_quaternion.h>

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
         /* Return random result */
         lua_pushboolean(pt_state, pcRNG->Bernoulli());
         return 1;
      }
      else {
         /* Check parameter */
         luaL_checktype(pt_state, 1, LUA_TNUMBER);
         /* Return random number */
         lua_pushboolean(pt_state,
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

   int LuaRNGPoisson(lua_State* pt_state) {
      /* Check number of parameters */
      if(lua_gettop(pt_state) != 1) {
         return luaL_error(pt_state, "robot.random.poisson() expects 1 argument");
      }
      /* Get RNG instance */
      CRandom::CRNG* pcRNG = CLuaUtility::GetDeviceInstance<CRandom::CRNG>(pt_state, "random");
      /* Check parameter */
      luaL_checktype(pt_state, 1, LUA_TNUMBER);
      /* Return random number */
      lua_pushinteger(pt_state,
                     pcRNG->Poisson(lua_tonumber(pt_state, 1)));
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
         LOGERR << "[FATAL] Error loading \"" << str_filename
                << "\"" << std::endl;
         return false;
      }
      if(lua_pcall(pt_state, 0, 0, 0)) {
         LOGERR << "[FATAL] Error executing \"" << str_filename
                << "\"" << std::endl;
         LOGERR << "[FATAL] " << lua_tostring(pt_state, -1)
                << std::endl;
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
         LOGERR << "[FATAL] Error calling \"" << str_function
                << "\"" << std::endl;
         LOGERR << "[FATAL] " << lua_tostring(pt_state, -1)
                << std::endl;
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

   /****************************************/
   /****************************************/

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

   /****************************************/
   /****************************************/

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
      AddToTable(pt_state, "poisson", &LuaRNGPoisson);
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

   void CLuaUtility::SetMetatable(lua_State* pt_state,
                                  const std::string& str_key) {
      luaL_getmetatable(pt_state, str_key.c_str());
      lua_setmetatable(pt_state, -2);
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
                                const std::string& str_data){
      lua_pushstring(pt_state, str_key.c_str() );
      lua_pushstring(pt_state, str_data.c_str());
      lua_settable  (pt_state, -3              );
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                int n_key,
                                const std::string& str_data){
      lua_pushnumber(pt_state, n_key );
      lua_pushstring(pt_state, str_data.c_str());
      lua_settable  (pt_state, -3              );
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
      lua_pushstring          (pt_state, str_key.c_str());
      CLuaVector2::PushVector2(pt_state, c_data);
      lua_settable            (pt_state, -3);
   }
   
   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                int n_key,
                                const CVector2& c_data) {
      lua_pushnumber          (pt_state, n_key);
      CLuaVector2::PushVector2(pt_state, c_data);
      lua_settable            (pt_state, -3);
   }
   
   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                const std::string& str_key,
                                const CVector3& c_data) {
      lua_pushstring          (pt_state, str_key.c_str());
      CLuaVector3::PushVector3(pt_state, c_data);
      lua_settable            (pt_state, -3);
   }
   
   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                int n_key,
                                const CVector3& c_data) {
      lua_pushnumber          (pt_state, n_key);
      CLuaVector3::PushVector3(pt_state, c_data);
      lua_settable            (pt_state, -3);
   }

   
   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                const std::string& str_key,
                                const CQuaternion& c_data) {
      lua_pushstring                (pt_state, str_key.c_str());
      CLuaQuaternion::PushQuaternion(pt_state, c_data);
      lua_settable                  (pt_state, -3);
   }
   
   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                int n_key,
                                const CQuaternion& c_data) {
      lua_pushnumber                (pt_state, n_key);
      CLuaQuaternion::PushQuaternion(pt_state, c_data);
      lua_settable                  (pt_state, -3);
   }
   
   /****************************************/
   /****************************************/

   void CLuaUtility::AddToTable(lua_State* pt_state,
                                const std::string& str_key,
                                const CColor& c_data) {
      StartTable(pt_state, str_key                   );
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

   void CLuaUtility::LuaSerializeTable(CByteArray& c_data,
                                       lua_State* pt_lua_state,
                                       SInt32 n_table_offset) {
      lua_pushnil(pt_lua_state);
      /* if we are indexing from the top of the stack (negative numbers) then we need to decrement
          n_table_offset to compensate for the nil that we just pushed onto the stack */
      if(n_table_offset < 0) {
         n_table_offset -= 1;
      }
      while(lua_next(pt_lua_state, n_table_offset)) {
         /* write the key */
         SInt8 nKeyTypeId = static_cast<UInt8>(lua_type(pt_lua_state, -2));
         SInt8 nValueTypeId = static_cast<UInt8>(lua_type(pt_lua_state, -1));
         CByteArray cKeyValueBytes;
         cKeyValueBytes << nKeyTypeId;
         switch(nKeyTypeId) {
         case LUA_TSTRING:
            cKeyValueBytes << lua_tostring(pt_lua_state, -2);
            break;
         case LUA_TNUMBER:
            cKeyValueBytes << lua_tonumber(pt_lua_state, -2);
            break;
         case LUA_TBOOLEAN:
            cKeyValueBytes << static_cast<SInt8>(lua_toboolean(pt_lua_state, -2));
            break;
         case LUA_TTABLE:
            LuaSerializeTable(cKeyValueBytes, pt_lua_state, -2);
            break;
         case LUA_TUSERDATA:
            if (lua_getmetatable(pt_lua_state, -2)) {
               /* push the vector2 metatable onto the stack */
               lua_getfield(pt_lua_state, LUA_REGISTRYINDEX, CLuaVector2::GetTypeId().c_str());  /* is it a vector2? */
               if (lua_rawequal(pt_lua_state, -2, -1)) {
                  /* pop both the two metatables off the stack */
                  lua_pop(pt_lua_state, 2);
                  cKeyValueBytes << static_cast<UInt8>(LUA_TUSERDATA_VECTOR2);
                  const CVector2& cVector2 = *static_cast<CVector2*>(lua_touserdata(pt_lua_state, -2));
                  cKeyValueBytes << cVector2.GetX() << cVector2.GetY();
               }
               else {
                  /* pop the vector2 metatable off the stack */
                  lua_pop(pt_lua_state, 1);
                  /* push the vector3 metatable onto the stack */
                  lua_getfield(pt_lua_state, LUA_REGISTRYINDEX, CLuaVector3::GetTypeId().c_str());  /* is it a vector3? */
                  if (lua_rawequal(pt_lua_state, -2, -1)) {
                     /* pop both the two metatables off the stack */
                     lua_pop(pt_lua_state, 2);
                     cKeyValueBytes << static_cast<UInt8>(LUA_TUSERDATA_VECTOR3);
                     const CVector3& cVector3 = *static_cast<CVector3*>(lua_touserdata(pt_lua_state, -2));
                     cKeyValueBytes << cVector3.GetX() << cVector3.GetY() << cVector3.GetZ();
                  }
                  else {
                     /* pop the vector3 metatable off the stack */
                     lua_pop(pt_lua_state, 1);
                     /* push the quaternion metatable onto the stack */
                     lua_getfield(pt_lua_state, LUA_REGISTRYINDEX, CLuaQuaternion::GetTypeId().c_str());  /* is it a quaternion? */
                     if (lua_rawequal(pt_lua_state, -2, -1)) {
                        /* pop both the metatables off the stack */
                        lua_pop(pt_lua_state, 2);
                        cKeyValueBytes << static_cast<UInt8>(LUA_TUSERDATA_QUATERNION);
                        const CQuaternion& cQuaternion = *static_cast<CQuaternion*>(lua_touserdata(pt_lua_state, -2));
                        cKeyValueBytes << cQuaternion.GetW() << cQuaternion.GetX() << cQuaternion.GetY() << cQuaternion.GetZ();
                     }
                     else {
                        /* unrecognized userdata subtype detected */
                        lua_pushstring(pt_lua_state, "Unrecognized userdata subtype detected in table");
                        lua_error(pt_lua_state);
                     }
                  }
               }
            }
            break;
         default:
            /* key type not recognized, move to the next pair */
            lua_pop(pt_lua_state, 1);
            continue;
            break;
         }
         /* write the value */
         cKeyValueBytes << nValueTypeId;
         switch(nValueTypeId) {
         case LUA_TSTRING:
            cKeyValueBytes << lua_tostring(pt_lua_state, -1);
            break;
         case LUA_TNUMBER:
            cKeyValueBytes << lua_tonumber(pt_lua_state, -1);
            break;
         case LUA_TBOOLEAN:
            cKeyValueBytes << static_cast<SInt8>(lua_toboolean(pt_lua_state, -1));
            break;
         case LUA_TTABLE:
            LuaSerializeTable(cKeyValueBytes, pt_lua_state, -1);
            break;
         case LUA_TUSERDATA:
            if (lua_getmetatable(pt_lua_state, -1)) {
               /* push the vector2 metatable onto the stack */
               lua_getfield(pt_lua_state, LUA_REGISTRYINDEX, CLuaVector2::GetTypeId().c_str());  /* is it a vector2? */
               if (lua_rawequal(pt_lua_state, -2, -1)) {
                  /* pop both the two metatables off the stack */
                  lua_pop(pt_lua_state, 2);
                  cKeyValueBytes << static_cast<UInt8>(LUA_TUSERDATA_VECTOR2);
                  const CVector2& cVector2 = *static_cast<CVector2*>(lua_touserdata(pt_lua_state, -1));
                  cKeyValueBytes << cVector2.GetX() << cVector2.GetY();
               }
               else {
                  /* pop the vector2 metatable off the stack */
                  lua_pop(pt_lua_state, 1);
                  /* push the vector3 metatable onto the stack */
                  lua_getfield(pt_lua_state, LUA_REGISTRYINDEX, CLuaVector3::GetTypeId().c_str());  /* is it a vector3? */
                  if (lua_rawequal(pt_lua_state, -2, -1)) {
                     /* pop both the two metatables off the stack */
                     lua_pop(pt_lua_state, 2);
                     cKeyValueBytes << static_cast<UInt8>(LUA_TUSERDATA_VECTOR3);
                     const CVector3& cVector3 = *static_cast<CVector3*>(lua_touserdata(pt_lua_state, -1));
                     cKeyValueBytes << cVector3.GetX() << cVector3.GetY() << cVector3.GetZ();
                  }
                  else {
                     /* pop the vector3 metatable off the stack */
                     lua_pop(pt_lua_state, 1);
                     /* push the quaternion metatable onto the stack */
                     lua_getfield(pt_lua_state, LUA_REGISTRYINDEX, CLuaQuaternion::GetTypeId().c_str());  /* is it a quaternion? */
                     if (lua_rawequal(pt_lua_state, -2, -1)) {
                        /* pop both the metatables off the stack */
                        lua_pop(pt_lua_state, 2);
                        cKeyValueBytes << static_cast<UInt8>(LUA_TUSERDATA_QUATERNION);
                        const CQuaternion& cQuaternion = *static_cast<CQuaternion*>(lua_touserdata(pt_lua_state, -1));
                        cKeyValueBytes << cQuaternion.GetW() << cQuaternion.GetX() << cQuaternion.GetY() << cQuaternion.GetZ();
                     }
                     else {
                        /* unrecognized userdata subtype detected */
                        lua_pushstring(pt_lua_state, "Unrecognized userdata subtype detected in table");
                        lua_error(pt_lua_state);
                     }
                  }
               }
            }
            break;
         default:
            /* value type not recognized, move to the next pair */
            lua_pop(pt_lua_state, 1);
            continue;
            break;
         }
         c_data.AddBuffer(cKeyValueBytes.ToCArray(), cKeyValueBytes.Size());
         lua_pop(pt_lua_state, 1);
      }
      /* insert a Lua nil value (0) into the data stream as a sentinel value */
      c_data << static_cast<SInt8>(LUA_TNIL);
   }

   /****************************************/
   /****************************************/

   void CLuaUtility::LuaDeserializeTable(CByteArray& c_data,
                                         lua_State* pt_lua_state) {
      SInt8 nKeyType, nValueType;
      std::string strBuffer;
      Real fBuffer[4];
      SInt8 nBuffer;
      UInt8 unUserDataSubtype;
      bool bContinue = true;
      while(bContinue && !c_data.Empty()) {
         c_data >> nKeyType;
         switch(nKeyType) {
         case LUA_TSTRING:
            c_data >> strBuffer;
            lua_pushstring(pt_lua_state, strBuffer.c_str());
            break;
         case LUA_TNUMBER:
            c_data >> fBuffer[0];
            lua_pushnumber(pt_lua_state, fBuffer[0]);
            break;
         case LUA_TBOOLEAN:
            c_data >> nBuffer;
            lua_pushboolean(pt_lua_state, nBuffer);
            break;
         case LUA_TTABLE:
            lua_newtable (pt_lua_state);
            LuaDeserializeTable(c_data, pt_lua_state);
            break;
         case LUA_TUSERDATA:
            c_data >> unUserDataSubtype;
            switch(unUserDataSubtype) {
            case LUA_TUSERDATA_VECTOR2:
               c_data >> fBuffer[0] >> fBuffer[1];
               CLuaVector2::PushVector2(pt_lua_state, fBuffer[0], fBuffer[1]);
               break;
            case LUA_TUSERDATA_VECTOR3:
               c_data >> fBuffer[0] >> fBuffer[1] >> fBuffer[2];
               CLuaVector3::PushVector3(pt_lua_state, fBuffer[0], fBuffer[1], fBuffer[2]);
               break;
            case LUA_TUSERDATA_QUATERNION:
               c_data >> fBuffer[0] >> fBuffer[1] >> fBuffer[2] >> fBuffer[3];
               CLuaQuaternion::PushQuaternion(pt_lua_state, fBuffer[0], fBuffer[1], fBuffer[2], fBuffer[3]);
               break;
            default:
               /* key is not a recognized subtype of userdata */
               bContinue = false;
               continue;
               break;
            }
            break;
         default:
            /* sentinel or the key is not a recognized type */
            bContinue = false;
            continue;
            break;
         }
         c_data >> nValueType;
         switch(nValueType) {
         case LUA_TSTRING:
            c_data >> strBuffer;
            lua_pushstring(pt_lua_state, strBuffer.c_str());
            break;
         case LUA_TNUMBER:
            c_data >> fBuffer[0];
            lua_pushnumber(pt_lua_state, fBuffer[0]);
            break;
         case LUA_TBOOLEAN:
            c_data >> nBuffer;
            lua_pushboolean(pt_lua_state, nBuffer);
            break;
         case LUA_TTABLE:
            lua_newtable (pt_lua_state);
            LuaDeserializeTable(c_data, pt_lua_state);
            break;
         case LUA_TUSERDATA:
            c_data >> unUserDataSubtype;
            switch(unUserDataSubtype) {
            case LUA_TUSERDATA_VECTOR2:
               c_data >> fBuffer[0] >> fBuffer[1];
               CLuaVector2::PushVector2(pt_lua_state, fBuffer[0], fBuffer[1]);
               break;
            case LUA_TUSERDATA_VECTOR3:
               c_data >> fBuffer[0] >> fBuffer[1] >> fBuffer[2];
               CLuaVector3::PushVector3(pt_lua_state, fBuffer[0], fBuffer[1], fBuffer[2]);
               break;
            case LUA_TUSERDATA_QUATERNION:
               c_data >> fBuffer[0] >> fBuffer[1] >> fBuffer[2] >> fBuffer[3];
               CLuaQuaternion::PushQuaternion(pt_lua_state, fBuffer[0], fBuffer[1], fBuffer[2], fBuffer[3]);
               break;
            default:
               /* key is not a recognized subtype of userdata */
               bContinue = false;
               /* remove the key from the stack */
               lua_pop(pt_lua_state, 1);
               continue;
               break;
            }
            break;
         default:
            /* the value is not a recognized type */
            bContinue = false;
            /* remove the key from the stack */
            lua_pop(pt_lua_state, 1);
            continue;
            break;
         }
         lua_settable(pt_lua_state, -3);
      }
   }

   /****************************************/
   /****************************************/

}
