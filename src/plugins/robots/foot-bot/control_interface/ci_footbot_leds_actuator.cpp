/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_leds_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_footbot_leds_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/

   const UInt32 CCI_FootBotLEDsActuator::NUM_LEDS = 12;

   /****************************************/
   /****************************************/

   CCI_FootBotLEDsActuator::CCI_FootBotLEDsActuator() :
      CCI_LEDsActuator(NUM_LEDS) {}

   /****************************************/
   /****************************************/

}
