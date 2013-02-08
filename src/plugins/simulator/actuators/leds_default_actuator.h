/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_leds_actuator.h>
 *
 * @brief This file provides the definition of
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef LEDS_DEFAULT_ACTUATOR_H
#define LEDS_DEFAULT_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CLEDsDefaultActuator;
}

#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/core/simulator/actuator.h>

namespace argos {

   class CLEDsDefaultActuator : public CSimulatedActuator,
                                public CCI_LEDsActuator {

   public:

      CLEDsDefaultActuator();

      virtual ~CLEDsDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Update();
      virtual void Reset();

   private:

      CLEDEquippedEntity* m_pcLEDEquippedEntity;

   };

}

#endif
