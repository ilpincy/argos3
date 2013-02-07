
/**
 * @file <argos3/core/control_interface/ci_ring_proximity_sensor.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef CCI_RING_PROXIMITY_SENSOR_H
#define CCI_RING_PROXIMITY_SENSOR_H

namespace argos {
   class CCI_RingProximitySensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/angles.h>

namespace argos {

   class CCI_RingProximitySensor : virtual public CCI_Sensor {

   public:

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

   public:

      CCI_RingProximitySensor(size_t un_num_sensors);
      virtual ~CCI_RingProximitySensor() {}

      inline const TReadings& GetReadings() const {
         return m_tReadings;
      }

   protected:

      TReadings m_tReadings;

   };

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_RingProximitySensor::SReading& s_reading);
   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_RingProximitySensor::TReadings& t_readings);
}

#endif
