/**
 * @file <argos3/core/control_interface/ci_proximity_sensor.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef CCI_PROXIMITY_SENSOR_H
#define CCI_PROXIMITY_SENSOR_H

namespace argos {
   class CCI_ProximitySensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_ProximitySensor : virtual public CCI_Sensor {

   public:

      typedef std::vector<Real> TReadings;

   public:

      virtual ~CCI_ProximitySensor() {}

      inline const TReadings& GetReadings() const {
         return m_tReadings;
      }

   protected:

      TReadings m_tReadings;

   };

}

#endif
