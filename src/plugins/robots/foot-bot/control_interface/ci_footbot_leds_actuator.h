/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_leds_actuator.h>
 *
 * @brief This file provides the definition of footbot LEDs actuator.
 *
 * This file provides the definition of footbot LEDs actuator.
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
 * The G depicts the gripper. Please notice that in ASEBA the LEDs indexing
 * is opposite to the one here indicated.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CCI_FOOTBOT_LEDS_ACTUATOR_H
#define CCI_FOOTBOT_LEDS_ACTUATOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_FootBotLEDsActuator;
}

#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
#include <argos3/core/utility/datatypes/color.h>

namespace argos {

   class CCI_FootBotLEDsActuator : public CCI_LEDsActuator {

   public:

      static const UInt32 NUM_LEDS;

   public:

      CCI_FootBotLEDsActuator();

      virtual ~CCI_FootBotLEDsActuator() {}

   };

}

#endif
