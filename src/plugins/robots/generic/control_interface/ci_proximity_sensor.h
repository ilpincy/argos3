/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_proximity_sensor.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef CCI_PROXIMITY_SENSOR_H
#define CCI_PROXIMITY_SENSOR_H

namespace argos {
   class CCI_ProximitySensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_ProximitySensor : public CCI_Sensor {

   public:

      virtual ~CCI_ProximitySensor() {}

      inline const std::vector<Real>& GetReadings() const {
         return m_tReadings;
      }

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaVariables(lua_State* pt_lua_state);

      virtual void ReadingsToLuaVariables(lua_State* pt_lua_state);
#endif

   protected:

      std::vector<Real> m_tReadings;

   };

}

#endif
