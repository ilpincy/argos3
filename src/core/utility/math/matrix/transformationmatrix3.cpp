 /**
 * @file <argos3/core/utility/math/matrix/transformationmatrix3.cpp>
 *
 * @brief Contains the implementation of a 3D transformation matrix (4x4)
 *
 * @author Michael Allwright <michael.allwright@upb.de>
 */

#include "transformationmatrix3.h"
#include "rotationmatrix3.h"
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   /****************************************/
   /****************************************/   

   void CTransformationMatrix3::SetFromComponents(const CRotationMatrix3& c_rotation, const CVector3& c_translation) {
      /* Set the rotation elements */
      SetRotationMatrix(c_rotation);
      
      /* Set the translation elements */
      SetTranslationVector(c_translation);
      
      /* Set the bottom row elements */
      m_pfValues[12] = 0.0f; m_pfValues[13] = 0.0f; m_pfValues[14] = 0.0f; m_pfValues[15] = 1.0f;
   }

   /****************************************/
   /****************************************/   

   void CTransformationMatrix3::SetFromMatrix(const CMatrix<4,4>& c_matrix) {
      m_pfValues[ 0] = c_matrix.m_pfValues[ 0];
      m_pfValues[ 1] = c_matrix.m_pfValues[ 1];
      m_pfValues[ 2] = c_matrix.m_pfValues[ 2];
      m_pfValues[ 3] = c_matrix.m_pfValues[ 3];
      m_pfValues[ 4] = c_matrix.m_pfValues[ 4];
      m_pfValues[ 5] = c_matrix.m_pfValues[ 5];
      m_pfValues[ 6] = c_matrix.m_pfValues[ 6];
      m_pfValues[ 7] = c_matrix.m_pfValues[ 7];
      m_pfValues[ 8] = c_matrix.m_pfValues[ 8];
      m_pfValues[ 9] = c_matrix.m_pfValues[ 9];
      m_pfValues[10] = c_matrix.m_pfValues[10];
      m_pfValues[11] = c_matrix.m_pfValues[11];
      m_pfValues[12] = c_matrix.m_pfValues[12];
      m_pfValues[13] = c_matrix.m_pfValues[13];
      m_pfValues[14] = c_matrix.m_pfValues[14];
      m_pfValues[15] = c_matrix.m_pfValues[15];
   }
   
   /****************************************/
   /****************************************/   

   void CTransformationMatrix3::SetFromValues(Real f_value0,  Real f_value1,  Real f_value2,  Real f_value3,
                                              Real f_value4,  Real f_value5,  Real f_value6,  Real f_value7, 
                                              Real f_value8,  Real f_value9,  Real f_value10, Real f_value11,
                                              Real f_value12, Real f_value13, Real f_value14, Real f_value15) {
      m_pfValues[ 0] = f_value0;
      m_pfValues[ 1] = f_value1;
      m_pfValues[ 2] = f_value2;
      m_pfValues[ 3] = f_value3;
      m_pfValues[ 4] = f_value4;
      m_pfValues[ 5] = f_value5;
      m_pfValues[ 6] = f_value6;
      m_pfValues[ 7] = f_value7;
      m_pfValues[ 8] = f_value8;
      m_pfValues[ 9] = f_value9;
      m_pfValues[10] = f_value10;
      m_pfValues[11] = f_value11;
      m_pfValues[12] = f_value12;
      m_pfValues[13] = f_value13;
      m_pfValues[14] = f_value14;
      m_pfValues[15] = f_value15;
   }
   
   /****************************************/
   /****************************************/   

   void CTransformationMatrix3::SetRotationMatrix(const CRotationMatrix3& c_rotation) {
      m_pfValues[0] = c_rotation.m_pfValues[0]; m_pfValues[1] = c_rotation.m_pfValues[1]; m_pfValues[ 2] = c_rotation.m_pfValues[2];
      m_pfValues[4] = c_rotation.m_pfValues[3]; m_pfValues[5] = c_rotation.m_pfValues[4]; m_pfValues[ 6] = c_rotation.m_pfValues[5];
      m_pfValues[8] = c_rotation.m_pfValues[6]; m_pfValues[9] = c_rotation.m_pfValues[7]; m_pfValues[10] = c_rotation.m_pfValues[8];
   }
   
   /****************************************/
   /****************************************/   

   const CRotationMatrix3 CTransformationMatrix3::GetRotationMatrix() const {
      return CRotationMatrix3(m_pfValues[0], m_pfValues[1], m_pfValues[2],
                              m_pfValues[4], m_pfValues[5], m_pfValues[6],
                              m_pfValues[8], m_pfValues[9], m_pfValues[10]);
   }
   
   /****************************************/
   /****************************************/   

   void CTransformationMatrix3::SetTranslationVector(const CVector3& c_translation) {
      m_pfValues[ 3] = c_translation.m_fX;
      m_pfValues[ 7] = c_translation.m_fY;
      m_pfValues[11] = c_translation.m_fZ; 
   }
   
   /****************************************/
   /****************************************/   

   const CVector3 CTransformationMatrix3::GetTranslationVector() const {
      return CVector3(m_pfValues[3], m_pfValues[7], m_pfValues[11]);
   }

   /****************************************/
   /****************************************/   

   CVector3 CTransformationMatrix3::operator*(const CVector3& c_vector) const {
      return CVector3(m_pfValues[0]*c_vector.m_fX + m_pfValues[1]*c_vector.m_fY + m_pfValues[2]*c_vector.m_fZ + m_pfValues[3],
	                   m_pfValues[4]*c_vector.m_fX + m_pfValues[5]*c_vector.m_fY + m_pfValues[6]*c_vector.m_fZ + m_pfValues[7],
	                   m_pfValues[8]*c_vector.m_fX + m_pfValues[9]*c_vector.m_fY + m_pfValues[10]*c_vector.m_fZ + m_pfValues[11]);
   }

   /****************************************/
   /****************************************/
}
