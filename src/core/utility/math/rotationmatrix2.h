/**
 * @file core/utility/math/rotationmatrix2.h
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
#include "vector2.h"

namespace argos {

   class CRotationMatrix2 : public CSquareMatrix<2> {
      
   public:
      CRotationMatrix2() : CSquareMatrix<2>() {
         SetIdentityMatrix();
      }
      
      CRotationMatrix2(const CRadians& c_angle) : CSquareMatrix<2>() {
         SetFromAngle(c_angle);
      }
      
      void SetFromAngle(const CRadians& c_angle);
      
      CVector2 operator*(const CVector2& c_vector) const {
         return CVector2(m_fValues[0]*c_vector.m_fX + m_fValues[1]*c_vector.m_fY,
		                   m_fValues[2]*c_vector.m_fX + m_fValues[3]*c_vector.m_fY);
      }
   };
}

#endif
