/**
 * @file <argos3/core/utility/math/vector3.h>
 *
 * @brief This file provides the definition of
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

   class CVector3 {
   
   friend class CRotationMatrix3;
   friend class CTransformationMatrix3;

   public:

      static const CVector3 X;
      static const CVector3 Y;
      static const CVector3 Z;
      static const CVector3 ZERO;

      CVector3() :
         m_fX(0.0),
         m_fY(0.0),
         m_fZ(0.0) {
      }

      CVector3(Real f_x,
               Real f_y,
               Real f_z) :
         m_fX(f_x),
         m_fY(f_y),
         m_fZ(f_z) {
      }

      inline CVector3(Real f_radius,
                      const CRadians& c_inclination,
                      const CRadians& c_azimuth) {
         FromSphericalCoords(f_radius, c_inclination, c_azimuth);
      }

      inline Real GetX() const {
         return m_fX;
      }

      inline void SetX(const Real f_x) {
         m_fX = f_x;
      }

      inline Real GetY() const {
         return m_fY;
      }

      inline void SetY(const Real f_y) {
         m_fY = f_y;
      }

      inline Real GetZ() const {
         return m_fZ;
      }

      inline void SetZ(const Real f_z) {
         m_fZ = f_z;
      }

      inline void Set(const Real f_x,
                      const Real f_y,
                      const Real f_z) {
         m_fX = f_x;
         m_fY = f_y;
         m_fZ = f_z;
      }

      inline CVector3& FromSphericalCoords(Real f_radius,
                                           const CRadians& c_inclination,
                                           const CRadians& c_azimuth) {
         m_fX = f_radius * Sin(c_inclination) * Cos(c_azimuth);
         m_fY = f_radius * Sin(c_inclination) * Sin(c_azimuth);
         m_fZ = f_radius * Cos(c_inclination);
         return *this;
      }

      inline void ToSphericalCoords(Real& f_radius,
                                    CRadians& c_inclination,
                                    CRadians& c_azimuth) const {
         f_radius = Length();
         c_inclination = ACos(m_fZ / f_radius);
         c_azimuth = ATan2(m_fY, m_fX);
      }

      inline void ToSphericalCoordsHorizontal(Real& f_radius,
                                              CRadians& c_inclination,
                                              CRadians& c_azimuth) const {
         f_radius = Length();
         c_inclination = ASin(m_fZ / f_radius);
         c_azimuth = ATan2(m_fY, m_fX);
      }

      inline Real SquareLength() const {
         return Square(m_fX) + Square(m_fY) + Square(m_fZ);
      }

      inline Real Length() const {
         return ::sqrt(SquareLength());
      }

      inline CVector3& Normalize() {
         (*this) /= Length();
         return (*this);
      }

      inline CVector3& RotateX(const CRadians& c_angle) {
         Real fCos = Cos(c_angle);
         Real fSin = Sin(c_angle);
         Real fNewY = m_fY * fCos - m_fZ * fSin;
         Real fNewZ = m_fY * fSin + m_fZ * fCos;
         m_fY = fNewY;
         m_fZ = fNewZ;
         return *this;
      }

      inline CVector3& RotateY(const CRadians& c_angle) {
         Real fCos = Cos(c_angle);
         Real fSin = Sin(c_angle);
         Real fNewX = m_fX * fCos + m_fZ * fSin;
         Real fNewZ = m_fZ * fCos - m_fX * fSin;
         m_fX = fNewX;
         m_fZ = fNewZ;
         return *this;
      }

      inline CVector3& RotateZ(const CRadians& c_angle) {
         Real fCos = Cos(c_angle);
         Real fSin = Sin(c_angle);
         Real fNewX = m_fX * fCos - m_fY * fSin;
         Real fNewY = m_fX * fSin + m_fY * fCos;
         m_fX = fNewX;
         m_fY = fNewY;
         return *this;
      }

      /* Fast rotate by angle theta: the passed 2d vector is normalized
         and is < cos(theta), sin(theta) > */
      inline CVector3& RotateZ(const CVector2& c_vector) {
         Real fNewX = m_fX * c_vector.GetX() - m_fY * c_vector.GetY();
         Real fNewY = m_fX * c_vector.GetY() + m_fY * c_vector.GetX();
         m_fX = fNewX;
         m_fY = fNewY;
         return *this;
      }

      CVector3& Rotate(const CQuaternion& c_quaternion);

      inline CRadians GetZAngle() const {
         return ATan2(m_fY, m_fX);
      }

      inline Real DotProduct(const CVector3& c_vector3) const {
         return
            m_fX * c_vector3.m_fX +
            m_fY * c_vector3.m_fY +
            m_fZ * c_vector3.m_fZ;
      }

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

      inline CVector2 ProjectOntoXY() const {
         return CVector2(m_fX,m_fY);
      }

      inline CVector2 ProjectOntoYZ() const {
         return CVector2(m_fY,m_fZ);
      }

      inline CVector2 ProjectOntoXZ() const {
         return CVector2(m_fX,m_fZ);
      }

      inline CVector3& Negate() {
         m_fX = -m_fX;
         m_fY = -m_fY;
         m_fZ = -m_fZ;
         return *this;
      }

      inline Real operator[](UInt32 un_index) const {
         switch(un_index) {
            case 0: return m_fX;
            case 1: return m_fY;
            case 2: return m_fZ;
            default: THROW_ARGOSEXCEPTION("Real Vector3::operator[]: index out of bounds");
         }
      }

      inline Real& operator[](UInt32 un_index) {
         switch(un_index) {
            case 0: return m_fX;
            case 1: return m_fY;
            case 2: return m_fZ;
            default: THROW_ARGOSEXCEPTION("Real& Vector3::operator[]: index out of bounds");
         }
      }

      inline bool operator==(const CVector3& c_vector3) const {
         return m_fX == c_vector3.m_fX && m_fY == c_vector3.m_fY && m_fZ == c_vector3.m_fZ;
      }

      inline bool operator!=(const CVector3& c_vector3) const {
         return !((*this) == c_vector3);
      }

      inline bool operator<(const CVector3& c_vector3) const {
         return m_fX < c_vector3.m_fX && m_fY < c_vector3.m_fY && m_fZ < c_vector3.m_fZ;
      }

      inline bool operator<=(const CVector3& c_vector3) const {
         return m_fX <= c_vector3.m_fX && m_fY <= c_vector3.m_fY && m_fZ <= c_vector3.m_fZ;
      }

      inline bool operator>(const CVector3& c_vector3) const {
         return m_fX > c_vector3.m_fX && m_fY > c_vector3.m_fY && m_fZ > c_vector3.m_fZ;
      }

      inline bool operator>=(const CVector3& c_vector3) const {
         return m_fX >= c_vector3.m_fX && m_fY >= c_vector3.m_fY && m_fZ >= c_vector3.m_fZ;
      }

      inline CVector3 operator-() const {
         return CVector3(-m_fX, -m_fY, -m_fZ);
      }

      inline CVector3& operator+=(const CVector3& c_vector3) {
         m_fX += c_vector3.m_fX;
         m_fY += c_vector3.m_fY;
         m_fZ += c_vector3.m_fZ;
         return *this;
      }

      inline CVector3& operator-=(const CVector3& c_vector3) {
         m_fX -= c_vector3.m_fX;
         m_fY -= c_vector3.m_fY;
         m_fZ -= c_vector3.m_fZ;
         return *this;
      }

      inline CVector3& operator*=(Real f_value) {
         m_fX *= f_value;
         m_fY *= f_value;
         m_fZ *= f_value;
         return *this;
      }

      inline CVector3& operator/=(Real f_value) {
         m_fX /= f_value;
         m_fY /= f_value;
         m_fZ /= f_value;
         return *this;
      }

      inline CVector3 operator+(const CVector3& c_vector3) const {
         CVector3 cResult(*this);
         cResult += c_vector3;
         return cResult;
      }

      inline CVector3 operator-(const CVector3& c_vector3) const {
         CVector3 cResult(*this);
         cResult -= c_vector3;
         return cResult;
      }

      inline CVector3 operator*(Real f_value) const {
         CVector3 cResult(*this);
         cResult *= f_value;
         return cResult;
      }

      inline CVector3 operator/(const Real f_value) const {
         CVector3 cResult(*this);
         cResult /= f_value;
         return cResult;
      }

      inline friend CVector3 operator*(Real f_value,
                                       const CVector3& c_vector3) {
         return c_vector3 * f_value;
      }
      
      inline friend std::ostream& operator<<(std::ostream& os,
                                             const CVector3& c_vector3) {
         os << c_vector3.m_fX << ","
            << c_vector3.m_fY << ","
            << c_vector3.m_fZ;
         return os;
      }

      inline friend std::istream& operator>>(std::istream& is,
                                             CVector3& c_vector3) {
         Real fValues[3];
         ParseValues<Real>(is, 3, fValues, ',');
         c_vector3.Set(fValues[0], fValues[1], fValues[2]);
         return is;
      }
      
   private:
      
      Real m_fX;
      Real m_fY;
      Real m_fZ;

   };

   /****************************************/
   /****************************************/

   inline Real SquareDistance(const CVector3& c_v1, const CVector3& c_v2) {
      return (c_v1 - c_v2).SquareLength();
   }

   inline Real Distance(const CVector3& c_v1, const CVector3& c_v2) {
      return (c_v1 - c_v2).Length();
   }

/****************************************/
/****************************************/

}

#endif
