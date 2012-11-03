/**
 * @file core/utility/math/vector2.h
 * @author Carlo Pinciroli <cpinciro@ulb.ac.be>
 *
 * @brief Contains the definition of the CVector2 class.
 */

#ifndef VECTOR2_H
#define VECTOR2_H

namespace argos {
   class CRotationMatrix2;
}

#include <argos3/core/utility/math/general.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/string_utilities.h>
#include <iostream>
#include <cmath>

namespace argos {

   /**
    * It defines a basic bidimensional vector.
    */
   class CVector2 {
   
   friend class CRotationMatrix2;

   public:

      static const CVector2 X;  /**< The X axis <1,0> */
      static const CVector2 Y;  /**< The Y axis <0,1> */

      /**
       * @brief Class constructor
       *
       * It initializes the vector coordinates to zero.
       */
      CVector2() :
         m_fX(0.0),
         m_fY(0.0) {
      }

      /**
       * @brief Class constructor
       *
       * It initializes the vector coordinates to the passed values.
       *
       * @param f_x the wanted value for the x coordinate
       * @param f_y the wanted value for the y coordinate
       */
      CVector2(Real f_x,
               Real f_y) :
         m_fX(f_x),
         m_fY(f_y) {
      }

      /**
       * @brief Class constructor
       *
       * It initializes the vector coordinates from the passed length and angle.
       *
       * @param f_length the wanted length
       * @param f_angle the wanted angle in radians
       */
      CVector2(Real f_length,
               const CRadians& f_angle) :
         m_fX(Cos(f_angle) * f_length),
         m_fY(Sin(f_angle) * f_length) {
      }

      /**
       * @brief It returns the X coordinate
       *
       * @return the vector's X coordinate
       */
      inline Real GetX() const {
         return m_fX;
      }

      /**
       * @brief It sets the value of the X coordinate
       *
       * @param f_x the wanted value
       */
      inline void SetX(Real f_x) {
         m_fX = f_x;
      }

      /**
       * @brief It returns the Y coordinate
       *
       * @return the vector's Y coordinate
       */
      inline Real GetY() const {
         return m_fY;
      }

      /**
       * @brief It sets the value of the Y coordinate
       *
       * @param f_y the wanted value
       */
      inline void SetY(Real f_y) {
         m_fY = f_y;
      }

      /**
       * @brief It sets the value of the X and Y coordinates
       *
       * @param f_x the wanted X value
       * @param f_y the wanted Y value
       */
      inline void Set(Real f_x, Real f_y) {
         m_fX = f_x;
         m_fY = f_y;
      }

      /**
       * @brief Sets the vector coordinates from angle and length
       *
       * It sets the vector coordinates from the passed angle and length.
       *
       * @param f_angle the wanted angle in radians
       * @param f_length the wanted length
       */
      inline void SetFromAngleAndLength(const CRadians& f_angle,
                                        Real f_length)
      {
         m_fX = Cos(f_angle) * f_length;
         m_fY = Sin(f_angle) * f_length;
      }

      /**
       * @brief It returns the square length of the vector
       *
       * @return the square length of the vector
       */
      inline Real SquareLength() const {
         return Square(m_fX) + Square(m_fY);
      }

      /**
       * @brief It returns the length of the vector
       *
       * @return the length of the vector
       */
      inline Real Length() const {
         return ::sqrt(SquareLength());
      }

      /**
       * @brief It normalizes the vector
       *
       * It normalizes the vector, i.e., it resets the vector coordinates in such a way that the angle is the same, but the length is 1.
       *
       * @return a reference to the current object after normalization
       */
      inline CVector2& Normalize() {
         (*this) /= Length();
         return (*this);
      }

      /**
       * @brief It returns the angle of the vector
       *
       * @return the angle of the vector in radians
       */
      inline CRadians Angle() const {
         return ATan2(m_fY, m_fX);
      }

      /**
       * @brief It rotates the vector by the wanted angle
       *
       * @return a reference to the current object after the rotation
       */
      inline CVector2& Rotate(const CRadians& c_angle) {
         Real cosa = Cos(c_angle);
         Real sina = Sin(c_angle);
         Real fX = m_fX * cosa - m_fY * sina;
         Real fY = m_fX * sina + m_fY * cosa;
         m_fX = fX;
         m_fY = fY;
         return *this;
      }

      /**
       * @brief It computes the dot product of the current vector with the passed one
       *
       * It computes the dot product of the current vector with the passed one. In other words, it calculates
       * m_fX * c_vector2.m_fX + m_fY * c_vector2.m_fY.
       *
       * @return the dot product of the current vector with the passed one
       */
      inline Real DotProduct(const CVector2& c_vector2) const {
         return m_fX * c_vector2.m_fX + m_fY * c_vector2.m_fY;
      }

      /**
       * @brief It scales the vector by the wanted values
       *
       * It scales the vector by the wanted values. In practice, this means that it multiplies the X coordinate by f_scale_x and the Y coordinate by f_scale_y.
       *
       * @param f_scale_x the scale factor for the X coordinate
       * @param f_scale_y the scale factor for the Y coordinate
       *
       * @return a reference to the current object after scaling
       */
      inline CVector2& Scale(Real f_scale_x,
                             Real f_scale_y) {
         m_fX *= f_scale_x;
         m_fY *= f_scale_y;
         return *this;
      }

      /**
       * @brief It transforms a vector into its ortogonal
       *
       * It transofroms the vector into its ortogonal, simply by swapping y and x coordinates.
       *
       * @return a reference to the current object after scaling
       */
      inline CVector2& Perpendicularize() {
         Real fNewX = -m_fY;
         m_fY = m_fX;
         m_fX = fNewX;
         return *this;
      }

      /**
       * @brief It transforms a vector into positive coordinates
       *
       * It transofroms the vector into a vector with positive cooridnates.
       *
       * @return a reference to the current object after scaling
       */
      inline CVector2& Absolute() {
	 m_fX = Abs(m_fX);
         m_fY = Abs(m_fY);
         return *this;
      }

      inline bool operator==(const CVector2& c_vector2) {
         return (m_fX == c_vector2.m_fX && m_fY == c_vector2.m_fY);
      }

      inline bool operator!=(const CVector2& c_vector2) {
         return (m_fX != c_vector2.m_fX || m_fY != c_vector2.m_fY);
      }

      inline CVector2& operator+=(const CVector2& c_vector2) {
         m_fX += c_vector2.m_fX;
         m_fY += c_vector2.m_fY;
         return *this;
      }

      inline CVector2& operator-=(const CVector2& c_vector2) {
         m_fX -= c_vector2.m_fX;
         m_fY -= c_vector2.m_fY;
         return *this;
      }

      inline CVector2& operator*=(Real f_value) {
         m_fX *= f_value;
         m_fY *= f_value;
         return *this;
      }

      inline CVector2& operator/=(Real f_value) {
         m_fX /= f_value;
         m_fY /= f_value;
         return *this;
      }

      inline CVector2 operator+(const CVector2& c_vector2) const {
         CVector2 cResult(*this);
         cResult += c_vector2;
         return cResult;
      }

      inline CVector2 operator-(const CVector2& c_vector2) const {
         CVector2 cResult(*this);
         cResult -= c_vector2;
         return cResult;
      }

      inline CVector2 operator*(Real f_value) const {
         CVector2 cResult(*this);
         cResult *= f_value;
         return cResult;
      }

      inline CVector2 operator/(Real f_value) const {
         CVector2 cResult(*this);
         cResult /= f_value;
         return cResult;
      }

      inline friend CVector2 operator*(Real f_value,
                                       const CVector2& c_vector2) {
         return c_vector2 * f_value;
      }

      inline CVector2 operator-() const {
         return CVector2(-m_fX, -m_fY);
      }

      inline friend std::ostream& operator<<(std::ostream& os,
                                             const CVector2& c_vector2) {
         os << c_vector2.m_fX << ","
            << c_vector2.m_fY;
         return os;
      }

      inline friend std::istream& operator>>(std::istream& is,
                                             CVector2& c_vector2) {
         Real fValues[2];
         ParseValues<Real> (is, 2, fValues, ',');
         c_vector2.Set(fValues[0], fValues[1]);
         return is;
      }

   private:

      Real m_fX;                /**< The vector X coordinate */
      Real m_fY;                /**< The vector Y coordinate */

   };

   /****************************************/
   /****************************************/

   /** 
    * It computes the square distance between the passed vectors.
    * 
    * @param c_v1 The first vector
    * @param c_v2 The second vector
    * 
    * @return the square distance between the passed vectors
    */
   inline Real SquareDistance(const CVector2& c_v1, const CVector2& c_v2) {
      return (c_v1 - c_v2).SquareLength();
   }

   /** 
    * It computes the distance between the passed vectors.
    * 
    * @param c_v1 The first vector
    * @param c_v2 The second vector
    * 
    * @return the distance between the passed vectors
    */
   inline Real Distance(const CVector2& c_v1, const CVector2& c_v2) {
      return (c_v1 - c_v2).Length();
   }

   /****************************************/
   /****************************************/

}

#endif
