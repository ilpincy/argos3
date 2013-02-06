/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_leds_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "footbot_leds_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   CFootBotLEDsActuator::CFootBotLEDsActuator() :
      m_pcLEDEquippedEntity(NULL) {}

   /****************************************/
   /****************************************/

   void CFootBotLEDsActuator::SetEntity(CEntity& c_entity) {
      CSimulatedActuator<CFootBotEntity>::SetEntity(c_entity);
      m_pcLEDEquippedEntity = &(GetEntity().GetLEDEquippedEntity());
   }

   /****************************************/
   /****************************************/

   void CFootBotLEDsActuator::SetAllColors(const CColor& c_color) {
      /* No for loop - much faster than with */
      m_tSettings[0] = c_color;
      m_tSettings[1] = c_color;
      m_tSettings[2] = c_color;
      m_tSettings[3] = c_color;
      m_tSettings[4] = c_color;
      m_tSettings[5] = c_color;
      m_tSettings[6] = c_color;
      m_tSettings[7] = c_color;
      m_tSettings[8] = c_color;
      m_tSettings[9] = c_color;
      m_tSettings[10] = c_color;
      m_tSettings[11] = c_color;
   }

   /****************************************/
   /****************************************/

   void CFootBotLEDsActuator::Update() {
      m_pcLEDEquippedEntity->SetAllLEDsColors(m_tSettings);
   }

   /****************************************/
   /****************************************/

   void CFootBotLEDsActuator::Reset() {
      SetAllColors(CColor::BLACK);
   }

   /****************************************/
   /****************************************/

}

REGISTER_ACTUATOR(CFootBotLEDsActuator,
                  "footbot_leds", "default",
                  "Carlo Pinciroli [ilpincy@gmail.com]",
                  "1.0",
                  "The foot-bot LED actuator.",
                  "This actuator controls the foot-bot LEDs. For a complete description of its\n"
                  "usage, refer to the ci_footbot_leds_actuator.h file.\n\n"
                  "REQUIRED XML CONFIGURATION\n\n"
                  "  <controllers>\n"
                  "    ...\n"
                  "    <my_controller ...>\n"
                  "      ...\n"
                  "      <actuators>\n"
                  "        ...\n"
                  "        <footbot_leds implementation=\"default\" />\n"
                  "        ...\n"
                  "      </actuators>\n"
                  "      ...\n"
                  "    </my_controller>\n"
                  "    ...\n"
                  "  </controllers>\n\n"
                  "OPTIONAL XML CONFIGURATION\n\n"
                  "None.\n",
                  "Usable"
   );

