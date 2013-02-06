/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_leds_actuator.h>
 *
 * @brief This file provides the definition of
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FOOTBOT_LEDS_ACTUATOR_H
#define FOOTBOT_LEDS_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CFootBotLEDsActuator;
}

#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_leds_actuator.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/core/simulator/actuator.h>
#include <argos3/core/simulator/entity/led_equipped_entity.h>

namespace argos {

   class CFootBotLEDsActuator : public CSimulatedActuator<CFootBotEntity>,
                                public CCI_FootBotLEDsActuator {

   public:

      CFootBotLEDsActuator();

      virtual ~CFootBotLEDsActuator() {}

      virtual void SetEntity(CEntity& c_entity);

      virtual void SetAllColors(const CColor& c_color);

      virtual void Update();
      virtual void Reset();

   private:

      CLEDEquippedEntity* m_pcLEDEquippedEntity;

   };

}

#endif
