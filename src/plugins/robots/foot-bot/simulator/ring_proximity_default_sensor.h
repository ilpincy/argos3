/**
 * @file <argos3/plugins/simulator/sensors/ring_proximity_default_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef RING_PROXIMITY_DEFAULT_SENSOR_H
#define RING_PROXIMITY_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CRingProximityDefaultSensor;
}

#include <argos3/plugins/robots/generic/control_interface/ci_ring_proximity_sensor.h>
#include <argos3/plugins/simulator/sensors/proximity_default_sensor.h>

namespace argos {

   class CRingProximityDefaultSensor : public CCI_RingProximitySensor,
                                       public CProximityDefaultSensor {

   public:

      virtual ~CRingProximityDefaultSensor() {}

      virtual void Init(TConfigurationNode& t_tree);

   };

}

#endif
