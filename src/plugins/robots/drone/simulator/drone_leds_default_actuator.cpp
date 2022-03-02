/**
 * @file <argos3/plugins/robots/drone/hardware/drone_leds_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "drone_leds_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/datatypes/color.h>

#include <argos3/plugins/robots/drone/simulator/drone_entity.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CDroneLEDsDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      CDirectionalLEDEquippedEntity* pcLEDEquippedEntity = 
         &(c_entity.GetComponent<CDirectionalLEDEquippedEntity>("directional_leds"));
      pcLEDEquippedEntity->Enable();
      CDirectionalLEDEquippedEntity::SInstance::TVector& sInstances =
         pcLEDEquippedEntity->GetInstances();
      if(sInstances.size() != CCI_DroneLEDsActuator::NUM_LEDS) {
         THROW_ARGOSEXCEPTION("Simulator reports an incorrect number of LEDs for the drone");
      }
      for(UInt32 un_index = 0; un_index < CCI_DroneLEDsActuator::NUM_LEDS; un_index++) {
         m_arrLEDs[un_index] = std::make_pair(&sInstances[un_index].LED, CColor::BLACK);
      }
   }

   /****************************************/
   /****************************************/
   
   void CDroneLEDsDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_DroneLEDsActuator::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the drone leds actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/

   void CDroneLEDsDefaultActuator::Update() {
      for(std::pair<CDirectionalLEDEntity*, CColor>& c_led : m_arrLEDs) {
         c_led.first->SetColor(c_led.second);
      }
   }

   /****************************************/
   /****************************************/
   
   void CDroneLEDsDefaultActuator::Reset() {
      for(std::pair<CDirectionalLEDEntity*, CColor>& c_led : m_arrLEDs) {
         c_led.second = CColor::BLACK;
      }
   }

   /****************************************/
   /****************************************/
   
   void CDroneLEDsDefaultActuator::SetLEDIndex(UInt32 un_index, const CColor& c_color) {
      if(un_index < NUM_LEDS) {
         m_arrLEDs[un_index].second = c_color;
      }
      else {
         LOGERR << "[ERROR] drone LED index out of range" << std::endl;
      }
   }

   /****************************************/
   /****************************************/

   void CDroneLEDsDefaultActuator::SetLEDs(const CColor& c_color) {
      for(UInt32 un_index = 0; un_index < NUM_LEDS; un_index++) {
         m_arrLEDs[un_index].second = c_color;
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CDroneLEDsDefaultActuator,
                     "drone_leds", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "The drone LEDs actuator.",
                     "The documentation for this actuator is \e]8;;https://github.com/ilpincy/argos3/blob/master/src/plugins/robots/drone/README.md#drone_leds\aavailable on Github\e]8;;\a.",
                     "Usable"
   );

   /****************************************/
   /****************************************/
   
}
