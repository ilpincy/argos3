/**
 * @file argos3/core/simulator/simulated_sensor.h
 *
 * @brief This file provides the definition of
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef SIMULATED_SENSOR_H
#define SIMULATED_SENSOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CSimulatedSensor;
   class CEntity;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <string>
#include <map>

namespace argos {

   class CSimulatedSensor {

   public:

      virtual ~CSimulatedSensor() {}

      virtual CEntity& GetEntity() = 0;
      virtual void SetEntity(CEntity& c_entity) = 0;

      virtual void Update() = 0;
      virtual void Reset() = 0;

      virtual void Destroy() {}

   };

}

#endif
