/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_light_sensor.cpp>
 *
 * @brief This file provides the implementation of the footbot light sensor.
 *
 * This file provides the definition of the footbot light sensor.
 * The sensors are evenly spaced on a ring around the base of the robot.
 * Therefore, they do not turn with the turret. The readings are in the
 * following order (seeing the robot from TOP, battery socket is the BACK):
 *
 *              front
 *
 *              0 23
 *            1     22
 *          2         21
 *        3             20      r
 * l    4                 19    i
 * e  5                     18  g
 * f  6                     17  h
 * t    7                 16    t
 *        8             15
 *          9         14
 *            10     13
 *              11 12
 *
 *              back
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_footbot_light_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   const UInt32 CCI_FootBotLightSensor::NUM_READINGS = 24;
   const CRange<Real> CCI_FootBotLightSensor::READING_RANGE(0.0,4096.0);

   /****************************************/
   /****************************************/

   CCI_FootBotLightSensor::CCI_FootBotLightSensor() :
      m_tReadings(NUM_READINGS) {
      // Set the values for the light sensor angles
      CRadians cSensorSpacing = CRadians::TWO_PI / CCI_FootBotLightSensor::NUM_READINGS;
      CRadians cSensorHalfSpacing = cSensorSpacing / 2.0f;
      for(UInt32 i = 0; i < CCI_FootBotLightSensor::NUM_READINGS; ++i) {
         m_tReadings[i].Angle = cSensorHalfSpacing + i * cSensorSpacing;
         m_tReadings[i].Angle.SignedNormalize();
      }
   }

   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_FootBotLightSensor::SReading& s_reading) {
      c_os << "Value=<" << s_reading.Value
           << ">, Angle=<" << s_reading.Angle << ">";
      return c_os;
   }

   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_FootBotLightSensor::TReadings& t_readings) {
      if(! t_readings.empty()) {
         c_os << "{ " << t_readings[0].Value << " }";
         for(UInt32 i = 1; i < t_readings.size(); ++i) {
            c_os << " { " << t_readings[0].Value << " }";
         }
         c_os << std::endl;
      }
      return c_os;
   }

   /****************************************/
   /****************************************/

}
