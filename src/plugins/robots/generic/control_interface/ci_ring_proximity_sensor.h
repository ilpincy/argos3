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

#include <argos3/plugins/robots/generic/control_interface/ci_proximity_sensor.h>
#include <argos3/core/utility/math/angles.h>

namespace argos {

   class CCI_RingProximitySensor : virtual public CCI_ProximitySensor {

   public:

      virtual ~CCI_RingProximitySensor() {}

      inline const std::vector<CRadians>& GetAngles() const {
         return m_tAngles;
      }

   protected:

      std::vector<CRadians> m_tAngles;

   };

}

#endif
