/**
 * @file <argos3/plugins/robots/pi-puck/hardware/pipuck_leds_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pipuck_leds_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/plugins/robots/pi-puck/simulator/pipuck_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CPiPuckLEDsDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      CDirectionalLEDEquippedEntity* pcLEDEquippedEntity = 
         &(c_entity.GetComponent<CDirectionalLEDEquippedEntity>("directional_leds"));
      pcLEDEquippedEntity->Enable();
      CDirectionalLEDEquippedEntity::SInstance::TVector& sInstances =
         pcLEDEquippedEntity->GetInstances();
      if(sInstances.size() != CCI_PiPuckLEDsActuator::NUM_LEDS) {
         THROW_ARGOSEXCEPTION("Simulator reports an incorrect number of LEDs for the Pi-Puck");
      }
      for(UInt32 un_index = 0; un_index < CCI_PiPuckLEDsActuator::NUM_LEDS; un_index++) {
         m_arrLEDs[un_index] = std::make_pair(&sInstances[un_index].LED, false);
      }
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckLEDsDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PiPuckLEDsActuator::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the Pi-Puck leds actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/

   void CPiPuckLEDsDefaultActuator::Update() {
      for(std::pair<CDirectionalLEDEntity*, bool>& c_led : m_arrLEDs) {
         c_led.first->SetColor(c_led.second ? CColor::WHITE : CColor::BLACK);
      }
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckLEDsDefaultActuator::Reset() {
      for(std::pair<CDirectionalLEDEntity*, bool>& c_led : m_arrLEDs) {
         c_led.second = false;
      }
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckLEDsDefaultActuator::SetRingLEDIndex(UInt32 un_index, bool b_on) {
      if(un_index < NUM_RING_LEDS) {
         m_arrLEDs[un_index].second = b_on;
      }
      else {
         LOGERR << "[ERROR] Pi-Puck ring LED index out of range" << std::endl;
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckLEDsDefaultActuator::SetRingLEDs(bool b_on) {
      for(UInt32 un_index = 0; un_index < NUM_RING_LEDS; un_index++) {
         m_arrLEDs[un_index].second = b_on;
      }
   }

   /****************************************/
   /****************************************/

   void CPiPuckLEDsDefaultActuator::SetFrontLED(bool b_on) {
      m_arrLEDs[INDEX_FRONT_LED].second = b_on;
   }

   /****************************************/
   /****************************************/

   void CPiPuckLEDsDefaultActuator::SetBodyLED(bool b_on) {
      m_arrLEDs[INDEX_BODY_LED].second = b_on;
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CPiPuckLEDsDefaultActuator,
                     "pipuck_leds", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "The Pi-Puck LEDs actuator.",
                     "The documentation for this actuator is \e]8;;https://github.com/ilpincy/argos3/blob/master/src/plugins/robots/pi-puck/README.md#pipuck_leds\aavailable on Github\e]8;;\a.",
                     "Usable"
   );

   /****************************************/
   /****************************************/
   
}
