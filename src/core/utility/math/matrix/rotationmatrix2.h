 /**
 * @file <argos3/core/utility/math/matrix/rotationmatrix2.h>
 *
 * @brief Contains the definition of a 2x2 rotation matrix
 *
 * @author Michael Allwright <michael.allwright@upb.de>
 */
 
#ifndef ROTATION_MATRIX2_H
#define ROTATION_MATRIX2_H

namespace argos {
   class CRadians;
}

#include "squarematrix.h"
#include <argos3/core/utility/math/vector2.h>

namespace argos {

   class CRotationMatrix2 : public CSquareMatrix<2> {
   
   friend class CTransformationMatrix2;
      
   public:
      CRotationMatrix2() : CSquareMatrix<2>() {
         SetIdentityMatrix();
      }
      
      CRotationMatrix2(const CMatrix<2,2>& c_matrix) {
         SetFromMatrix(c_matrix);
      }
      
      CRotationMatrix2(const CRadians& c_angle) : CSquareMatrix<2>() {
         SetFromAngle(c_angle);
      }
      
      CRotationMatrix2(Real f_value0, Real f_value1,
                       Real f_value2, Real f_value3) : CSquareMatrix<2>() {
         SetFromValues(f_value0, f_value1,
                       f_value2, f_value3);
      }
      
      void SetFromMatrix(const CMatrix<2,2>& c_matrix);
      
      void SetFromAngle(const CRadians& c_angle);
      
      void SetFromValues(Real f_value0, Real f_value1,
                         Real f_value2, Real f_value3);
      
      CVector2 operator*(const CVector2& c_vector) const {
         return CVector2(m_pfValues[0]*c_vector.m_fX + m_pfValues[1]*c_vector.m_fY,
		                   m_pfValues[2]*c_vector.m_fX + m_pfValues[3]*c_vector.m_fY);
      }
   };
}

#endif
