/**
 * @file <argos3/plugins/robots/generic/simulator/directional_leds_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "directional_leds_default_actuator.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/simulator/media/directional_led_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDirectionalLEDsDefaultActuator::CDirectionalLEDsDefaultActuator() :
      m_pcDirectionalLEDEquippedEntity(nullptr) {}

   /****************************************/
   /****************************************/

   void CDirectionalLEDsDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcDirectionalLEDEquippedEntity = &(c_entity.GetComponent<CDirectionalLEDEquippedEntity>("directional_leds"));
      m_tSettings.resize(m_pcDirectionalLEDEquippedEntity->GetInstances().size());
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDsDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_LEDsActuator::Init(t_tree);
         std::string strMedium;
         GetNodeAttribute(t_tree, "medium", strMedium);
         m_pcDirectionalLEDMedium = &CSimulator::GetInstance().GetMedium<CDirectionalLEDMedium>(strMedium);
         m_pcDirectionalLEDEquippedEntity->AddToMedium(*m_pcDirectionalLEDMedium);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing the directional LEDs default actuator", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDsDefaultActuator::Update() {
      m_pcDirectionalLEDEquippedEntity->SetLEDColors(m_tSettings);
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDsDefaultActuator::Reset() {
      SetAllColors(CColor::BLACK);
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDsDefaultActuator::Destroy() {
      m_pcDirectionalLEDEquippedEntity->RemoveFromMedium();
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CDirectionalLEDsDefaultActuator,
                     "directional_leds", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "The directional LEDs actuator.",
                     "This actuator controls a group of directional LEDs. For a complete description\n"
                     "of its usage, refer to the ci_leds_actuator.h file.\n\n"
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

   /****************************************/
   /****************************************/

}
