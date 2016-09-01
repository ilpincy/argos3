/**
 * @file <argos3/core/utility/math/angles.h>
 * 
 * Contains definitions for angle management, such as the CRadians and CDegrees classes.
 * 
 * Use ARGOS_PI when initializing static CRadians or CDegrees variables.
 * If instead you use the CRadians::PI constants (or similar) you may
 * incur in the 'static initialization order fiasco'.
 *
 * For normal usage (main() code and all the class methods) you have to
 * use the normal CRadians::PI variable and its siblings.
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#ifndef ANGLES_H
#define ANGLES_H

namespace argos {
   class CRadians;
   class CDegrees;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/math/general.h>
#include <argos3/core/utility/math/range.h>
#include <cmath>

/**
 * To be used when initializing static variables
 */
#define ARGOS_PI 3.14159265358979323846264338327950288

namespace argos {

   /****************************************/
   /****************************************/

   /**
    * It defines the basic type CRadians, used to store an angle value in radians.
    */
   class CRadians {

   public:

      /**
       * The PI constant
       */
      static const CRadians PI;

      /**
       * Set to PI * 2
       */
      static const CRadians TWO_PI;

      /**
       * Set to PI / 2
       */
      static const CRadians PI_OVER_TWO;

      /**
       * Set to PI / 3
       */
      static const CRadians PI_OVER_THREE;

      /**
       * Set to PI / 4
       */
      static const CRadians PI_OVER_FOUR;

      /**
       * Set to PI / 6
       */
      static const CRadians PI_OVER_SIX;

      /**
       * Set to zero radians
       */
      static const CRadians ZERO;

      /**
       * Class constructor
       * It initializes m_fValue to 0 radians.
       */
      CRadians() :
         m_fValue(0.0) {
      }

      /**
       * Class constructor
       * It initializes m_fValue to the passed value.
       * @param f_value the wanted value in radians
       */
      explicit CRadians(Real f_value) :
         m_fValue(f_value) {
      }

      /**
       * Sets the value from a value in degrees
       * It sets m_fValue (which is in radians) converting from the passed value in degrees.
       * @param f_value a value in degrees
       */
      inline void FromValueInDegrees(Real f_value) {
         m_fValue = f_value / RADIANS_TO_DEGREES;
      }

      /**
       * Returns the value in radians
       * @return the value in radians
       */
      inline Real GetValue() const {
         return m_fValue;
      }

      /**
       * Returns the absolute value in radians
       * @return the absolute value in radians
       */
      inline Real GetAbsoluteValue() const {
         return Abs(m_fValue);
      }

      /**
       * Sets the value in radians
       * @param f_value the wanted value in radians
       */
      inline void SetValue(Real f_value) {
         m_fValue = f_value;
      }

      /**
       * Normalizes the value in the range [-PI:PI]
       * @return A reference to the current object
       * @see CRadians::PI
       * @see CRange
       */
      inline CRadians& SignedNormalize() {
         SIGNED_RANGE.WrapValue(*this);
         return *this;
      }

      /**
       * Normalizes the value in the range [0:TWO_PI]
       * @return A reference to the current object
       * @see CRadians::TWO_PI
       * @see CRange
       */
      inline CRadians& UnsignedNormalize() {
         UNSIGNED_RANGE.WrapValue(*this);
         return *this;
      }

      inline CRadians& Negate() {
         m_fValue = -m_fValue;
         return *this;
      }

      inline CRadians& operator+() {
         return *this;
      }

      inline CRadians operator-() const {
         return CRadians(-m_fValue);
      }

      inline CRadians& operator+=(const CRadians& c_radians) {
         m_fValue += c_radians.m_fValue;
         return *this;
      }

      inline CRadians& operator-=(const CRadians& c_radians) {
         m_fValue -= c_radians.m_fValue;
         return *this;
      }

      inline CRadians& operator*=(Real f_value) {
         m_fValue *= f_value;
         return *this;
      }

      inline CRadians& operator/=(Real f_value) {
         m_fValue /= f_value;
         return *this;
      }

      inline CRadians operator+(const CRadians& c_radians) const {
         CRadians cResult(*this);
         cResult += c_radians;
         return cResult;
      }

      inline CRadians operator-(const CRadians& c_radians) const {
         CRadians cResult(*this);
         cResult -= c_radians;
         return cResult;
      }

      inline CRadians operator*(Real f_value) const {
         CRadians cResult(*this);
         cResult *= f_value;
         return cResult;
      }

      inline friend CRadians operator*(Real f_value,
                                       const CRadians& c_radians) {
         CRadians cResult(c_radians);
         cResult *= f_value;
         return cResult;
      }

      inline Real operator/(const CRadians& c_radians) const {
         return m_fValue / c_radians.m_fValue;
      }

      inline CRadians operator/(Real f_value) const {
         CRadians cResult(*this);
         cResult /= f_value;
         return cResult;
      }

      inline bool operator<(const CRadians& c_radians) const {
         return m_fValue < c_radians.m_fValue;
      }

      inline bool operator<=(const CRadians& c_radians) const {
         return m_fValue <= c_radians.m_fValue;
      }

      inline bool operator>(const CRadians& c_radians) const {
         return m_fValue > c_radians.m_fValue;
      }

      inline bool operator>=(const CRadians& c_radians) const {
         return m_fValue >= c_radians.m_fValue;
      }

      inline bool operator==(const CRadians& c_radians) const {
         return m_fValue == c_radians.m_fValue;
      }

      inline bool operator!=(const CRadians& c_radians) const {
         return m_fValue != c_radians.m_fValue;
      }

      /**
       * Converts this object to CDegrees
       * @return the conversion of m_fValue into CDegrees
       */
      friend CDegrees ToDegrees(const CRadians& c_radians);

      friend CRadians NormalizedDifference(const CRadians& c_rad1,
                                           const CRadians& c_rad2);

      inline friend std::ostream& operator<<(std::ostream& c_os,
                                             const CRadians& c_radians) {
         c_os << "CRadians("
              << c_radians.m_fValue
              << " -> "
              << c_radians.m_fValue * RADIANS_TO_DEGREES
              << " degrees"
              << ")";
         return c_os;
      }

      inline friend std::istream& operator>>(std::istream& is,
                                             CRadians& c_radians) {
         is >> c_radians.m_fValue;
         return is;
      }

   public:

      static const CRange<CRadians> SIGNED_RANGE; /**< The signed normalization range [-PI:PI] */
      static const CRange<CRadians> UNSIGNED_RANGE; /**< The unsigned normalization range [0:TWO_PI] */
      static const Real RADIANS_TO_DEGREES; /**< Constant to convert from radians to degrees */

   private:

      Real m_fValue;            /**< Actual angle value in radians */
   };

   /****************************************/
   /****************************************/

   /**
    * It defines the basic type CDegrees, used to store an angle value in degrees.
    */
   class CDegrees {

   public:

      /**
       * Class constructor
       * It initializes m_fValue to 0 degrees.
       */
      CDegrees() :
         m_fValue(0.0) {
      }

      /**
       * Class constructor
       * It initializes m_fValue to the passed value.
       * @param f_value the wanted value in degrees
       */
      explicit CDegrees(Real f_value) :
         m_fValue(f_value) {
      }

      /**
       * Sets the value from a value in radians
       * It sets m_fValue (which is in degrees) converting from the passed value in radians.
       * @param f_value a value in radians
       */
      inline void FromValueInRadians(Real f_value) {
         m_fValue = f_value / DEGREES_TO_RADIANS;
      }

      /**
       * Returns the value in degrees
       * @return the value in degrees
       */
      inline Real GetValue() const {
         return m_fValue;
      }

      /**
      * Returns the absolute value in degrees
      * @return the absolute value in degrees
      */
      inline Real GetAbsoluteValue() const {
       return Abs(m_fValue);
      }

      /**
       * Sets the value in degrees
       * @param f_value the wanted value in degrees
       */
      inline void SetValue(Real f_value) {
         m_fValue = f_value;
      }

      /**
       * Normalizes the value in the range [-180:180]
       * @return A reference to the current object
       * @see CRange
       */
      CDegrees& SignedNormalize() {
         SIGNED_RANGE.WrapValue(*this);
         return (*this);
      }

      /**
       * Normalizes the value in the range [0:360]
       * @return A reference to the current object
       * @see CRange
       */
      CDegrees& UnsignedNormalize() {
         UNSIGNED_RANGE.WrapValue(*this);
         return (*this);
      }

      inline CDegrees& operator+() {
         return *this;
      }

      inline CDegrees operator-() const {
         return CDegrees(-m_fValue);
      }

      inline CDegrees& operator+=(const CDegrees& c_degrees) {
         m_fValue += c_degrees.m_fValue;
         return *this;
      }

      inline CDegrees& operator-=(const CDegrees& c_degrees) {
         m_fValue -= c_degrees.m_fValue;
         return *this;
      }

      inline CDegrees& operator*=(Real f_value) {
         m_fValue *= f_value;
         return *this;
      }

      inline CDegrees& operator/=(Real f_value) {
         m_fValue /= f_value;
         return *this;
      }

      inline CDegrees operator+(const CDegrees& c_degrees) const {
         CDegrees cResult(*this);
         cResult += c_degrees;
         return cResult;
      }

      inline CDegrees operator-(const CDegrees& c_degrees) const {
         CDegrees cResult(*this);
         cResult -= c_degrees;
         return cResult;
      }

      inline CDegrees operator*(Real f_value) const {
         CDegrees cResult(*this);
         cResult *= f_value;
         return cResult;
      }

      inline friend CDegrees operator*(Real f_value,
                                       const CDegrees& c_degrees) {
         CDegrees cResult(c_degrees);
         cResult *= f_value;
         return cResult;
      }

      inline Real operator/(const CDegrees& c_degrees) const {
         return m_fValue / c_degrees.m_fValue;
      }

      inline CDegrees operator/(Real f_value) const {
         CDegrees cResult(*this);
         cResult /= f_value;
         return cResult;
      }

      inline bool operator<(const CDegrees& c_degrees) const {
         return m_fValue < c_degrees.m_fValue;
      }

      inline bool operator<=(const CDegrees& c_degrees) const {
         return m_fValue <= c_degrees.m_fValue;
      }

      inline bool operator>(const CDegrees& c_degrees) const {
         return m_fValue > c_degrees.m_fValue;
      }

      inline bool operator>=(const CDegrees& c_degrees) const {
         return m_fValue >= c_degrees.m_fValue;
      }

      inline bool operator==(const CDegrees& c_degrees) const {
         return m_fValue == c_degrees.m_fValue;
      }

      inline bool operator!=(const CDegrees& c_degrees) const {
         return m_fValue != c_degrees.m_fValue;
      }

      /**
       * Converts this object to CRadians
       * @return the conversion of m_fValue into CRadians
       */
      friend CRadians ToRadians(const CDegrees& c_degrees);

      friend CDegrees NormalizedDifference(const CDegrees& c_angle1,
                                           const CDegrees& c_angle2);

      inline friend std::ostream& operator<<(std::ostream& c_os,
                                             const CDegrees& c_degrees) {
         c_os << "CDegrees("
              << c_degrees.m_fValue
              << ")";
         return c_os;
      }

      inline friend std::istream& operator>>(std::istream& is,
                                             CDegrees& c_degrees) {
         is >> c_degrees.m_fValue;
         return is;
      }

   private:

      Real m_fValue; /**< Actual angle value in radians */
      static const CRange<CDegrees> SIGNED_RANGE; /**< The signed normalization range [-180:180] */
      static const CRange<CDegrees> UNSIGNED_RANGE; /**< The unsigned normalization range [0:360] */
      static const Real DEGREES_TO_RADIANS; /**< Constant to convert from degrees to radians */

   };

   /****************************************/
   /****************************************/

   /**
    * Converts CRadians to CDegrees
    * @param c_radians the object to convert
    * @return the converted CDegrees object
    */
   inline CDegrees ToDegrees(const CRadians& c_radians) {
      return CDegrees(c_radians.m_fValue * CRadians::RADIANS_TO_DEGREES);
   }

   /**
    * Converts CDegrees to CRadians
    * @param c_degrees the object to convert
    * @return the converted CRadians object
    */
   inline CRadians ToRadians(const CDegrees& c_degrees) {
      return CRadians(c_degrees.m_fValue * CDegrees::DEGREES_TO_RADIANS);
   }

   /**
    * Calculates the normalized difference between the given angles.
    * The difference is calculated as c_angle1 - c_angle2 and normalized
    * in the range [-pi,pi].
    * @param c_angle1 The first angle
    * @param c_angle2 The second angle
    * @return The smallest difference between the two angles
    */
   inline CRadians NormalizedDifference(const CRadians& c_angle1,
                                        const CRadians& c_angle2) {
      CRadians cResult;
      cResult.m_fValue = Mod(c_angle1.m_fValue - c_angle2.m_fValue + CRadians::PI.m_fValue,
                             CRadians::TWO_PI.m_fValue);
      if(cResult.m_fValue < 0.0f) cResult.m_fValue += CRadians::TWO_PI.m_fValue;
      cResult.m_fValue -= CRadians::PI.m_fValue;
      return cResult;
   }
   
   /**
    * Calculates the normalized difference between the given angles.
    * The difference is calculated as c_angle1 - c_angle2 and normalized
    * in the range [-180,180].
    * @param c_angle1 The first angle
    * @param c_angle2 The second angle
    * @return The smallest difference between the two angles
    */
   inline CDegrees NormalizedDifference(const CDegrees& c_angle1,
                                        const CDegrees& c_angle2) {
      CDegrees cResult;
      cResult.m_fValue = Mod(c_angle1.m_fValue - c_angle2.m_fValue + 180.0f, 360.0f);
      if(cResult.m_fValue < 0.0f) cResult.m_fValue += 360.0f;
      cResult.m_fValue -= 180.0f;
      return cResult;
   }
   
   /****************************************/
   /****************************************/

#undef ARGOS_SINCOS
#ifdef ARGOS_USE_DOUBLE
#  ifndef __APPLE__
#    define ARGOS_SINCOS ::sincos
#  else
#    define ARGOS_SINCOS ::__sincos
#  endif
#  define ARGOS_SIN    ::sin
#  define ARGOS_ASIN   ::asin
#  define ARGOS_COS    ::cos
#  define ARGOS_ACOS   ::acos
#  define ARGOS_TAN    ::tan
#  define ARGOS_ATAN2  ::atan2
#else
#  ifndef __APPLE__
#    define ARGOS_SINCOS ::sincosf
#  else
#    define ARGOS_SINCOS ::__sincosf
#  endif
#  define ARGOS_SIN    ::sinf
#  define ARGOS_ASIN   ::asinf
#  define ARGOS_COS    ::cosf
#  define ARGOS_ACOS   ::acosf
#  define ARGOS_TAN    ::tanf
#  define ARGOS_ATAN2  ::atan2f
#endif

#ifdef ARGOS_SINCOS
   /** 
    * Computes the sine and cosine of the passed value in radians
    * @param c_radians the angle in CRadians
    * @param f_sin the buffer in which the sin must be stored
    * @param f_cos the buffer in which the cos must be stored
    */
   inline void SinCos(const CRadians& c_radians,
                      Real& f_sin,
                      Real& f_cos) {
      ARGOS_SINCOS(c_radians.GetValue(), &f_sin, &f_cos);
   }
#endif

   /** 
    * Computes the sine of the passed value in radians
    * @param c_radians the angle in CRadians
    * @return the sine of the passed value
    */
   inline Real Sin(const CRadians& c_radians) {
      return ARGOS_SIN(c_radians.GetValue());
   }

   /** 
    * Computes the cosine of the passed value in radians
    * @param c_radians the angle in CRadians
    * @return the cosine of the passed value
    */
   inline Real Cos(const CRadians& c_radians) {
      return ARGOS_COS(c_radians.GetValue());
   }

   /** 
    * Computes the tangent of the passed value in radians
    * @param c_radians the angle in CRadians
    * @return the tangent of the passed value
    */
   inline Real Tan(const CRadians& c_radians) {
      return ARGOS_TAN(c_radians.GetValue());
   }

   /** 
    * Computes the arcsine of the passed value
    * @param f_value a value in the range [-1:1]
    * @return the arcsine in CRadians
    */
   inline CRadians ASin(Real f_value) {
      return CRadians(ARGOS_ASIN(f_value));
   }

   /** 
    * Computes the arccosine of the passed value
    * @param f_value a value in the range [-1:1]
    * @return the arccosine in CRadians
    */
   inline CRadians ACos(Real f_value) {
      return CRadians(ARGOS_ACOS(f_value));
   }

   /** 
    * Computes the arctangent of the passed values.
    * It internally uses the standard function <tt>atan2(y,x)</tt>.
    * @param f_y the extent on the y axis
    * @param f_x the extent on the x axis
    * @return the arctangent in CRadians
    */
   inline CRadians ATan2(const Real f_y, const Real f_x) {
      return CRadians(ARGOS_ATAN2(f_y, f_x));
   }

   /****************************************/
   /****************************************/

}

#endif
