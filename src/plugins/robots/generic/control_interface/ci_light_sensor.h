/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_light_sensor.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef CCI_LIGHT_SENSOR_H
#define CCI_LIGHT_SENSOR_H

namespace argos {
   class CCI_LightSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_LightSensor : public CCI_Sensor {

   public:

      virtual ~CCI_LightSensor() {}

      const std::vector<Real>& GetReadings() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      std::vector<Real> m_tReadings;

   };

}

#endif
