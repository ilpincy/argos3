/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_footbot_proximity_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   const UInt32 CCI_FootBotProximitySensor::NUM_READINGS = 24;
   const CRange<Real> CCI_FootBotProximitySensor::READING_RANGE(0.0f, 4096.0f);

   /****************************************/
   /****************************************/

   CCI_FootBotProximitySensor::CCI_FootBotProximitySensor() :
      m_tReadings(NUM_READINGS) {
      // Set the values for the proximity sensor angles
      CRadians cSensorSpacing = CRadians::TWO_PI / NUM_READINGS;
      CRadians cSensorHalfSpacing = cSensorSpacing / 2.0f;
      for(UInt32 i = 0; i < NUM_READINGS; ++i) {
         m_tReadings[i].Angle = cSensorHalfSpacing + i * cSensorSpacing;
         m_tReadings[i].Angle.SignedNormalize();
      }
   }

   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_FootBotProximitySensor::SReading& s_reading) {
      c_os << "Value=<" << s_reading.Value
           << ">, Angle=<" << s_reading.Angle << ">";
      return c_os;
   }

   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_FootBotProximitySensor::TReadings& t_readings) {
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
