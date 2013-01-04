 /**
 * @file <argos3/core/utility/math/matrix/rotationmatrix3.h>
 *
 * @brief Contains the definition of a 3x3 rotation matrix
 *
 * @author Michael Allwright <michael.allwright@upb.de>
 */

#ifndef ROTATION_MATRIX3_H
#define ROTATION_MATRIX3_H

namespace argos {
   class CQuaternion;
   class CVector3;
}

#include "squarematrix.h"

namespace argos {

   class CRotationMatrix3 : public CSquareMatrix<3> {
   
   friend class CTransformationMatrix3;
   
   public:
      CRotationMatrix3() : CSquareMatrix<3>() {
         SetIdentityMatrix();
      }
      
      CRotationMatrix3(const CMatrix<3,3>& c_matrix) : CSquareMatrix<3>() {
         SetFromMatrix(c_matrix);
      }
      
      CRotationMatrix3(const CQuaternion& c_quaternion) : CSquareMatrix<3>() {
         SetFromQuaternion(c_quaternion);
      }
      
      CRotationMatrix3(const CRadians& c_z_angle, const CRadians& c_y_angle, const CRadians& c_x_angle) : CSquareMatrix<3>() {
         SetFromAngles(c_z_angle, c_y_angle, c_x_angle);
      }
      
      CRotationMatrix3(Real f_value0, Real f_value1, Real f_value2,
                       Real f_value3, Real f_value4, Real f_value5,
                       Real f_value6, Real f_value7, Real f_value8) : CSquareMatrix<3>() {
         SetFromValues(f_value0, f_value1, f_value2,
                       f_value3, f_value4, f_value5,
                       f_value6, f_value7, f_value8);               
      }
      
      void SetFromMatrix(const CMatrix<3,3>& c_matrix);
      
      void SetFromQuaternion(const CQuaternion& c_quaternion);
      
      void SetFromAngles(const CRadians& c_x_angle, const CRadians& c_y_angle, const CRadians& c_z_angle);
      
      void SetFromValues(Real f_value0, Real f_value1, Real f_value2,
                         Real f_value3, Real f_value4, Real f_value5,
                         Real f_value6, Real f_value7, Real f_value8);
      
      CQuaternion ToQuaternion() const;
      
      CVector3 operator*(const CVector3& c_vector) const;
   };
}

#endif
