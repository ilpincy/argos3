/**
 * @file core/simulator/actuator.h
 *
 * @brief This file provides the definition of
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H

namespace argos {
   class CActuator;
   class CEntity;
}

#include <argos3/core/control_interface/ci_actuator.h>

#include <string>
#include <map>

namespace argos {

   class CActuator : virtual public CCI_Actuator {

   public:

      virtual ~CActuator() {}
      virtual CEntity& GetEntity() = 0;
      virtual void SetEntity(CEntity& c_entity) = 0;
      virtual void Update() = 0;

   };

}

#endif
