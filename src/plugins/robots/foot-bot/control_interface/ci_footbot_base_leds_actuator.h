/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_base_leds_actuator.h>
 *
 * @brief This file provides the definition of footbot LEDs actuator.
 *
 * This file provides the definition of footbot base LEDs actuator.
 *
 * The base LEDs consist of several LEDs which are scattered over the
 * base of the footbot. In detail, they consist of:
 *
 *  1. RGB LED on the ceiling camera (SetCameraLEDColor)
 *  2. REB LED on the IMX board (SetImxLEDColor)
 *  3. 4 red debugging LEDs (SetDebugLEDs)
 *  4. blue LED for Wifi status display (SetWifiStatusLED)
 *  5. orange LED for Wifi transmit display (SetWifiTransmitLED)
 *
 * The user can control the color and the intensity of the first two LEDs.
 * The other LEDs (3-5) can only be switched on or off.
 *
 * The main use of this actuator is to lower the amount of visual noise
 * in an experiment or for debugging.
 *
 * @author Arne Brutschy - <arne.brutschy@ulb.ac.be>
 */

#ifndef CCI_FOOTBOT_BASE_LEDS_ACTUATOR_H
#define CCI_FOOTBOT_BASE_LEDS_ACTUATOR_H

/* To avoid dependency problems when including */
namespace argos {
    class CCI_FootBotBaseLEDsActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/datatypes/color.h>

namespace argos {
   
   class CCI_FootBotBaseLEDsActuator : virtual public CCI_Actuator {
      
   public:
      
      static const UInt32 NUM_LEDS;
      
   public:
      
      CCI_FootBotBaseLEDsActuator() {}
      
      virtual ~CCI_FootBotBaseLEDsActuator() {}
      
      virtual void SetCameraLEDColor(const CColor& c_color) = 0;

      virtual void SetImxLEDColor(const CColor& c_color) = 0;

      virtual void SetDebugLEDs(UInt32 un_led_number,
                                bool b_state) = 0;

      virtual void SetWifiStatusLED(bool b_state) = 0;

      virtual void SetWifiTransmitLED(bool b_state) = 0;
      
   };
   
}

#endif
