/**
 * @file <argos3/core/control_interface/ci_sensor.h>
 *
 * @brief This file provides the basic interface for all sensors.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CI_SENSOR_H
#define CI_SENSOR_H

namespace argos {
   class CCI_Sensor;
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
    * The basic interface for all sensors.
    */
   class CCI_Sensor : public CBaseConfigurableResource {

   public:

      typedef std::map<std::string, CCI_Sensor*, std::less<std::string> > TMap;

   public:

      /**
       * Class destructor.
       */
      virtual ~CCI_Sensor() {}

      /**
       * Initializes the sensor from the XML configuration tree.
       * The default implementation of this method does nothing.
       * @param t_node The XML configuration tree relative to this sensor.
       * @see Reset()
       * @see Destroy()
       */
      virtual void Init(TConfigurationNode& t_node) {}

      /**
       * Resets the sensor to the state it had just after Init().
       * The default implementation of this method does nothing.
       * @see Init()
       * @see Destroy()
       */
      virtual void Reset() {}

      /**
       * Destroys the sensor.
       * This method is supposed to undo whatever was done in Init().
       * The default implementation of this method does nothing.
       * @see Init()
       * @see Reset()
       */
      virtual void Destroy() {}

#ifdef ARGOS_WITH_LUA
      /**
       * Creates the Lua state for this sensor.
       * The state must be added to the <tt>robot</tt> table, as a nested table.
       * @param pt_lua_state The current Lua state.
       * @see ReadingsToLuaState()
       */
      virtual void CreateLuaState(lua_State* pt_lua_state) = 0;

      /**
       * Writes the current sensor readings into the Lua state.
       * @param pt_lua_state The current Lua state.
       * @see CreateLuaState()
       */
      virtual void ReadingsToLuaState(lua_State* pt_lua_state) = 0;
#endif

   };

}

#endif
