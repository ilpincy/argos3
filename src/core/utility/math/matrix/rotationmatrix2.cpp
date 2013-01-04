 /**
 * @file <argos3/core/utility/math/matrix/rotationmatrix2.cpp>
 *
 * @brief Contains the implementation of a 2x2 rotation matrix
 *
 * @author Michael Allwright <michael.allwright@upb.de>
 */
 
#include "rotationmatrix2.h"
#include <argos3/core/utility/math/angles.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CRotationMatrix2::SetFromAngle(const CRadians& c_angle) {
      Real cos_angle = Cos(c_angle);
      Real sin_angle = Sin(c_angle);

      /* Set values */
      m_pfValues[0] =  cos_angle;
      m_pfValues[1] = -sin_angle;
      m_pfValues[2] =  sin_angle;
      m_pfValues[3] =  cos_angle;
   }
   
   /****************************************/
   /****************************************/
   
   void CRotationMatrix2::SetFromMatrix(const CMatrix<2,2>& c_matrix) {
      m_pfValues[0] =  c_matrix.m_pfValues[0];
      m_pfValues[1] =  c_matrix.m_pfValues[1];
      m_pfValues[2] =  c_matrix.m_pfValues[2];
      m_pfValues[3] =  c_matrix.m_pfValues[3];
   }
   
   /****************************************/
   /****************************************/
   
   void CRotationMatrix2::SetFromValues(Real f_value0, Real f_value1,
                                        Real f_value2, Real f_value3) {
      m_pfValues[0] = f_value0;
      m_pfValues[1] = f_value1;
      m_pfValues[2] = f_value2;
      m_pfValues[3] = f_value3;
   }
   
   /****************************************/
   /****************************************/

}
