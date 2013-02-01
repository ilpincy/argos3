/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_wheel_speed_sensor.h>
 *
 * @brief This file provides the definition of the wheel speed sensor.
 *
 * This file provides the definition of the wheel speed sensor.
 * This sensor returns the current actual speed of the wheels as measured by
 * the sensors present on the robot. The return value is expressed in
 * cm/s and correspond to a linear velocity. For a more detailed explanation
 * of these concepts, see the footbot wheels actuator.
 *
 * @author Carlo Pinciroli - <cpinciro@iridia.ulb.ac.be>
 */

#ifndef CCI_FOOTBOT_WHEEL_SPEED_SENSOR_H
#define CCI_FOOTBOT_WHEEL_SPEED_SENSOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_FootBotWheelSpeedSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_FootBotWheelSpeedSensor : virtual public CCI_Sensor {

   public:

      struct SReading {
         Real LinearVelocityLeft;
         Real LinearVelocityRight;

         SReading() :
            LinearVelocityLeft(0.0f),
            LinearVelocityRight(0.0f) {
         }

         SReading(Real f_linear_velocity_left,
                  Real f_linear_velocity_right) :
            LinearVelocityLeft(f_linear_velocity_left),
            LinearVelocityRight(f_linear_velocity_right) {
         }
      };

   public:

      virtual ~CCI_FootBotWheelSpeedSensor() {}

      inline const TReading& GetReading() const {
         return m_tReading;
      }

   protected:

      TReading m_tReading;
   };
}

#endif
