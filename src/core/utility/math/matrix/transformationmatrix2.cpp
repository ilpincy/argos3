 /**
 * @file <argos3/core/utility/math/matrix/transformationmatrix2.cpp>
 *
 * @brief Contains the implementation of a 3x3 matrix for 2D transformations
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "transformationmatrix2.h"

#include <argos3/core/utility/math/matrix/rotationmatrix2.h>
#include <argos3/core/utility/math/vector2.h>

namespace argos {

   /****************************************/
   /****************************************/   

   void CTransformationMatrix2::SetFromComponents(const CRotationMatrix2& c_rotation, const CVector2& c_translation) {
      SetRotationMatrix(c_rotation);
      SetTranslationVector(c_translation);
      m_pfValues[6] = 0.0f;
      m_pfValues[7] = 0.0f;
      m_pfValues[8] = 1.0f;
   }
   
   /****************************************/
   /****************************************/   

   void CTransformationMatrix2::SetFromMatrix(const CMatrix<3,3>& c_matrix) {
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

   void CTransformationMatrix2::SetFromValues(Real f_value0, Real f_value1, Real f_value2,
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

   void CTransformationMatrix2::SetRotationMatrix(const CRotationMatrix2& c_rotation) {
      m_pfValues[0] = c_rotation.m_pfValues[0]; m_pfValues[1] = c_rotation.m_pfValues[1];
      m_pfValues[3] = c_rotation.m_pfValues[2]; m_pfValues[4] = c_rotation.m_pfValues[3];
   }
   
   /****************************************/
   /****************************************/   

   CRotationMatrix2 CTransformationMatrix2::GetRotationMatrix() const {
      return CRotationMatrix2(m_pfValues[0], m_pfValues[1],
                              m_pfValues[3], m_pfValues[4]);
   }
   
   /****************************************/
   /****************************************/   

   void CTransformationMatrix2::SetTranslationVector(const CVector2& c_translation) {
      m_pfValues[2] = c_translation.m_fX;
      m_pfValues[5] = c_translation.m_fY;
   }
   
   /****************************************/
   /****************************************/   

   CVector2 CTransformationMatrix2::GetTranslationVector() const {
      return CVector2(m_pfValues[2], m_pfValues[5]);
   }

   /****************************************/
   /****************************************/
}
