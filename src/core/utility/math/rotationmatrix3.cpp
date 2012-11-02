/**
 * @file argos3/core/utility/math/rotationmatrix3.cpp
 *
 * @brief Contains the implementation of a 3x3 rotation matrix
 *
 * @author Michael Allwright <michael.allwright@upb.de>
 */

#include "rotationmatrix3.h"
#include "quaternion.h"

namespace argos {

   /****************************************/
   /****************************************/

   void CRotationMatrix3::SetFromQuaternion(const CQuaternion& c_quaternion) {
      /* This code is adapted from the Bullet source code */
      Real d = c_quaternion.SquareLength();
      Real s = 2.0f / d;
      Real xs = c_quaternion.GetX() * s,   ys = c_quaternion.GetY() * s,   zs = c_quaternion.GetZ() * s;
      Real wx = c_quaternion.GetW() * xs,  wy = c_quaternion.GetW() * ys,  wz = c_quaternion.GetW() * zs;
      Real xx = c_quaternion.GetX() * xs,  xy = c_quaternion.GetX() * ys,  xz = c_quaternion.GetX() * zs;
      Real yy = c_quaternion.GetY() * ys,  yz = c_quaternion.GetY() * zs,  zz = c_quaternion.GetZ() * zs;
      
      /* Set values */
      m_fValues[0] = 1.0f - (yy + zz);
      m_fValues[1] = xy - wz;
      m_fValues[2] = xz + wy;
      m_fValues[3] = xy + wz;
      m_fValues[4] = 1.0f - (xx + zz);
      m_fValues[5] = yz - wx;
      m_fValues[6] = xz - wy;
      m_fValues[7] = yz + wx;
      m_fValues[8] = 1.0f - (xx + yy);
   }

   /****************************************/
   /****************************************/

}
