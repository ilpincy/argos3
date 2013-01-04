/**
 * @file <argos3/core/utility/logging/argos_colored_text.h>
 *
 * @brief This file provides a list of functions to add colors to std::ostreams.
 *
 * Credit for some of the ideas in this file to Vincent Godin.
 * See <a href="http://www.codeproject.com/KB/cpp/cout_color.aspx">http://www.codeproject.com/KB/cpp/cout_color.aspx</a> (accessed May 10th, 2010).
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef ARGOS_COLORED_TEXT_H
#define ARGOS_COLORED_TEXT_H

#include <argos3/core/utility/datatypes/datatypes.h>

namespace argos {

   /**
    * The possible attributes of the logged text.
    */
   enum EARGoSLogAttributes {
      ARGOS_LOG_ATTRIBUTE_RESET      = 0,
      ARGOS_LOG_ATTRIBUTE_BRIGHT     = 1,
      ARGOS_LOG_ATTRIBUTE_DIM        = 2,
      ARGOS_LOG_ATTRIBUTE_UNDERSCORE = 3,
      ARGOS_LOG_ATTRIBUTE_BLINK      = 5,
      ARGOS_LOG_ATTRIBUTE_REVERSE    = 7,
      ARGOS_LOG_ATTRIBUTE_HIDDEN     = 8
   };

   /**
    * The possible colors of the logged text.
    */
   enum EARGoSLogColors {
      ARGOS_LOG_COLOR_BLACK   = 0,
      ARGOS_LOG_COLOR_RED     = 1,
      ARGOS_LOG_COLOR_GREEN   = 2,
      ARGOS_LOG_COLOR_YELLOW  = 3,
      ARGOS_LOG_COLOR_BLUE    = 4,
      ARGOS_LOG_COLOR_MAGENTA = 5,
      ARGOS_LOG_COLOR_CYAN    = 6,
      ARGOS_LOG_COLOR_WHITE   = 7
   };

   /**
    * Stream modifier to set attribute and color of the subsequent text.
    */
   struct SLogColor {
      /**
       * The text attribute.
       */
      UInt8 Attribute;
      /**
       * The foreground color.
       */
      UInt8 Foreground;

      /**
       * Constructor with standard settings.
       */
      SLogColor() :
         Attribute(ARGOS_LOG_ATTRIBUTE_RESET),
         Foreground(ARGOS_LOG_COLOR_WHITE) {}
      /**
       * Constructor with custom settings.
       */
      SLogColor(EARGoSLogAttributes e_attribute,
                EARGoSLogColors e_foreground_color) :
         Attribute(e_attribute),
         Foreground(e_foreground_color) {}
   };

   /**
    * Stream operator that accepts the stream modifier.
    * @see SLogColor
    */
   inline std::ostream& operator<<(std::ostream& c_os, const SLogColor& s_log_color)
   {
      c_os << "\033["
           << s_log_color.Attribute << ";"
           << (s_log_color.Foreground + 30) << "m";
      return c_os;
   }

   /**
    * Utility macro to ease the definition of the presets.
    */
#define DEFINE_ARGOS_STREAM_COLOR_HELPER(lc_color, uc_color)            \
   inline std::ostream& lc_color(std::ostream& c_os) {                  \
      c_os << SLogColor(ARGOS_LOG_ATTRIBUTE_BRIGHT, ARGOS_LOG_COLOR_ ## uc_color); \
      return c_os;                                                      \
   }

   /**
    * Bright red text modifier.
    */
   DEFINE_ARGOS_STREAM_COLOR_HELPER(red, RED);
   /**
    * Bright green text modifier.
    */
   DEFINE_ARGOS_STREAM_COLOR_HELPER(green, GREEN);
   /**
    * Bright yellow text modifier.
    */
   DEFINE_ARGOS_STREAM_COLOR_HELPER(yellow, YELLOW);
   /**
    * Bright blue text modifier.
    */
   DEFINE_ARGOS_STREAM_COLOR_HELPER(blue, BLUE);
   /**
    * Bright magenta text modifier.
    */
   DEFINE_ARGOS_STREAM_COLOR_HELPER(magenta, MAGENTA);
   /**
    * Bright cyan text modifier.
    */
   DEFINE_ARGOS_STREAM_COLOR_HELPER(cyan, CYAN);
   /**
    * Bright white text modifier.
    */
   DEFINE_ARGOS_STREAM_COLOR_HELPER(white, WHITE);

   /**
    * Resets the text to the default settings.
    * The default settings are white text on black background.
    */
   inline std::ostream& reset(std::ostream& c_os) {
      c_os << "\033["
           << ARGOS_LOG_ATTRIBUTE_RESET
           << "m";
      return c_os;
   }

}

#endif
