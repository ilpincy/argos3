/**
 * @file <argos3/core/utility/math/vector3.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef VECTOR3_H
#define VECTOR3_H

namespace argos {
   class CVector3;
   class CVector2;
   class CQuaternion;
   class CRotationMatrix3;
}

#include <argos3/core/utility/math/general.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/string_utilities.h>
#include <iostream>
#include <cmath>

namespace argos {

   /**
    * A 3D vector class.
    */
   class CVector3 {
   
   public:

      /** The <em>x</em> axis */
      static const CVector3 X;

      /** The <em>y</em> axis */
      static const CVector3 Y;

      /** The <em>z</em> axis */
      static const CVector3 Z;

      /** The zero vector (0,0,0) */
      static const CVector3 ZERO;

   public:

      /**
       * Class constructor.
       * It initializes the vector to (0,0,0).
       * @see ZERO
       */
      CVector3() :
         m_fX(0.0),
         m_fY(0.0),
         m_fZ(0.0) {
      }

      /**
       * Class constructor.
       * It initializes the vector from Cartesian coordinates.
       * @param f_x The <em>x</em> coordinate.
       * @param f_y The <em>y</em> coordinate.
       * @param f_z The <em>z</em> coordinate.
       * @see Set()
       */
      CVector3(Real f_x,
               Real f_y,
               Real f_z) :
         m_fX(f_x),
         m_fY(f_y),
         m_fZ(f_z) {
      }

      /**
       * Class constructor.
       * It initializes the vector from spherical coordinates.
       * @param f_length The length of the vector.
       * @param c_inclination The angle between the <em>z</em> axis and the vector (range [<em>-pi</em>,<em>pi</em>])
       * @param c_azimuth The angle on the <em>xy</em> plane (range [<em>0</em>,<em>2pi</em>])
       * @see FromSphericalCoords()
       * @see ToSphericalCoords()
       */
      inline CVector3(Real f_length,
                      const CRadians& c_inclination,
                      const CRadians& c_azimuth) {
         FromSphericalCoords(f_length, c_inclination, c_azimuth);
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
      inline void SetX(const Real f_x) {
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
      inline void SetY(const Real f_y) {
         m_fY = f_y;
      }

      /**
       * Returns the <em>z</em> coordinate of this vector.
       * @return The <em>z</em> coordinate of this vector.
       */
      inline Real GetZ() const {
         return m_fZ;
      }

      /**
       * Sets the <em>z</em> coordinate of this vector.
       * @param f_z The new <em>z</em> coordinate of this vector.
       */
      inline void SetZ(const Real f_z) {
         m_fZ = f_z;
      }

      /**
       * Sets the vector contents from Cartesian coordinates.
       * @param f_x The new <em>x</em> coordinate of this vector.
       * @param f_y The new <em>y</em> coordinate of this vector.
       * @param f_z The new <em>z</em> coordinate of this vector.
       */
      inline void Set(const Real f_x,
                      const Real f_y,
                      const Real f_z) {
         m_fX = f_x;
         m_fY = f_y;
         m_fZ = f_z;
      }

      /**
       * Sets the vector contents from spherical coordinates.
       * @param f_length The length of the vector.
       * @param c_inclination The angle between the <em>z</em> axis and the vector (range [<em>-pi</em>,<em>pi</em>])
       * @param c_azimuth The angle on the <em>xy</em> plane (range [<em>0</em>,<em>2pi</em>])
       * @see ToSphericalCoords()
       */
      inline CVector3& FromSphericalCoords(Real f_length,
                                           const CRadians& c_inclination,
                                           const CRadians& c_azimuth) {
         Real fInclinationSin, fInclinationCos;
         Real fAzimuthSin, fAzimuthCos;
#ifdef ARGOS_SINCOS
         SinCos(c_inclination, fInclinationSin, fInclinationCos);
         SinCos(c_azimuth, fAzimuthSin, fAzimuthCos);
#else
         fInclinationSin = Sin(c_inclination);
         fInclinationCos = Cos(c_inclination);
         fAzimuthSin = Sin(c_azimuth);
         fAzimuthCos = Cos(c_azimuth);
#endif
         m_fX = f_length * fInclinationSin * fAzimuthCos;
         m_fY = f_length * fInclinationSin * fAzimuthSin;
         m_fZ = f_length * fInclinationCos;
         return *this;
      }

      /**
       * Returns the vector contents as spherical coordinates.
       * @param f_length A buffer for the length of the vector.
       * @param c_inclination A buffer for the angle between the <em>z</em> axis and the vector (range [<em>-pi</em>,<em>pi</em>])
       * @param c_azimuth A buffer for the angle on the <em>xy</em> plane (range [<em>0</em>,<em>2pi</em>])
       * @see FromSphericalCoords()
       */
      inline void ToSphericalCoords(Real& f_radius,
                                    CRadians& c_inclination,
                                    CRadians& c_azimuth) const {
         f_radius = Length();
         c_inclination = ACos(m_fZ / f_radius);
         c_azimuth = ATan2(m_fY, m_fX);
      }

      /**
       * Returns the square length of this vector.
       * @return The square length of this vector.
       */
      inline Real SquareLength() const {
         return Square(m_fX) + Square(m_fY) + Square(m_fZ);
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
       * is (0,0,0), this call results in a division by zero error.
       * @return A reference to this vector.
       */
      inline CVector3& Normalize() {
         *this /= Length();
         return *this;
      }

      /**
       * Rotates this vector wrt the <em>x</em> axis.
       * @param c_angle The rotation angle.
       * @return A reference to this vector.
       */
      inline CVector3& RotateX(const CRadians& c_angle) {
         Real fSin, fCos;
#ifdef ARGOS_SINCOS
         SinCos(c_angle, fSin, fCos);
#else
         fSin = Sin(c_angle);
         fCos = Cos(c_angle);
#endif
         Real fNewY = m_fY * fCos - m_fZ * fSin;
         Real fNewZ = m_fY * fSin + m_fZ * fCos;
         m_fY = fNewY;
         m_fZ = fNewZ;
         return *this;
      }

      /**
       * Rotates this vector wrt the <em>y</em> axis.
       * @param c_angle The rotation angle.
       * @return A reference to this vector.
       */
      inline CVector3& RotateY(const CRadians& c_angle) {
         Real fSin, fCos;
#ifdef ARGOS_SINCOS
         SinCos(c_angle, fSin, fCos);
#else
         fSin = Sin(c_angle);
         fCos = Cos(c_angle);
#endif
         Real fNewX = m_fX * fCos + m_fZ * fSin;
         Real fNewZ = m_fZ * fCos - m_fX * fSin;
         m_fX = fNewX;
         m_fZ = fNewZ;
         return *this;
      }

      /**
       * Rotates this vector wrt the <em>z</em> axis.
       * @param c_angle The rotation angle.
       * @return A reference to this vector.
       */
      inline CVector3& RotateZ(const CRadians& c_angle) {
         Real fSin, fCos;
#ifdef ARGOS_SINCOS
         SinCos(c_angle, fSin, fCos);
#else
         fSin = Sin(c_angle);
         fCos = Cos(c_angle);
#endif
         Real fNewX = m_fX * fCos - m_fY * fSin;
         Real fNewY = m_fX * fSin + m_fY * fCos;
         m_fX = fNewX;
         m_fY = fNewY;
         return *this;
      }

      /**
       * Rotates this vector wrt the <em>z</em> axis.
       * This method rotates the vector wrt to the <em>z</em> axis using a
       * 2d vector that is expected to contain: (cos(<em>angle</em>), sin(<em>angle</em>)).
       * This method is mostly useful when you must rotate a lot of vectors by the same
       * angle. You calculate the angle's cosine and sine, store it, and using multiple
       * times. For speed reasons, no checks are performed - be sure you feed this method
       * the right values. 
       * @param c_vector The precalculated cosine and sine of the rotation angle.
       * @return A reference to this vector.
       * @see RotateZ(const CRadians&)
       */
      inline CVector3& RotateZ(const CVector2& c_vector) {
         Real fNewX = m_fX * c_vector.GetX() - m_fY * c_vector.GetY();
         Real fNewY = m_fX * c_vector.GetY() + m_fY * c_vector.GetX();
         m_fX = fNewX;
         m_fY = fNewY;
         return *this;
      }

      /**
       * Rotates this vector by the given quaternion.
       * @param c_quaternion The quaternion to use.
       * @return A reference to this vector.
       * @see CQuaternion
       */
      CVector3& Rotate(const CQuaternion& c_quaternion);

      /**
       * Returns the angle between this vector and the passed vector.
       * @param c_other The other vector.
       * @return The angle between this vector and the passed vector.
       */
      inline CRadians GetAngleWith(const CVector3& c_other) {
         return CRadians(ACos(DotProduct(c_other) /
                              (Length() *
                               c_other.Length())));
      }

      /**
       * Returns the angle between this vector and the <em>x</em> axis.
       * @return The angle between this vector and the <em>x</em> axis.
       */
      inline CRadians GetXAngle() const {
         return ATan2(m_fZ, m_fY);
      }

      /**
       * Returns the angle between this vector and the <em>y</em> axis.
       * @return The angle between this vector and the <em>y</em> axis.
       */
      inline CRadians GetYAngle() const {
         return ATan2(m_fX, m_fZ);
      }

      /**
       * Returns the angle between this vector and the <em>z</em> axis.
       * @return The angle between this vector and the <em>z</em> axis.
       */
      inline CRadians GetZAngle() const {
         return ATan2(m_fY, m_fX);
      }

      /**
       * Returns the dot product between this vector and the passed one.
       * @param c_vector3 The other vector.
       * @return The dot product between this vector and the passed one.
       */
      inline Real DotProduct(const CVector3& c_vector3) const {
         return
            m_fX * c_vector3.m_fX +
            m_fY * c_vector3.m_fY +
            m_fZ * c_vector3.m_fZ;
      }

      /**
       * Calculates the cross product between this vector and the passed one.
       * This method modifies this vector.
       * @param c_vector3 The other vector.
       * @return A reference to this vector.
       */
      inline CVector3& CrossProduct(const CVector3& c_vector3) {
         Real fNewX, fNewY, fNewZ;
         fNewX = m_fY * c_vector3.m_fZ - m_fZ * c_vector3.m_fY;
         fNewY = m_fZ * c_vector3.m_fX - m_fX * c_vector3.m_fZ;
         fNewZ = m_fX * c_vector3.m_fY - m_fY * c_vector3.m_fX;
         m_fX = fNewX;
         m_fY = fNewY;
         m_fZ = fNewZ;
         return *this;
      }

      /**
       * Calculates the projection of this vector onto the <em>xy</em> plane.
       * @param c_proj A 2d vector to use as buffer.
       * @return A reference to the passed 2d vector buffer.
       */
      inline CVector2& ProjectOntoXY(CVector2& c_proj) const {
         c_proj.Set(m_fX,m_fY);
         return c_proj;
      }

      /**
       * Calculates the projection of this vector onto the <em>yz</em> plane.
       * @param c_proj A 2d vector to use as buffer.
       * @return A reference to the passed 2d vector buffer.
       */
      inline CVector2& ProjectOntoYZ(CVector2& c_proj) const {
         c_proj.Set(m_fY,m_fZ);
         return c_proj;
      }

      /**
       * Calculates the projection of this vector onto the <em>xz</em> plane.
       * @param c_proj A 2d vector to use as buffer.
       * @return A reference to the passed 2d vector buffer.
       */
      inline CVector2& ProjectOntoXZ(CVector2& c_proj) const {
         c_proj.Set(m_fX,m_fZ);
         return c_proj;
      }

      /**
       * Negates this vector.
       * After this method is called, this vector contains (-<em>x</em>,-<em>y</em>,-<em>z</em>).
       * @return A reference to this vector.
       */
      inline CVector3& Negate() {
         m_fX = -m_fX;
         m_fY = -m_fY;
         m_fZ = -m_fZ;
         return *this;
      }

      /**
       * Returns a Cartesian coordinate of this vector.
       * This method returns the value by copy.
       * @param un_index The desired coordinate, with 0 mapping to <em>x</em>, 1 mapping to <em>y</em>, and 2 mapping to <em>z</em>.
       * @return A Cartesian coordinate of this vector.
       * @throws CARGoSException if the given index is out of bounds.
       */
      inline Real operator[](UInt32 un_index) const {
         switch(un_index) {
            case 0: return m_fX;
            case 1: return m_fY;
            case 2: return m_fZ;
            default: THROW_ARGOSEXCEPTION("Real Vector3::operator[]: index " << un_index << " out of bounds");
         }
      }

      /**
       * Returns a Cartesian coordinate of this vector.
       * This method returns the value by reference.
       * @param un_index The desired coordinate, with 0 mapping to <em>x</em>, 1 mapping to <em>y</em>, and 2 mapping to <em>z</em>.
       * @return A Cartesian coordinate of this vector.
       * @throws CARGoSException if the given index is out of bounds.
       */
      inline Real& operator[](UInt32 un_index) {
         switch(un_index) {
            case 0: return m_fX;
            case 1: return m_fY;
            case 2: return m_fZ;
            default: THROW_ARGOSEXCEPTION("Real Vector3::operator[]: index " << un_index << " out of bounds");
         }
      }

      /**
       * Returns <tt>true</tt> if this vector and the passed one are equal.
       * This method checks all the coordinates for equality.
       * @param c_vector3 The other vector.
       * @return <tt>true</tt> if this vector and the passed one are equal.
       */
      inline bool operator==(const CVector3& c_vector3) const {
         return m_fX == c_vector3.m_fX && m_fY == c_vector3.m_fY && m_fZ == c_vector3.m_fZ;
      }

      /**
       * Returns <tt>true</tt> if this vector and the passed one are not equal.
       * This method checks all the coordinates for equality.
       * @param c_vector3 The other vector.
       * @return <tt>true</tt> if this vector and the passed one are not equal.
       */
      inline bool operator!=(const CVector3& c_vector3) const {
         return !((*this) == c_vector3);
      }

      /**
       * Returns <tt>true</tt> if this vector is smaller than the passed one.
       * This method checks all the coordinates, and returns <tt>true</tt> only
       * if the condition is true for all of them.
       * @param c_vector3 The other vector.
       * @return <tt>true</tt> if this vector is smaller than the passed one.
       */
      inline bool operator<(const CVector3& c_vector3) const {
         return m_fX < c_vector3.m_fX && m_fY < c_vector3.m_fY && m_fZ < c_vector3.m_fZ;
      }

      /**
       * Returns <tt>true</tt> if this vector is smaller than or equal to the passed one.
       * This method checks all the coordinates, and returns <tt>true</tt> only
       * if the condition is true for all of them.
       * @param c_vector3 The other vector.
       * @return <tt>true</tt> if this vector is smaller than or equal to the passed one.
       */
      inline bool operator<=(const CVector3& c_vector3) const {
         return m_fX <= c_vector3.m_fX && m_fY <= c_vector3.m_fY && m_fZ <= c_vector3.m_fZ;
      }

      /**
       * Returns <tt>true</tt> if this vector is greater than the passed one.
       * This method checks all the coordinates, and returns <tt>true</tt> only
       * if the condition is true for all of them.
       * @param c_vector3 The other vector.
       * @return <tt>true</tt> if this vector is greater than the passed one.
       */
      inline bool operator>(const CVector3& c_vector3) const {
         return m_fX > c_vector3.m_fX && m_fY > c_vector3.m_fY && m_fZ > c_vector3.m_fZ;
      }

      /**
       * Returns <tt>true</tt> if this vector is greater than or equal to the passed one.
       * This method checks all the coordinates, and returns <tt>true</tt> only
       * if the condition is true for all of them.
       * @param c_vector3 The other vector.
       * @return <tt>true</tt> if this vector is greater than or equal to the passed one.
       */
      inline bool operator>=(const CVector3& c_vector3) const {
         return m_fX >= c_vector3.m_fX && m_fY >= c_vector3.m_fY && m_fZ >= c_vector3.m_fZ;
      }

      /**
       * Returns a negated copy of this vector.
       * @return A negated copy of this vector.
       */
      inline CVector3 operator-() const {
         return CVector3(-m_fX, -m_fY, -m_fZ);
      }

      /**
       * Sums the passed vector to this vector.
       * @param c_vector3 The other vector.
       * @returns A reference to this vector.
       */
      inline CVector3& operator+=(const CVector3& c_vector3) {
         m_fX += c_vector3.m_fX;
         m_fY += c_vector3.m_fY;
         m_fZ += c_vector3.m_fZ;
         return *this;
      }

      /**
       * Subtracts the passed vector from this vector.
       * @param c_vector3 The other vector.
       * @returns A reference to this vector.
       */
      inline CVector3& operator-=(const CVector3& c_vector3) {
         m_fX -= c_vector3.m_fX;
         m_fY -= c_vector3.m_fY;
         m_fZ -= c_vector3.m_fZ;
         return *this;
      }

      /**
       * Multiplies this vector by the given value.
       * @param f_value The wanted value.
       * @return A reference to this vector.
       */
      inline CVector3& operator*=(Real f_value) {
         m_fX *= f_value;
         m_fY *= f_value;
         m_fZ *= f_value;
         return *this;
      }

      /**
       * Divides this vector by the given value.
       * @param f_value The wanted value.
       * @return A reference to this vector.
       */
      inline CVector3& operator/=(Real f_value) {
         m_fX /= f_value;
         m_fY /= f_value;
         m_fZ /= f_value;
         return *this;
      }

      /**
       * Returns a new vector containing the sum between this vector and the passed one.
       * @param c_vector3 The other vector.
       * @return A new vector containing the sum between this vector and the passed one.
       */
      inline CVector3 operator+(const CVector3& c_vector3) const {
         CVector3 cResult(*this);
         cResult += c_vector3;
         return cResult;
      }

      /**
       * Returns a new vector containing the subtraction between this vector and the passed one.
       * @param c_vector3 The other vector.
       * @return A new vector containing the subtraction between this vector and the passed one.
       */
      inline CVector3 operator-(const CVector3& c_vector3) const {
         CVector3 cResult(*this);
         cResult -= c_vector3;
         return cResult;
      }

      /**
       * Returns a new vector containing the multiplication between this vector and the passed value.
       * @param f_value The wanted value.
       * @return A new vector containing the multiplication between this vector and the passed value.
       */
      inline CVector3 operator*(Real f_value) const {
         CVector3 cResult(*this);
         cResult *= f_value;
         return cResult;
      }

      /**
       * Returns a new vector containing the division between this vector and the passed value.
       * @param f_value The wanted value.
       * @return A new vector containing the division between this vector and the passed value.
       */
      inline CVector3 operator/(const Real f_value) const {
         CVector3 cResult(*this);
         cResult /= f_value;
         return cResult;
      }

      /**
       * Returns a new vector containing the multiplication between the passed value and the passed vector.
       * @param f_value The value.
       * @param c_vector3 The vector.
       * @return A new vector containing the multiplication between the passed value and the passed vector.
       */
      inline friend CVector3 operator*(Real f_value,
                                       const CVector3& c_vector3) {
         return c_vector3 * f_value;
      }

      /**
       * Serializes the contents of the passed vector onto a stream.
       * @param c_os The stream.
       * @param c_vector The vector.
       * @return The new state of the stream.
       */
      inline friend std::ostream& operator<<(std::ostream& c_os,
                                             const CVector3& c_vector3) {
         c_os << c_vector3.m_fX << ","
              << c_vector3.m_fY << ","
              << c_vector3.m_fZ;
         return c_os;
      }

      /**
       * Deserializes the contents of a stream and stores them into the passed vector.
       * @param c_is The stream.
       * @param c_vector The vector.
       * @return The new state of the stream.
       */
      inline friend std::istream& operator>>(std::istream& c_is,
                                             CVector3& c_vector3) {
         Real fValues[3];
         ParseValues<Real>(c_is, 3, fValues, ',');
         c_vector3.Set(fValues[0], fValues[1], fValues[2]);
         return c_is;
      }
      
   private:
      
      /** The Cartesian <em>x</em> coordinate */
      Real m_fX;

      /** The Cartesian <em>y</em> coordinate */
      Real m_fY;

      /** The Cartesian <em>z</em> coordinate */
      Real m_fZ;

      friend class CRotationMatrix3;
      friend class CTransformationMatrix3;

   };

   /****************************************/
   /****************************************/

   /** 
    * Computes the square distance between the passed vectors.
    * @param c_v1 The first vector
    * @param c_v2 The second vector
    * @return The square distance between the passed vectors
    */
   inline Real SquareDistance(const CVector3& c_v1, const CVector3& c_v2) {
      return (c_v1 - c_v2).SquareLength();
   }

   /** 
    * Computes the distance between the passed vectors.
    * @param c_v1 The first vector
    * @param c_v2 The second vector
    * @return The distance between the passed vectors
    */
   inline Real Distance(const CVector3& c_v1, const CVector3& c_v2) {
      return (c_v1 - c_v2).Length();
   }

/****************************************/
/****************************************/

}

#endif
