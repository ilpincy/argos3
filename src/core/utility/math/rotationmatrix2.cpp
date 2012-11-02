/**
 * @file argos3/core/utility/math/rotationmatrix2.cpp
 *
 * @brief Contains the implementation of a 2x2 rotation matrix
 *
 * @author Michael Allwright <michael.allwright@upb.de>
 */
 
#include "rotationmatrix2.h"
#include "angles.h"

namespace argos {

   /****************************************/
   /****************************************/

   void CRotationMatrix2::SetFromAngle(const CRadians& c_angle) {
      Real cos_angle = Cos(c_angle);
      Real sin_angle = Sin(c_angle);

      /* Set values */
      m_fValues[0] =  cos_angle;
      m_fValues[1] = -sin_angle;
      m_fValues[2] =  sin_angle;
      m_fValues[3] =  cos_angle;
   }
   
   /****************************************/
   /****************************************/

}
