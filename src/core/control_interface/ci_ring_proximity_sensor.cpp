#include "ci_ring_proximity_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   CCI_RingProximitySensor::CCI_RingProximitySensor(size_t un_num_sensors) :
      m_tReadings(un_num_sensors) {
      // Set the values for the proximity sensor angles
      CRadians cSensorSpacing = CRadians::TWO_PI / un_num_sensors;
      CRadians cSensorHalfSpacing = cSensorSpacing / 2.0f;
      for(UInt32 i = 0; i < un_num_sensors; ++i) {
         m_tReadings[i].Angle = cSensorHalfSpacing + i * cSensorSpacing;
         m_tReadings[i].Angle.SignedNormalize();
      }
   }

   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_RingProximitySensor::SReading& s_reading) {
      c_os << "Value=<" << s_reading.Value
           << ">, Angle=<" << s_reading.Angle << ">";
      return c_os;
   }

   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_RingProximitySensor::TReadings& t_readings) {
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
