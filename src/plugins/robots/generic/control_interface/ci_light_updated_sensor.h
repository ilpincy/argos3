/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_light_updated_sensor.h>
 *
 * @author Danesh Tarapore <daneshtarapore@gmail.com>
 */
#ifndef CCI_LIGHTUPDATED_SENSOR_H
#define CCI_LIGHTUPDATED_SENSOR_H

namespace argos {
   class CCI_LightUpdatedSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/angles.h>
#include <vector>

namespace argos {

   class CCI_LightUpdatedSensor : public CCI_Sensor {

   public:

      /**
       * The DTO of the light sensor. It contains the reading of each sensor and
       * and the angle at which each sensor is placed.
       */
      struct SReading {
         Real Value;
         CRadians Angle;

         SReading() :
            Value(0.0f) {}

         SReading(Real f_value,
                  const CRadians& c_angle) :
            Value(f_value),
            Angle(c_angle) {}
      };

      typedef std::vector<SReading> TReadings;

      CCI_LightUpdatedSensor();
      virtual ~CCI_LightUpdatedSensor() {}

      const TReadings& GetReadings() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      TReadings m_tReadings;

   };

   std::ostream& operator<<(std::ostream& c_os, const CCI_LightUpdatedSensor::SReading& s_reading);
   std::ostream& operator<<(std::ostream& c_os, const CCI_LightUpdatedSensor::TReadings& t_readings);

}

#endif
