#ifndef LUA_UTILITY_H
#define LUA_UTILITY_H

/**
 * @file <argos3/core/wrappers/lua/lua_utility.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

namespace argos {
   class CRadians;
   class CVector2;
   class CVector3;
   class CQuaternion;
   class CColor;
}

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/rng.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <string>

namespace argos {

   class CLuaUtility {
      
   public:

      /**
       * Loads the given Lua script.
       * @param pt_state The Lua state.
       * @param str_filename The script file name.
       * @return <tt>false</tt> in case of errors, <tt>true</tt> otherwise.
       */
      static bool LoadScript(lua_State* pt_state,
                             const std::string& str_filename);
      
      /**
       * Calls a parameter-less function in the Lua script.
       * @param pt_state The Lua state.
       * @param str_function The function name.
       * @return <tt>false</tt> in case of errors, <tt>true</tt> otherwise.
       */
      static bool CallLuaFunction(lua_State* pt_state,
                                  const std::string& str_function);

      /**
       * Prints the global Lua symbols on the specified log.
       * @param c_log The output log.
       * @param pt_state The Lua state.
       * @see LOG
       * @see LOGERR
       * @see CARGoSLogger
       */
      static void PrintGlobals(CARGoSLog& c_log,
                               lua_State* pt_state);
      
      /**
       * Prints the Lua stack on the specified log.
       * @param c_log The output log.
       * @param pt_state The Lua state.
       * @see LOG
       * @see LOGERR
       * @see CARGoSLogger
       */
      static void PrintStack(CARGoSLog& c_log,
                             lua_State* pt_state);

      /**
       * Registers LOG and LOGERR in the Lua state.
       * After this call, in a Lua script one can use <tt>log()</tt> and
       * <tt>logerr()</tt> to print to the ARGoS logs.
       * @param pt_state The Lua state.
       * @see LOG
       * @see LOGERR
       * @see CARGoSLogger
       */
      static void RegisterLoggerWrapper(lua_State* pt_state);

      /**
       * Registers the given random number generator in the Lua state.
       * Internally, it resets the passed RNG.
       * @param pt_state The Lua state.
       * @param pc_rng The random number generator.
       * @see CRandom
       * @see CRandom::CRNG
       * @see CRandom::CRNG::Reset()
       */
      static void RegisterRNG(lua_State* pt_state,
                              CRandom::CRNG* pc_rng);

      /**
       * Opens a table in the robot state, creating it if it does not exist.
       * This method expects the "robot" table to be at -1 in the stack, and
       * pushes the table object on the stack.
       * To close the table call CloseRobotStateTable().
       * @param pt_state The Lua state.
       * @param str_key The string key for the robot state table.
       * @see CloseRobotStateTable()
       */
      static void OpenRobotStateTable(lua_State* pt_state,
                                      const std::string& str_key);

      /**
       * Closes a table in the robot state.
       * This method expects the table itself to be at the top of the stack (-1).
       * @param pt_state The Lua state.
       * @see OpenRobotStateTable()
       */
      static void CloseRobotStateTable(lua_State* pt_state);

      /**
       * Adds a table with the given string key to the table located at the top of the stack.
       * This method pushes the table key and the table itself on the stack.
       * To close a table call EndTable().
       * @param pt_state The Lua state.
       * @param str_key The string key for the parent table.
       * @see EndTable()
       */
      static void StartTable(lua_State* pt_state,
                             const std::string& str_key);

      /**
       * Adds a table with the given numeric key to the table located at the top of the stack.
       * This method pushes the table key and the table itself on the stack.
       * To close a table call EndTable().
       * @param pt_state The Lua state.
       * @param n_key The numeric key for the parent table.
       * @see EndTable()
       */
      static void StartTable(lua_State* pt_state,
                             int n_key);

      /**
       * Adds a table to the Lua stack.
       * This method expects the table itself to be at the top of the stack (-1)
       * and the table name to be a index -2.
       * @param pt_state The Lua state.
       * @see StartTable()
       */
      static void EndTable(lua_State* pt_state);


      /**
       * Sets the metatable with the given string key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param str_key The string key for the metatable.
       */
      static void SetMetatable(lua_State* pt_state,
                               const std::string& str_key);


      /**
       * Adds a pointer to a chunk of data with the given string key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param str_key The string key for the parent table.
       * @param pt_data The data to add to the table.
       */
      static void AddToTable(lua_State* pt_state,
                             const std::string& str_key,
                             void* pt_data);

      /**
       * Adds a pointer to a C function with the given string key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param str_key The string key for the parent table.
       * @param pt_data The data to add to the table.
       */
      static void AddToTable(lua_State* pt_state,
                             const std::string& str_key,
                             lua_CFunction pt_data);

      /**
       * Adds a string with the given string key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param str_key The string key for the parent table.
       * @param str_data The data to add to the table.
       */
      static void AddToTable(lua_State* pt_state,
                             const std::string& str_key,
                             const std::string& str_data);

      /**
       * Adds a string with the given string key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param n_key The numeric key for the parent table.
       * @param str_data The data to add to the table.
       */
      static void AddToTable(lua_State* pt_state,
                             int n_key,
                             const std::string& str_data);

      /**
       * Adds a Real with the given string key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param str_key The string key for the parent table.
       * @param f_data The data to add to the table.
       */
      static void AddToTable(lua_State* pt_state,
                             const std::string& str_key,
                             Real f_data);

      /**
       * Adds a Real with the given numeric key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param n_key The numeric key for the parent table.
       * @param f_data The data to add to the table.
       */
      static void AddToTable(lua_State* pt_state,
                             int n_key,
                             Real f_data);

      /**
       * Adds a CRadians with the given string key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param str_key The string key for the parent table.
       * @param c_data The data to add to the table.
       */
      static void AddToTable(lua_State* pt_state,
                             const std::string& str_key,
                             const CRadians& c_data);

      /**
       * Adds a CRadians with the given numeric key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param n_key The numeric key for the parent table.
       * @param c_data The data to add to the table.
       */
      static void AddToTable(lua_State* pt_state,
                             int n_key,
                             const CRadians& c_data);

      /**
       * Adds a CVector2 the given string key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param str_key The string key for the parent table.
       * @param c_data The data to add to the table.
       * @see StartTable()
       * @see EndTable()
       */
      static void AddToTable(lua_State* pt_state,
                             const std::string& str_key,
                             const CVector2& c_data);
      
      /**
       * Adds a CVector2 the given numeric key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param n_key The numeric key for the parent table.
       * @param c_data The data to add to the table.
       * @see StartTable()
       * @see EndTable()
       */
      static void AddToTable(lua_State* pt_state,
                             int n_key,
                             const CVector2& c_data);
      
      /**
       * Adds a CVector3 the given string key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param str_key The string key for the parent table.
       * @param c_data The data to add to the table.
       * @see StartTable()
       * @see EndTable()
       */
      static void AddToTable(lua_State* pt_state,
                             const std::string& str_key,
                             const CVector3& c_data);
      
      /**
       * Adds a CVector3 the given numeric key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param n_key The numeric key for the parent table.
       * @param c_data The data to add to the table.
       * @see StartTable()
       * @see EndTable()
       */
      static void AddToTable(lua_State* pt_state,
                             int n_key,
                             const CVector3& c_data);
      
      /**
       * Adds a CQuaternion the given string key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param str_key The string key for the parent table.
       * @param c_data The data to add to the table.
       * @see StartTable()
       * @see EndTable()
       */
      static void AddToTable(lua_State* pt_state,
                             const std::string& str_key,
                             const CQuaternion& c_data);
      
      /**
       * Adds a CQuaternion the given numeric key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param n_key The numeric key for the parent table.
       * @param c_data The data to add to the table.
       * @see StartTable()
       * @see EndTable()
       */
      static void AddToTable(lua_State* pt_state,
                             int n_key,
                             const CQuaternion& c_data);
      
      /**
       * Adds a CColor the given string key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param str_key The string key for the parent table.
       * @param c_data The data to add to the table.
       * @see StartTable()
       * @see EndTable()
       */
      static void AddToTable(lua_State* pt_state,
                             const std::string& str_key,
                             const CColor& c_data);
      
      /**
       * Adds a CColor the given numeric key to the table located at the top of the stack.
       * At the end of the execution, the stack is in the same state as it was
       * before this function was called.
       * @param pt_state The Lua state.
       * @param n_key The numeric key for the parent table.
       * @param c_data The data to add to the table.
       * @see StartTable()
       * @see EndTable()
       */
      static void AddToTable(lua_State* pt_state,
                             int n_key,
                             const CColor& c_data);

      /**
       * Returns a pointer to the instance to the wanted device.
       * The Lua state is stored in a table called <tt>robot</tt>. Each
       * robot device is stored as a nested table. Each nested table
       * has an <tt>_instance</tt> field which contains the pointer
       * to the actual device. This function returns such pointer.
       * @param pt_state The Lua state.
       * @param str_key The string key for the device table.
       * @return A pointer to the instance to the wanted device.
       */
      template<class T>
      static T* GetDeviceInstance(lua_State* pt_state,
                                  const std::string& str_key) {
         lua_getglobal(pt_state, "robot");
         lua_getfield(pt_state, -1, str_key.c_str());
         lua_getfield(pt_state, -1, "_instance");
         T* ptRetVal = reinterpret_cast<T*>(lua_touserdata(pt_state, -1));
         lua_pop(pt_state, 3);
         return ptRetVal;
      }
      
   private:
      
      static int LOGWrapper(lua_State* pt_state);
      
      static int LOGERRWrapper(lua_State* pt_state);

      static int LoggerWrapper(CARGoSLog& c_log,
                               lua_State* pt_state);

   };

}

#endif
