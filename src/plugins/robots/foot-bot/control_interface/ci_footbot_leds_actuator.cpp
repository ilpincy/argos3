/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_leds_actuator.cpp>
 *
 * @brief This file provides the implementation of footbot LEDs actuator.
 *
 * This file provides the implementation of footbot LEDs actuator.
 *
 * The user can control the color and the intensity of each LED.
 * Intensity s stored in the alpha channel of the color. If the intensity
 * of the (one of the) LEDs is changed, its value is kept in memory
 * also when the color is changed (i.e SetColor(s) methods do not change
 * the intensity).
 * The method that takes a TLEDSettings type as parameter, on the other hand,
 * also changes the intensity of the LEDs. It is up to the user to provide
 * a vector of CColor with the desired intensity if the method is called.
 *
 * The LEDs are distributed evenly around the robot, in the same module
 * as the gripper. When the gripper rotates, so do the LEDs. The total
 * number of LEDs is 12 and they are distributed as follows:
 *
 *       00 G 11
 *     01       10
 *   02           09
 *   03           08
 *     04       07
 *        05 06
 *
 * The G depicts the gripper.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Giovanni Pini   - <gpini@iridia.ulb.ac.be>
 * @author Arne Brutschy - <arne.brutschy@ulb.ac.be>
 */

#include "ci_footbot_leds_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/

   const UInt32 CCI_FootBotLEDsActuator::NUM_LEDS = 12;

   /****************************************/
   /****************************************/

   CCI_FootBotLEDsActuator::CCI_FootBotLEDsActuator() {
      for(UInt32 i = 0; i < NUM_LEDS; ++i) {
         m_tLEDSettings.push_back(CColor::BLACK);
      }
   }

   /****************************************/
   /****************************************/

   void CCI_FootBotLEDsActuator::SetSingleIntensity(UInt32 un_led_number,
                                                    UInt32 un_intensity) {
      m_tLEDSettings[un_led_number].SetAlpha(un_intensity);
      SetSingleColor(un_led_number, m_tLEDSettings[un_led_number]);
   }

   /****************************************/
   /****************************************/

   void CCI_FootBotLEDsActuator::SetAllIntensities(UInt32 un_intensity) {
      for (UInt32 i = 0; i < NUM_LEDS; ++i) {
         m_tLEDSettings[i].SetAlpha(un_intensity);
      }
      SetAllColors(m_tLEDSettings);
   }

   /****************************************/
   /****************************************/

}
