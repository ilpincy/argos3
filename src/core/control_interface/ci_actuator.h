/**
 * @file <argos3/core/control_interface/ci_actuator.h>
 *
 * @brief This file provides the basic interface for all actuators.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CI_ACTUATOR_H
#define CI_ACTUATOR_H

namespace argos {
   class CCI_Actuator;
}

#include <argos3/core/config.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/configuration/base_configurable_resource.h>
#include <map>

#ifdef ARGOS_WITH_LUA
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#endif

namespace argos {

   /**
    * The basic interface for all actuators.
    */
   class CCI_Actuator : public CBaseConfigurableResource {

   public:

      typedef std::map<std::string, CCI_Actuator*, std::less<std::string> > TMap;

   public:

      /**
       * Class destructor.
       */
      virtual ~CCI_Actuator() {}

      /**
       * Initializes the actuator from the XML configuration tree.
       * The default implementation of this method does nothing.
       * @param t_node The XML configuration tree relative to this actuator.
       * @see Reset()
       * @see Destroy()
       */
      virtual void Init(TConfigurationNode& t_node) {}

      /**
       * Resets the actuator to the state it had just after Init().
       * The default implementation of this method does nothing.
       * @see Init()
       * @see Destroy()
       */
      virtual void Reset() {}

      /**
       * Destroys the actuator.
       * This method is supposed to undo whatever was done in Init().
       * The default implementation of this method does nothing.
       * @see Init()
       * @see Reset()
       */
      virtual void Destroy() {}

#ifdef ARGOS_WITH_LUA
      /**
       * Creates the Lua state for this actuator.
       * The state must be added to the <tt>robot</tt> table, as a nested table.
       * @param pt_lua_state The current Lua state.
       * @see LuaStateToSettings()
       */
      virtual void CreateLuaState(lua_State* pt_lua_state) = 0;
#endif
   };

}

#endif
