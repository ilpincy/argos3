/**
 * @file argos3/core/simulator/simulated_sensor.h
 *
 * @brief This file provides the definition of
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef SENSOR_H
#define SENSOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CSensor;
   class CEntity;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <string>
#include <map>

namespace argos {

   class CSensor : virtual public CCI_Sensor {

   public:

      virtual ~CSensor() = 0;
      virtual CEntity& GetEntity() = 0;
      virtual void SetEntity(CEntity& c_entity) = 0;
      virtual void Update() = 0;
   };

}

#endif
