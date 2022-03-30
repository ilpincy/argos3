/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_leds_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_leds_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/datatypes/color.h>

#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CBuilderBotLEDsDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      CDirectionalLEDEquippedEntity* pcLEDEquippedEntity = 
         &(c_entity.GetComponent<CDirectionalLEDEquippedEntity>("directional_leds"));
      pcLEDEquippedEntity->Enable();
      CDirectionalLEDEquippedEntity::SInstance::TVector& sInstances =
         pcLEDEquippedEntity->GetInstances();
      if(sInstances.size() != CCI_BuilderBotLEDsActuator::NUM_LEDS) {
         THROW_ARGOSEXCEPTION("Simulator reports an incorrect number of LEDs for the BuilderBot");
      }
      for(UInt32 un_index = 0; un_index < CCI_BuilderBotLEDsActuator::NUM_LEDS; un_index++) {
         m_arrLEDs[un_index] = std::make_pair(&sInstances[un_index].LED, CColor::BLACK);
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotLEDsDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotLEDsActuator::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot LEDs actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/

   void CBuilderBotLEDsDefaultActuator::Update() {
      for(std::pair<CDirectionalLEDEntity*, CColor>& c_led : m_arrLEDs) {
         c_led.first->SetColor(c_led.second);
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotLEDsDefaultActuator::Reset() {
      for(std::pair<CDirectionalLEDEntity*, CColor>& c_led : m_arrLEDs) {
         c_led.second = CColor::BLACK;
      }
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotLEDsDefaultActuator::SetLEDIndex(UInt32 un_index, const CColor& c_color) {
      if(un_index < NUM_LEDS) {
         m_arrLEDs[un_index].second = c_color;
      }
      else {
         LOGERR << "[ERROR] BuilderBot LED index out of range" << std::endl;
      }
   }

   /****************************************/
   /****************************************/

   void CBuilderBotLEDsDefaultActuator::SetLEDs(const CColor& c_color) {
      for(UInt32 un_index = 0; un_index < NUM_LEDS; un_index++) {
         m_arrLEDs[un_index].second = c_color;
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CBuilderBotLEDsDefaultActuator,
                     "builderbot_leds", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "The BuilderBot LEDs actuator.",
                     "The documentation for this actuator is \e]8;;https://github.com/ilpincy/argos3/blob/master/src/plugins/robots/builderbot/README.md#builderbot_leds\aavailable on Github\e]8;;\a.",
                     "Usable"
   );

   /****************************************/
   /****************************************/
   
}
