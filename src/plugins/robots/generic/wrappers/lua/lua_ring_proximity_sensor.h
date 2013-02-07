
/**
 * @file <argos3/core/wrappers/lua/lua_ring_proximity_sensor.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef LUA_RING_PROXIMITY_SENSOR_H
#define LUA_RING_PROXIMITY_SENSOR_H

namespace argos {
   class CLuaRingProximitySensor;
}

#include <argos3/core/wrappers/lua/lua_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_ring_proximity_sensor.h>

namespace argos {

   class CLuaRingProximitySensor : public CLuaSensor<CCI_RingProximitySensor> {

   public:

      CLuaRingProximitySensor(CCI_RingProximitySensor& c_sensor,
                              lua_State* pt_lua_state,
                              const std::string& str_var_name) :
         CLuaSensor<CCI_RingProximitySensor>(c_sensor,
                                             pt_lua_state,
                                             str_var_name) {}

      virtual ~CLuaRingProximitySensor() {}

      virtual void CreateLuaVariables();

      virtual void ReadingsToLuaVariables();

   };

}

#endif
