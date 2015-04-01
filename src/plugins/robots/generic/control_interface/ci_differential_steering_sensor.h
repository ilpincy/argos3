/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_differential_steering_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CCI_DIFFERENTIAL_STEERING_SENSOR_H
#define CCI_DIFFERENTIAL_STEERING_SENSOR_H

namespace argos {
   class CCI_DifferentialSteeringSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_DifferentialSteeringSensor : public CCI_Sensor {

   public:

      struct SReading {
         Real VelocityLeftWheel;
         Real VelocityRightWheel;
         Real CoveredDistanceLeftWheel;
         Real CoveredDistanceRightWheel;
         Real WheelAxisLength;
         
         SReading() :
            VelocityLeftWheel(0.0f),
            VelocityRightWheel(0.0f),
            CoveredDistanceLeftWheel(0.0f),
            CoveredDistanceRightWheel(0.0f),
            WheelAxisLength(0.0f) {}
         
         SReading(Real f_velocity_left_wheel,
                  Real f_velocity_right_wheel,
                  Real f_covered_distance_left_wheel,
                  Real f_covered_distance_right_wheel,
                  Real f_wheel_axis_length) :
            VelocityLeftWheel(f_velocity_left_wheel),
            VelocityRightWheel(f_velocity_right_wheel),
            CoveredDistanceLeftWheel(f_covered_distance_left_wheel),
            CoveredDistanceRightWheel(f_covered_distance_right_wheel),
            WheelAxisLength(f_wheel_axis_length) {
         }

      };

      /**
       * Constructor
       */
      CCI_DifferentialSteeringSensor() {}

      /**
       * Destructor
       */
      virtual ~CCI_DifferentialSteeringSensor() {}

      /**
       * @brief Returns the reading of the encoder sensor
       * Returns the reading of the encoder sensor
       */
      const SReading& GetReading() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      SReading m_sReading;
   };

}

#endif
