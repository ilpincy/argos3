 /**
 * @file <argos3/core/utility/math/matrix/rotationmatrix3.cpp>
 *
 * @brief Contains the implementation of a 3x3 rotation matrix
 *
 * @author Michael Allwright <michael.allwright@upb.de>
 */

#include "rotationmatrix3.h"
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   CQuaternion CRotationMatrix3::ToQuaternion() const
   {
	   const Real trace = 1.0f + m_pfValues[0] + m_pfValues[4] + m_pfValues[8];

	   if (trace > 0.00001f) {
		   const Real s = Sqrt(trace) * 2.0f;
		   return CQuaternion(
			   s / 4.0f,
			   (m_pfValues[7] - m_pfValues[5]) / s,
			   (m_pfValues[2] - m_pfValues[6]) / s,
			   (m_pfValues[3] - m_pfValues[1]) / s);
	   }
	   else if (m_pfValues[0] > m_pfValues[4] && m_pfValues[0] > m_pfValues[8]) {
		   const Real s = Sqrt(1.0f + m_pfValues[0] - m_pfValues[4] - m_pfValues[8]) * 2.0f;
		   return CQuaternion(
			   (m_pfValues[7] - m_pfValues[5]) / s,
			   s / 4.0f,
			   (m_pfValues[3] + m_pfValues[1]) / s,
			   (m_pfValues[2] + m_pfValues[6]) / s);
	   }
	   else if (m_pfValues[4] > m_pfValues[8]) {
		   const Real s = Sqrt(1.0f + m_pfValues[4] - m_pfValues[0] - m_pfValues[8]) * 2.0f;
		   return CQuaternion(
			   (m_pfValues[2] - m_pfValues[6]) / s,
			   (m_pfValues[3] + m_pfValues[1]) / s,
			   s / 4.0f,
			   (m_pfValues[7] + m_pfValues[5]) / s);
	   }
	   else {
		   const Real s = Sqrt(1.0f + m_pfValues[8] - m_pfValues[0] - m_pfValues[4]) * 2.0f;
		   return CQuaternion(
			   (m_pfValues[3] - m_pfValues[1]) / s,
			   (m_pfValues[2] + m_pfValues[6]) / s,
			   (m_pfValues[7] + m_pfValues[5]) / s,
			   s / 4.0f);
	   }
   }

   /****************************************/
   /****************************************/

   void CRotationMatrix3::SetFromMatrix(const CMatrix<3,3>& c_matrix) {
      m_pfValues[0] = c_matrix.m_pfValues[0];
      m_pfValues[1] = c_matrix.m_pfValues[1];
      m_pfValues[2] = c_matrix.m_pfValues[2];
      m_pfValues[3] = c_matrix.m_pfValues[3];
      m_pfValues[4] = c_matrix.m_pfValues[4];
      m_pfValues[5] = c_matrix.m_pfValues[5];
      m_pfValues[6] = c_matrix.m_pfValues[6];
      m_pfValues[7] = c_matrix.m_pfValues[7];
      m_pfValues[8] = c_matrix.m_pfValues[8];
   }
   
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
      m_pfValues[0] = 1.0f - (yy + zz);
      m_pfValues[1] = xy - wz;
      m_pfValues[2] = xz + wy;
      m_pfValues[3] = xy + wz;
      m_pfValues[4] = 1.0f - (xx + zz);
      m_pfValues[5] = yz - wx;
      m_pfValues[6] = xz - wy;
      m_pfValues[7] = yz + wx;
      m_pfValues[8] = 1.0f - (xx + yy);
   }

   /****************************************/
   /****************************************/
   
  void CRotationMatrix3::SetFromAngles(const CRadians& c_z_angle, const CRadians& c_y_angle, const CRadians& c_x_angle) {
      Real fSinX = Sin(c_x_angle);
      Real fCosX = Cos(c_x_angle);
      Real fSinY = Sin(c_y_angle);
      Real fCosY = Cos(c_y_angle);
      Real fSinZ = Sin(c_z_angle);
      Real fCosZ = Cos(c_z_angle);
      
      m_pfValues[0] = (fCosZ * fCosY);
      m_pfValues[1] = (fCosZ * fSinY * fSinX) - (fCosX * fSinZ);
      m_pfValues[2] = (fSinZ * fSinX) + (fCosZ * fCosX * fSinY);
      m_pfValues[3] = (fCosY * fSinZ);
      m_pfValues[4] = (fCosZ * fCosX) + (fSinZ * fSinY * fSinX);
      m_pfValues[5] = (fCosX * fSinZ * fSinY) - (fCosZ * fSinX);
      m_pfValues[6] = -fSinY;
      m_pfValues[7] = (fCosY * fSinX);
      m_pfValues[8] = (fCosY * fCosX);
   }
   
   /****************************************/
   /****************************************/
   
   void CRotationMatrix3::SetFromValues(Real f_value0, Real f_value1, Real f_value2,
                                        Real f_value3, Real f_value4, Real f_value5,
                                        Real f_value6, Real f_value7, Real f_value8) {
      m_pfValues[0] = f_value0;
      m_pfValues[1] = f_value1;
      m_pfValues[2] = f_value2;
      m_pfValues[3] = f_value3;
      m_pfValues[4] = f_value4;
      m_pfValues[5] = f_value5;
      m_pfValues[6] = f_value6;
      m_pfValues[7] = f_value7;
      m_pfValues[8] = f_value8;
   }

   /****************************************/
   /****************************************/
   
   CVector3 CRotationMatrix3::operator*(const CVector3& c_vector) const {
      return CVector3(m_pfValues[0]*c_vector.m_fX + m_pfValues[1]*c_vector.m_fY + m_pfValues[2]*c_vector.m_fZ,
	                   m_pfValues[3]*c_vector.m_fX + m_pfValues[4]*c_vector.m_fY + m_pfValues[5]*c_vector.m_fZ,
	                   m_pfValues[6]*c_vector.m_fX + m_pfValues[7]*c_vector.m_fY + m_pfValues[8]*c_vector.m_fZ);
   }
      
   /****************************************/
   /****************************************/
}
