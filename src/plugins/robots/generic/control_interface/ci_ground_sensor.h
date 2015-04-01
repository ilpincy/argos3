/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_ground_sensor.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef CCI_GROUND_SENSOR_H
#define CCI_GROUND_SENSOR_H

namespace argos {
   class CCI_GroundSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_GroundSensor : public CCI_Sensor {

   public:

      virtual ~CCI_GroundSensor() {}

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
