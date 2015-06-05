/**
 * @file <argos3/plugins/simulator/actuators/leds_default_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "leds_default_actuator.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/plugins/simulator/media/led_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CLEDsDefaultActuator::CLEDsDefaultActuator() :
      m_pcLEDEquippedEntity(NULL) {}

   /****************************************/
   /****************************************/

   void CLEDsDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcLEDEquippedEntity = &(c_entity.GetComponent<CLEDEquippedEntity>("leds"));
      m_tSettings.resize(m_pcLEDEquippedEntity->GetLEDs().size());
   }

   /****************************************/
   /****************************************/

   void CLEDsDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_LEDsActuator::Init(t_tree);
         std::string strMedium;
         GetNodeAttribute(t_tree, "medium", strMedium);
         m_pcLEDMedium = &CSimulator::GetInstance().GetMedium<CLEDMedium>(strMedium);
         m_pcLEDEquippedEntity->AddToMedium(*m_pcLEDMedium);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the LEDs default actuator", ex);
      }
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

   void CLEDsDefaultActuator::Destroy() {
      m_pcLEDEquippedEntity->RemoveFromMedium();
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
                  "        <leds implementation=\"default\"\n"
                  "              medium=\"leds\" />\n"
                  "        ...\n"
                  "      </actuators>\n"
                  "      ...\n"
                  "    </my_controller>\n"
                  "    ...\n"
                  "  </controllers>\n\n"
                  "The 'medium' attribute sets the id of the LED medium declared in the <media>\n"
                  "XML section.\n\n"
                  "OPTIONAL XML CONFIGURATION\n\n"
                  "None.\n",
                  "Usable"
   );

