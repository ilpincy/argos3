/**
 * @file <argos3/core/control_interface/ci_sensor.h>
 *
 * @brief This file provides the basic interface for all actuators.
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

   class CCI_Sensor : public CBaseConfigurableResource {

   public:

      typedef std::map<std::string, CCI_Sensor*, std::less<std::string> > TMap;

   public:

      virtual ~CCI_Sensor() {}

      virtual void Init(TConfigurationNode& t_node) {}

      virtual void Reset() {}

      virtual void Destroy() {}

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaVariables(lua_State* pt_lua_state) = 0;

      virtual void ReadingsToLuaVariables(lua_State* pt_lua_state) = 0;
#endif

   };

}

#endif
