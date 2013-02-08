/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_leds_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "leds_default_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   CLEDsDefaultActuator::CLEDsDefaultActuator() :
      m_pcLEDEquippedEntity(NULL) {}

   /****************************************/
   /****************************************/

   void CLEDsDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcLEDEquippedEntity = &(c_entity.GetComponent<CLEDEquippedEntity>("leds"));
      m_tSettings.resize(m_pcLEDEquippedEntity->GetAllLEDs().size());
   }

   /****************************************/
   /****************************************/

   void CLEDsDefaultActuator::Update() {
      m_pcLEDEquippedEntity->SetAllLEDsColors(m_tSettings);
   }

   /****************************************/
   /****************************************/

   void CLEDsDefaultActuator::Reset() {
      SetAllColors(CColor::BLACK);
   }

   /****************************************/
   /****************************************/

}

REGISTER_ACTUATOR(CLEDsDefaultActuator,
                  "leds", "default",
                  "Carlo Pinciroli [ilpincy@gmail.com]",
                  "1.0",
                  "The LEDs actuator.",
                  "This actuator controls a group of LEDs. For a complete description of its\n"
                  "usage, refer to the ci_leds_actuator.h file.\n\n"
                  "REQUIRED XML CONFIGURATION\n\n"
                  "  <controllers>\n"
                  "    ...\n"
                  "    <my_controller ...>\n"
                  "      ...\n"
                  "      <actuators>\n"
                  "        ...\n"
                  "        <leds implementation=\"default\" />\n"
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

