/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_accelerometer_sensor.h>
 *
 * @brief This file provides the common interface definition of the footbot accelerometer.
 *
 * @author Nithin MATHEWS - <nmathews@ulb.ac.be>
 */

#ifndef CCI_FOOTBOT_ACCELEROMETER_SENSOR_H
#define CCI_FOOTBOT_ACCELEROMETER_SENSOR_H

namespace argos {
   class CCI_FootBotAccelerometerSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector2.h>

namespace argos {

   class CCI_FootBotAccelerometerSensor : virtual public CCI_Sensor {

   public:

      struct SReading {
         SInt16 AxisX;
         SInt16 AxisY;
         SInt16 AxisZ;

         SReading() :
        	 AxisX(0),
        	 AxisY(0),
        	 AxisZ(0){
         }

         SReading(SInt16 n_value_x_axis,
                  SInt16 n_value_y_axis,
                  SInt16 n_value_z_axis) :
        	 AxisX(n_value_x_axis),
        	 AxisY(n_value_y_axis),
        	 AxisZ(n_value_z_axis){
         }
      };

   public:

      CCI_FootBotAccelerometerSensor() {}

      virtual ~CCI_FootBotAccelerometerSensor() {}

      inline const SReading& GetReading() const {
         return m_sReading;
      }

   protected:

      SReading m_sReading;

   };

}

#endif
