 /**
 * @file <argos3/core/utility/math/matrix/transformationmatrix3.h>
 *
 * @brief Contains the definition of a 4x4 matrix for 3D transformations
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef TRANSFORMATION_MATRIX3_H
#define TRANSFORMATION_MATRIX3_H

namespace argos {
   class CVector3;
   class CRotationMatrix3;
}

#include <argos3/core/utility/math/matrix/squarematrix.h>

namespace argos {

   class CTransformationMatrix3 : public CSquareMatrix<4> {
   
   public:
     
      CTransformationMatrix3() : CSquareMatrix<4>() {
         SetIdentityMatrix();
      }
      
      CTransformationMatrix3(const CMatrix<4,4>& c_matrix) : CSquareMatrix<4>() {
         SetFromMatrix(c_matrix);
      }
      
      CTransformationMatrix3(const CRotationMatrix3& c_rotation, const CVector3& c_translation) : CSquareMatrix<4>() {
         SetFromComponents(c_rotation, c_translation);
      }
      
      CTransformationMatrix3(Real f_value0,  Real f_value1,  Real f_value2,  Real f_value3,
                             Real f_value4,  Real f_value5,  Real f_value6,  Real f_value7, 
                             Real f_value8,  Real f_value9,  Real f_value10, Real f_value11,
                             Real f_value12, Real f_value13, Real f_value14, Real f_value15) : CSquareMatrix<4>() {
         SetFromValues(f_value0,  f_value1,  f_value2,  f_value3,
                       f_value4,  f_value5,  f_value6,  f_value7, 
                       f_value8,  f_value9,  f_value10, f_value11,
                       f_value12, f_value13, f_value14, f_value15);              
      }

      void SetFromMatrix(const CMatrix<4,4>& c_matrix);
      
      void SetFromComponents(const CRotationMatrix3& c_rotation, const CVector3& c_translation);
      
      void SetFromValues(Real f_value0,  Real f_value1,  Real f_value2,  Real f_value3,
                         Real f_value4,  Real f_value5,  Real f_value6,  Real f_value7, 
                         Real f_value8,  Real f_value9,  Real f_value10, Real f_value11,
                         Real f_value12, Real f_value13, Real f_value14, Real f_value15);
      
      void SetRotationMatrix(const CRotationMatrix3& c_rotation);
      
      CRotationMatrix3 GetRotationMatrix() const;
      
      void SetTranslationVector(const CVector3& c_translation);
      
      CVector3 GetTranslationVector() const;
      
      CVector3 operator*(const CVector3& c_vector) const;

      CTransformationMatrix3 operator*(const CTransformationMatrix3& c_matrix) const;
   };
}

#endif
