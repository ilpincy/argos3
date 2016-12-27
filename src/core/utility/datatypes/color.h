/**
 * @file <argos3/core/utility/datatypes/color.h>
 *
 * @brief Basic color-related definitions.
 *
 * This file provides the definition of the class argos::CColor.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef COLOR_H
#define COLOR_H

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/string_utilities.h>
#include <string.h>
#include <iostream>

namespace argos {

   /**
    * The basic color type.
    * In ARGoS, color is encoded into the RGBA schema.
    */
   class CColor {

   public:

      static CColor BLACK;
      static CColor WHITE;
      static CColor RED;
      static CColor GREEN;
      static CColor BLUE;
      static CColor MAGENTA;
      static CColor CYAN;
      static CColor YELLOW;
      static CColor ORANGE;
      static CColor BROWN;
      static CColor PURPLE;
      static CColor GRAY10;
      static CColor GRAY20;
      static CColor GRAY30;
      static CColor GRAY40;
      static CColor GRAY50;
      static CColor GRAY60;
      static CColor GRAY70;
      static CColor GRAY80;
      static CColor GRAY90;

      /**
       * Class constructor.
       */
      CColor() {}

      /**
       * Class constructor.
       */
      explicit CColor(UInt8 un_red,
                      UInt8 un_green,
                      UInt8 un_blue,
                      UInt8 un_alpha = 255) throw() :
         m_tChannels(un_red, un_green, un_blue, un_alpha) {}

      /**
       * Returns the color in grayscale.
       * @returns the color in grayscale. The returned value is in the range [0,1].
       */
      inline Real ToGrayScale() const throw() {
         return
            0.299f * m_tChannels.m_unRed +
            0.587f * m_tChannels.m_unGreen +
            0.114f * m_tChannels.m_unBlue;
      }

      /**
       * Returns the red channel of the color.
       * @returns the red channel of the color.
       */
      inline UInt8 GetRed() const throw() { return m_tChannels.m_unRed; }
      /**
       * Sets the red channel of the color.
       * @param un_red the red channel of the color.
       */
      inline void SetRed(UInt8 un_red) throw() { m_tChannels.m_unRed = un_red; }

      /**
       * Returns the green channel of the color.
       * @returns the green channel of the color.
       */
      inline UInt8 GetGreen() const throw() { return m_tChannels.m_unGreen; }
      /**
       * Sets the green channel of the color.
       * @param un_green the green channel of the color.
       */
      inline void SetGreen(UInt8 un_green) throw() { m_tChannels.m_unGreen = un_green; }

      /**
       * Returns the blue channel of the color.
       * @returns the blue channel of the color.
       */
      inline UInt8 GetBlue() const throw() { return m_tChannels.m_unBlue; }
      /**
       * Sets the blue channel of the color.
       * @param un_blue the blue channel of the color.
       */
      inline void SetBlue(UInt8 un_blue) throw() { m_tChannels.m_unBlue = un_blue; }

      /**
       * Returns the alpha channel of the color.
       * @returns the alpha channel of the color.
       */
      inline UInt8 GetAlpha() const throw() { return m_tChannels.m_unAlpha; }
      /**
       * Sets the alpha channel of the color.
       * @param un_alpha the alpha channel of the color.
       */
      inline void SetAlpha(UInt8 un_alpha) throw() { m_tChannels.m_unAlpha = un_alpha; }

      /**
       * Sets the RGBA values of the color.
       * @param un_red the red channel of the color.
       * @param un_green the green channel of the color.
       * @param un_blue the blue channel of the color.
       * @param un_alpha the alpha channel of the color.
       */
      inline void Set(UInt8 un_red,
                      UInt8 un_green,
                      UInt8 un_blue,
                      UInt8 un_alpha = 255) throw() {
         SetRed(un_red);
         SetGreen(un_green);
         SetBlue(un_blue);
         SetAlpha(un_alpha);
      }

      /**
       * Sets the RGBA values of the color from a string.
       * The input stringcan contain predefined color names, such as <tt>red</tt>.
       * The color can be also input as a tuple <tt>&lt;r,g,b,a&gt;</tt>.
       * @param str_color the input string.
       * @throws CARGoSException in case of a parse error.
       */
      inline void Set(const std::string& str_color) {
         try {
            if      (str_color == "black")   *this = CColor::BLACK;
            else if (str_color == "white")   *this = CColor::WHITE;
            else if (str_color == "red")     *this = CColor::RED;
            else if (str_color == "green")   *this = CColor::GREEN;
            else if (str_color == "blue")    *this = CColor::BLUE;
            else if (str_color == "magenta") *this = CColor::MAGENTA;
            else if (str_color == "cyan")    *this = CColor::CYAN;
            else if (str_color == "yellow")  *this = CColor::YELLOW;
            else if (str_color == "orange")  *this = CColor::ORANGE;
            else if (str_color == "brown")   *this = CColor::BROWN;
            else if (str_color == "purple")  *this = CColor::PURPLE;
            else if (str_color == "gray10")  *this = CColor::GRAY10;
            else if (str_color == "gray20")  *this = CColor::GRAY20;
            else if (str_color == "gray30")  *this = CColor::GRAY30;
            else if (str_color == "gray40")  *this = CColor::GRAY40;
            else if (str_color == "gray50")  *this = CColor::GRAY50;
            else if (str_color == "gray60")  *this = CColor::GRAY60;
            else if (str_color == "gray70")  *this = CColor::GRAY70;
            else if (str_color == "gray80")  *this = CColor::GRAY80;
            else if (str_color == "gray90")  *this = CColor::GRAY90;
            else {
               UInt16 unValues[4];
               ParseValues<UInt16>(str_color, 4, unValues, ',');
               for(UInt16 i = 0; i < 4; ++i) {
                  if(unValues[i] > 255) {
                     THROW_ARGOSEXCEPTION("Color value " << unValues[i] << " is larger than 255.");
                  }
               }
               Set(unValues[0], unValues[1], unValues[2], unValues[3]);
            }
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Error while parsing color input string", ex);
         }
      }

      /**
       * Cast operator.
       * Casts the color into a 32-bit structure.
       */
      inline operator UInt32() {
         return *reinterpret_cast<UInt32*>(&m_tChannels);
      }

      /**
       * Returns <tt>true</tt> if the given color is identical to the current.
       * @param c_color2 the color to compare the current color to.
       * @return <tt>true</tt> if the given color is identical to the current.
       */
      inline bool operator==(const CColor& c_color2) const throw() {
         return m_tChannels == c_color2.m_tChannels;
      }

      /**
       * Returns <tt>true</tt> if the given color is different from the current.
       * @param c_color2 the color to compare the current color to.
       * @return <tt>true</tt> if the given color is different from the current.
       */
      inline bool operator!=(const CColor& c_color2) const throw() {
         return m_tChannels != c_color2.m_tChannels;
      }

      /**
       * Output stream operator.
       * Operator to serialize a color into a C++ output stream. When a color
       * corresponds to one of the predefined constants (such as argos::CColor::RED)
       * it streams the color name. For instance, if the current color is
       * argos::CColor::RED, the output is a string containing <tt>red</tt>. Otherwise,
       * the color is output as a tuple <tt>&lt;r,g,b,a&gt;</tt>.
       * @param os the C++ output stream.
       * @param c_color the color to stream.
       * @return the new state of the C++ output stream.
       */
      friend std::ostream& operator<<(std::ostream& os,
                                      const CColor& c_color) {
         if      (c_color == CColor::BLACK)   os << "black";
         else if (c_color == CColor::WHITE)   os << "white";
         else if (c_color == CColor::RED)     os << "red";
         else if (c_color == CColor::GREEN)   os << "green";
         else if (c_color == CColor::BLUE)    os << "blue";
         else if (c_color == CColor::MAGENTA) os << "magenta";
         else if (c_color == CColor::CYAN)    os << "cyan";
         else if (c_color == CColor::YELLOW)  os << "yellow";
         else if (c_color == CColor::ORANGE)  os << "orange";
         else if (c_color == CColor::BROWN)   os << "brown";
         else if (c_color == CColor::PURPLE)  os << "purple";
         else if (c_color == CColor::GRAY10)  os << "gray10";
         else if (c_color == CColor::GRAY20)  os << "gray20";
         else if (c_color == CColor::GRAY30)  os << "gray30";
         else if (c_color == CColor::GRAY40)  os << "gray40";
         else if (c_color == CColor::GRAY50)  os << "gray50";
         else if (c_color == CColor::GRAY60)  os << "gray60";
         else if (c_color == CColor::GRAY70)  os << "gray70";
         else if (c_color == CColor::GRAY80)  os << "gray80";
         else if (c_color == CColor::GRAY90)  os << "gray90";
         else {
            os << c_color.m_tChannels.m_unRed
               << "," << c_color.m_tChannels.m_unGreen
               << "," << c_color.m_tChannels.m_unBlue
               << "," << c_color.m_tChannels.m_unAlpha;
         }
         return os;
      }

      /**
       * Input stream operator.
       * Operator to get a color from a C++ input stream. The input stream
       * value can contain predefined color names, such as <tt>red</tt>. The
       * color can be also input as a tuple <tt>&lt;r,g,b,a&gt;</tt>.
       * @param is the C++ input stream.
       * @param c_color the color to fill.
       * @return the new state of the C++ input stream.
       */
      friend std::istream& operator>>(std::istream& is,
                                      CColor& c_color) {
         std::string strColor;
         is >> strColor;
         c_color.Set(strColor);
         return is;
      }


   private:

      struct TChannels {
         UInt8 m_unRed;
         UInt8 m_unGreen;
         UInt8 m_unBlue;
         UInt8 m_unAlpha;

         TChannels() :
            m_unRed(0),
            m_unGreen(0),
            m_unBlue(0),
            m_unAlpha(255) {}

         TChannels(const UInt8 un_red,
                   const UInt8 un_green,
                   const UInt8 un_blue,
                   const UInt8 un_alpha = 255) :
            m_unRed(un_red),
            m_unGreen(un_green),
            m_unBlue(un_blue),
            m_unAlpha(un_alpha) {}

         inline bool operator==(const TChannels& t_channels) const {
            return
               (m_unRed == t_channels.m_unRed) &&
               (m_unGreen == t_channels.m_unGreen) &&
               (m_unBlue == t_channels.m_unBlue) &&
               (m_unAlpha == t_channels.m_unAlpha);
         }

         inline bool operator!=(const TChannels& t_channels) const {
            return
               (m_unRed != t_channels.m_unRed) ||
               (m_unGreen != t_channels.m_unGreen) ||
               (m_unBlue != t_channels.m_unBlue) ||
               (m_unAlpha != t_channels.m_unAlpha);
         }

      } m_tChannels;

   };

}

#endif
