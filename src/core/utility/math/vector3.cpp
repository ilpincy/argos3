/**
 * @file <argos3/core/utility/math/vector3.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "vector3.h"
#include "quaternion.h"

namespace argos {

   /****************************************/
   /****************************************/

   const CVector3 CVector3::X(1.0, 0.0, 0.0);
   const CVector3 CVector3::Y(0.0, 1.0, 0.0);
   const CVector3 CVector3::Z(0.0, 0.0, 1.0);
   const CVector3 CVector3::ZERO;

   /****************************************/
   /****************************************/

   CVector3& CVector3::Rotate(const CQuaternion& c_quaternion) {
      CQuaternion cResult;
      cResult = c_quaternion;
      cResult *= CQuaternion(0.0f, m_fX, m_fY, m_fZ);
      cResult *= c_quaternion.Inverse();
      m_fX = cResult.GetX();
      m_fY = cResult.GetY();
      m_fZ = cResult.GetZ();
      return *this;
   }

   /****************************************/
   /****************************************/

}
