/**
 * @file argos3/core/simulator/simulated_actuator.h
 *
 * @brief This file provides the definition of
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef SIMULATED_ACTUATOR_H
#define SIMULATED_ACTUATOR_H

namespace argos {
   class CSimulatedActuator;
   class CEntity;
}

#include <argos3/core/control_interface/ci_actuator.h>

#include <string>
#include <map>

namespace argos {

   class CSimulatedActuator : virtual public CCI_Actuator {

   public:

      virtual ~CSimulatedActuator() {}

      virtual CEntity& GetEntity() = 0;
      virtual void SetEntity(CEntity& c_entity) = 0;

      virtual void Update() = 0;
      virtual void Reset() = 0;

   };

}

#endif
