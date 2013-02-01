
/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_motor_ground_sensor.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "ci_footbot_motor_ground_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   const UInt32       CCI_FootBotMotorGroundSensor::NUM_READINGS = 4;
   const CRange<Real> CCI_FootBotMotorGroundSensor::READING_RANGE(0.0f, 1024.0f);

   /****************************************/
   /****************************************/
    
   CCI_FootBotMotorGroundSensor::CCI_FootBotMotorGroundSensor() :
      m_tReadings(NUM_READINGS) {
      // Set the values for the motor ground sensor offset (taken from the CAD model, in cm)
      m_tReadings[0].Offset.Set( 6.3, 1.16);
      m_tReadings[1].Offset.Set(-6.3, 1.16);
      m_tReadings[2].Offset.Set(-6.3,-1.16);
      m_tReadings[3].Offset.Set( 6.3,-1.16);
   }
    
   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_FootBotMotorGroundSensor::SReading& s_reading) {
      c_os << "Value=<" << s_reading.Value
           << ">, Offset=<" << s_reading.Offset << ">";
      return c_os;
   }

   /****************************************/
   /****************************************/
   
   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_FootBotMotorGroundSensor::TReadings& t_readings) {
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
