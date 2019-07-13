/**
 * @file <argos3/core/utility/math/vector2.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef VECTOR2_H
#define VECTOR2_H

namespace argos {
   class CRotationMatrix2;
}

#include <argos3/core/utility/math/general.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/math/matrix/matrix.h>
#include <argos3/core/utility/string_utilities.h>
#include <array>
#include <iostream>
#include <cmath>

namespace argos {

   /**
    * A 2D vector class.
    */
   class CVector2 {
   
   friend class CRotationMatrix2;
   friend class CTransformationMatrix2;

   public:

      /** The <em>x</em> axis */
      static const CVector2 X;

      /** The <em>y</em> axis */
      static const CVector2 Y;

      /** The zero vector (0,0) */
      static const CVector2 ZERO;

      /**
       * Class constructor.
       * It initializes the vector to (0,0).
       * @see ZERO
       */
      CVector2() :
         m_fX(0.0),
         m_fY(0.0) {
      }

      /**
       * Class constructor.
       * It initializes the vector from Cartesian coordinates.
       * @param f_x The <em>x</em> coordinate.
       * @param f_y The <em>y</em> coordinate.
       * @see Set()
       */
      CVector2(Real f_x,
               Real f_y) :
         m_fX(f_x),
         m_fY(f_y) {
      }

      /**
       * Class constructor.
       * It initializes the vector from Cartesian coordinates.
       * @param arr_coordinates The coordinates.
       * @see Set()
       */
      CVector2(const std::array<Real, 2>& arr_coordinates) {
         Set(arr_coordinates);
      }

      /**
       * Class constructor.
       * It initializes the vector coordinates from polar coordinates.
       * @param f_length the vector length.
       * @param f_angle the vector angle.
       * @see FromPolarCoordinates()
       */
      CVector2(Real f_length,
               const CRadians& f_angle) :
         m_fX(Cos(f_angle) * f_length),
         m_fY(Sin(f_angle) * f_length) {
      }

      /**
       * Returns the <em>x</em> coordinate of this vector.
       * @return The <em>x</em> coordinate of this vector.
       */
      inline Real GetX() const {
         return m_fX;
      }

      /**
       * Sets the <em>x</em> coordinate of this vector.
       * @param f_x The new <em>x</em> coordinate of this vector.
       */
      inline void SetX(Real f_x) {
         m_fX = f_x;
      }

      /**
       * Returns the <em>y</em> coordinate of this vector.
       * @return The <em>y</em> coordinate of this vector.
       */
      inline Real GetY() const {
         return m_fY;
      }

      /**
       * Sets the <em>y</em> coordinate of this vector.
       * @param f_y The new <em>y</em> coordinate of this vector.
       */
      inline void SetY(Real f_y) {
         m_fY = f_y;
      }

      /**
       * Sets the vector contents from Cartesian coordinates.
       * @param f_x The new <em>x</em> coordinate of this vector.
       * @param f_y The new <em>y</em> coordinate of this vector.
       */
      inline void Set(Real f_x, Real f_y) {
         m_fX = f_x;
         m_fY = f_y;
      }

      /**
       * Sets the vector contents from Cartesian coordinates.
       * @param arr_coordinates The new coordinates.
       */
      inline void Set(const std::array<Real, 2>& arr_coordinates) {
         m_fX = arr_coordinates[0];
         m_fY = arr_coordinates[1];
      }

      /**
       * Sets the vector contents from polar coordinates.
       * @param f_length The length of the vector.
       * @param c_angle The angle of the vector (range [<em>0</em>,<em>2pi</em>])
       * @see Length()
       * @see Angle()
       */
      inline void FromPolarCoordinates(Real f_length,
                                       const CRadians& f_angle) {
         m_fX = Cos(f_angle) * f_length;
         m_fY = Sin(f_angle) * f_length;
      }

      /**
       * Returns the square length of this vector.
       * @return The square length of this vector.
       */
      inline Real SquareLength() const {
         return Square(m_fX) + Square(m_fY);
      }

      /**
       * Returns the length of this vector.
       * @return The length of this vector.
       */
      inline Real Length() const {
         return Sqrt(SquareLength());
      }

      /**
       * Normalizes this vector.
       * After this method is called, the vector has length 1. If the vector
       * is (0,0), this call results in a division by zero error.
       * @return A reference to this vector.
       */
      inline CVector2& Normalize() {
         *this /= Length();
         return *this;
      }

      /**
       * Returns the angle of this vector.
       * @return the angle of this vector.
       */
      inline CRadians Angle() const {
         return ATan2(m_fY, m_fX);
      }

      /**
       * Rotates this vector by the wanted angle.
       * @param c_angle The rotation angle.
       * @return A reference to this vector.
       */
      inline CVector2& Rotate(const CRadians& c_angle) {
         Real fSin, fCos;
#ifdef ARGOS_SINCOS
         SinCos(c_angle, fSin, fCos);
#else
         fSin = Sin(c_angle);
         fCos = Cos(c_angle);
#endif
         Real fX = m_fX * fCos - m_fY * fSin;
         Real fY = m_fX * fSin + m_fY * fCos;
         m_fX = fX;
         m_fY = fY;
         return *this;
      }

      /**
       * Returns the dot product between this vector and the passed one.
       * @param c_vector3 The other vector.
       * @return The dot product between this vector and the passed one.
       */
      inline Real DotProduct(const CVector2& c_vector2) const {
         return m_fX * c_vector2.m_fX + m_fY * c_vector2.m_fY;
      }

      /**
       * Returns the cross product between this vector and the passed one.
       * @param c_vector3 The other vector.
       * @return The cross product between this vector and the passed one.
       */
      inline Real CrossProduct(const CVector2& c_vector2) const {
         return m_fX * c_vector2.m_fY + m_fY * c_vector2.m_fX;
      }

      /**
       * Scales the vector by the wanted values.
       * This method scales the vector by the wanted values. In practice, this
       * means that it multiplies the <em>x</em> coordinate by f_scale_x and the
       * <em>y</em> coordinate by f_scale_y.
       * @param f_scale_x the scale factor for the <em>x</em> coordinate.
       * @param f_scale_y the scale factor for the <em>y</em> coordinate.
       * @return A reference to this vector.
       */
      inline CVector2& Scale(Real f_scale_x,
                             Real f_scale_y) {
         m_fX *= f_scale_x;
         m_fY *= f_scale_y;
         return *this;
      }

      /**
       * Transforms this vector into its ortogonal.
       * @return A reference to this vector.
       */
      inline CVector2& Perpendicularize() {
         Real fNewX = -m_fY;
         m_fY = m_fX;
         m_fX = fNewX;
         return *this;
      }

      /**
       * Applies Abs() to the coordinates of this vector.
       * @return A reference to this vector.
       */
      inline CVector2& Absolute() {
         m_fX = Abs(m_fX);
         m_fY = Abs(m_fY);
         return *this;
      }

      /**
       * Returns <tt>true</tt> if this vector and the passed one are equal.
       * This method checks all the coordinates for equality.
       * @param c_vector2 The other vector.
       * @return <tt>true</tt> if this vector and the passed one are equal.
       */
      inline bool operator==(const CVector2& c_vector2) const {
         return (m_fX == c_vector2.m_fX && m_fY == c_vector2.m_fY);
      }

      /**
       * Returns <tt>true</tt> if this vector and the passed one are not equal.
       * This method checks all the coordinates for equality.
       * @param c_vector2 The other vector.
       * @return <tt>true</tt> if this vector and the passed one are not equal.
       */
      inline bool operator!=(const CVector2& c_vector2) const {
         return (m_fX != c_vector2.m_fX || m_fY != c_vector2.m_fY);
      }

      /**
       * Sums the passed vector to this vector.
       * @param c_vector2 The other vector.
       * @returns A reference to this vector.
       */
      inline CVector2& operator+=(const CVector2& c_vector2) {
         m_fX += c_vector2.m_fX;
         m_fY += c_vector2.m_fY;
         return *this;
      }

      /**
       * Subtracts the passed vector from this vector.
       * @param c_vector3 The other vector.
       * @returns A reference to this vector.
       */
      inline CVector2& operator-=(const CVector2& c_vector2) {
         m_fX -= c_vector2.m_fX;
         m_fY -= c_vector2.m_fY;
         return *this;
      }

      /**
       * Multiplies this vector by the given value.
       * @param f_value The wanted value.
       * @return A reference to this vector.
       */
      inline CVector2& operator*=(Real f_value) {
         m_fX *= f_value;
         m_fY *= f_value;
         return *this;
      }

      /**
       * Divides this vector by the given value.
       * @param f_value The wanted value.
       * @return A reference to this vector.
       */
      inline CVector2& operator/=(Real f_value) {
         m_fX /= f_value;
         m_fY /= f_value;
         return *this;
      }

      /**
       * Returns a new vector containing the sum between this vector and the passed one.
       * @param c_vector2 The other vector.
       * @return A new vector containing the sum between this vector and the passed one.
       */
      inline CVector2 operator+(const CVector2& c_vector2) const {
         CVector2 cResult(*this);
         cResult += c_vector2;
         return cResult;
      }

      /**
       * Returns a new vector containing the subtraction between this vector and the passed one.
       * @param c_vector2 The other vector.
       * @return A new vector containing the subtraction between this vector and the passed one.
       */
      inline CVector2 operator-(const CVector2& c_vector2) const {
         CVector2 cResult(*this);
         cResult -= c_vector2;
         return cResult;
      }

      /**
       * Returns a new vector containing the multiplication between this vector and the passed value.
       * @param f_value The wanted value.
       * @return A new vector containing the multiplication between this vector and the passed value.
       */
      inline CVector2 operator*(Real f_value) const {
         CVector2 cResult(*this);
         cResult *= f_value;
         return cResult;
      }

      /**
       * Returns a new vector containing the division between this vector and the passed value.
       * @param f_value The wanted value.
       * @return A new vector containing the division between this vector and the passed value.
       */
      inline CVector2 operator/(Real f_value) const {
         CVector2 cResult(*this);
         cResult /= f_value;
         return cResult;
      }

      /**
       * Implict conversion to a row matrix.
       */
      operator CMatrix<1,2>() const {
         CMatrix<1,2> cMatrix;
         cMatrix(0,0) = m_fX;
         cMatrix(0,1) = m_fY;
         return cMatrix;
      }

      /**
       * Implict conversion to a column matrix.
       */
      operator CMatrix<2,1>() const {
         CMatrix<2,1> cMatrix;
         cMatrix(0,0) = m_fX;
         cMatrix(1,0) = m_fY;
         return cMatrix;
      }

      /**
       * Returns a new vector containing the multiplication between the passed value and the passed vector.
       * @param f_value The value.
       * @param c_vector3 The vector.
       * @return A new vector containing the multiplication between the passed value and the passed vector.
       */
      inline friend CVector2 operator*(Real f_value,
                                       const CVector2& c_vector2) {
         return c_vector2 * f_value;
      }

      inline CVector2 operator-() const {
         return CVector2(-m_fX, -m_fY);
      }

      /**
       * Serializes the contents of the passed vector onto a stream.
       * @param c_os The stream.
       * @param c_vector2 The vector.
       * @return The new state of the stream.
       */
      inline friend std::ostream& operator<<(std::ostream& c_os,
                                             const CVector2& c_vector2) {
         c_os << c_vector2.m_fX << ","
              << c_vector2.m_fY;
         return c_os;
      }

      /**
       * Deserializes the contents of a stream and stores them into the passed vector.
       * @param c_is The stream.
       * @param c_vector2 The vector.
       * @return The new state of the stream.
       */
      inline friend std::istream& operator>>(std::istream& c_is,
                                             CVector2& c_vector2) {
         Real fValues[2];
         ParseValues<Real> (c_is, 2, fValues, ',');
         c_vector2.Set(fValues[0], fValues[1]);
         return c_is;
      }

   private:

      /** The Cartesian <em>x</em> coordinate */
      Real m_fX;

      /** The Cartesian <em>y</em> coordinate */
      Real m_fY;

   };

   /****************************************/
   /****************************************/

   /** 
    * Computes the square distance between the passed vectors.
    * @param c_v1 The first vector
    * @param c_v2 The second vector
    * @return The square distance between the passed vectors
    */
   inline Real SquareDistance(const CVector2& c_v1, const CVector2& c_v2) {
      return (c_v1 - c_v2).SquareLength();
   }

   /** 
    * Computes the distance between the passed vectors.
    * @param c_v1 The first vector
    * @param c_v2 The second vector
    * @return The distance between the passed vectors
    */
   inline Real Distance(const CVector2& c_v1, const CVector2& c_v2) {
      return (c_v1 - c_v2).Length();
   }

   /****************************************/
   /****************************************/

}

#endif
