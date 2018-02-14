 /**
 * @file <argos3/core/utility/math/matrix/rotationmatrix3.cpp>
 *
 * @brief Contains the implementation of a 3x3 rotation matrix
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "rotationmatrix3.h"

#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   /****************************************/
   /****************************************/
   
   CRotationMatrix3::operator CQuaternion() const {
      Real fTrace = 1.0f + m_pfValues[0] + m_pfValues[4] + m_pfValues[8];
      if (fTrace > 0.00001f) {
         Real fS = Sqrt(fTrace) * 2.0f;
         return CQuaternion(
            fS / 4.0f,
            (m_pfValues[7] - m_pfValues[5]) / fS,
            (m_pfValues[2] - m_pfValues[6]) / fS,
            (m_pfValues[3] - m_pfValues[1]) / fS);
      }
      else if (m_pfValues[0] > m_pfValues[4] && m_pfValues[0] > m_pfValues[8]) {
         Real fS = Sqrt(1.0f + m_pfValues[0] - m_pfValues[4] - m_pfValues[8]) * 2.0f;
         return CQuaternion(
            (m_pfValues[7] - m_pfValues[5]) / fS,
            fS / 4.0f,
            (m_pfValues[3] + m_pfValues[1]) / fS,
            (m_pfValues[2] + m_pfValues[6]) / fS);
      }
      else if (m_pfValues[4] > m_pfValues[8]) {
         Real fS = Sqrt(1.0f + m_pfValues[4] - m_pfValues[0] - m_pfValues[8]) * 2.0f;
         return CQuaternion(
            (m_pfValues[2] - m_pfValues[6]) / fS,
            (m_pfValues[3] + m_pfValues[1]) / fS,
            fS / 4.0f,
            (m_pfValues[7] + m_pfValues[5]) / fS);
      }
      else {
         Real fS = Sqrt(1.0f + m_pfValues[8] - m_pfValues[0] - m_pfValues[4]) * 2.0f;
         return CQuaternion(
            (m_pfValues[3] - m_pfValues[1]) / fS,
            (m_pfValues[2] + m_pfValues[6]) / fS,
            (m_pfValues[7] + m_pfValues[5]) / fS,
            fS / 4.0f);
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
