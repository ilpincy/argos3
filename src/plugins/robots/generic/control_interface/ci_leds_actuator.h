/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#ifndef CCI_LEDS_ACTUATOR_H
#define CCI_LEDS_ACTUATOR_H

namespace argos {
   class CCI_LEDsActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/datatypes/color.h>

namespace argos {

   class CCI_LEDsActuator : public CCI_Actuator {

   public:

      typedef std::vector<CColor> TSettings;

   public:

      CCI_LEDsActuator() {}

      virtual ~CCI_LEDsActuator() {}

      /**
       * @brief Returns the number of LEDs
       */
      size_t GetNumLEDs() const;

      /**
       * @brief Sets the color of a single LED.
       * The method does not impact on the LED intensity: if the intensity was changed,
       * the method updates the color but preserves the intensity previously set.
       *
       * @param un_led_number LED that must be set
       * @param c_color color to set
       */
      virtual void SetSingleColor(UInt32 un_led_number,
                                  const CColor& c_color);
      
      /**
       * @brief Sets the color of the whole LED ring.
       * All the LEDs in the ring will be lit up in the same color.
       * The method does not impact on the LED intensity: if the intensity was changed,
       * the method updates the color but preserves the intensity previously set.
       *
       * @param c_color color to set
       */
      virtual void SetAllColors(const CColor& c_color);

      /**
       * @brief Sets the color of the whole LED ring.
       * All the LEDs in the ring will be lit up, it is possible
       * to have a different color for each LED. Notice that the method
       * also impacts on the intensity of the LED color: it is up to
       * the user to modulate the alpha channel of the color if
       * intensity reduction is needed.
       *
       * @param c_colors color to set for each LED.
       */
      virtual void SetAllColors(const TSettings& c_colors);

      /**
       * @brief Sets the intensity of a single LED in the ring.
       * Intensity is expressed in [0-255].
       *
       * @param un_led_number index of the LED whose intensity must be set.
       * @param un_intensity color to set for each LED.
       */
      virtual void SetSingleIntensity(UInt32 un_led_number,
                                      UInt8 un_intensity);

      /**
       * @brief Sets the intensity of all the LEDs in the ring.
       * Intensity is expressed in [0-255].
       *
       * @param un_intensity color to set for each LED.
       */
      virtual void SetAllIntensities(UInt8 un_intensity);

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      TSettings m_tSettings;

   };

}

#endif
