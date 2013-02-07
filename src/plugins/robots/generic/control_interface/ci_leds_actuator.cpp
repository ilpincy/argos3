
/**
 * @file <argos3/core/control_interface/ci_leds_actuator.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "ci_leds_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/

   CCI_LEDsActuator::CCI_LEDsActuator(size_t un_num_leds) :
      m_tSettings(un_num_leds) {}

   /****************************************/
   /****************************************/
   
   void CCI_LEDsActuator::SetSingleColor(UInt32 un_led_number,
                                         const CColor& c_color) {
      m_tSettings[un_led_number] = c_color;
   }
      
   /****************************************/
   /****************************************/

   void CCI_LEDsActuator::SetAllColors(const CColor& c_color) {
      for(size_t i = 0; i < m_tSettings.size(); ++i) {
         m_tSettings[i] = c_color;
      }
   }

   /****************************************/
   /****************************************/

   void CCI_LEDsActuator::SetAllColors(const TSettings& c_colors) {
      m_tSettings = c_colors;
   }

   /****************************************/
   /****************************************/

   void CCI_LEDsActuator::SetSingleIntensity(UInt32 un_led_number,
                                             UInt8 un_intensity) {
      m_tSettings[un_led_number].SetAlpha(un_intensity);
   }

   /****************************************/
   /****************************************/

   void CCI_LEDsActuator::SetAllIntensities(UInt8 un_intensity) {
      for(size_t i = 0; i < m_tSettings.size(); ++i) {
         m_tSettings[i].SetAlpha(un_intensity);
      }
   }

   /****************************************/
   /****************************************/

}
