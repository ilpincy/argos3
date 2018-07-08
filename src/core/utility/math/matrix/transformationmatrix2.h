 /**
 * @file <argos3/core/utility/math/matrix/transformationmatrix2.h>
 *
 * @brief Contains the definition of a 3x3 matrix for 2D transformations
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef TRANSFORMATION_MATRIX2_H
#define TRANSFORMATION_MATRIX2_H

namespace argos {
   class CVector2;
   class CRotationMatrix2;
}

#include <argos3/core/utility/math/matrix/squarematrix.h>

namespace argos {

   class CTransformationMatrix2 : public CSquareMatrix<3> {
      
   public:
      CTransformationMatrix2() : CSquareMatrix<3>() {
         SetIdentityMatrix();
      }
      
      CTransformationMatrix2(const CMatrix<3,3>& c_matrix) : CSquareMatrix<3>() {
         SetFromMatrix(c_matrix);
      }
      
      CTransformationMatrix2(const CRotationMatrix2& c_rotation, const CVector2& c_translation) : CSquareMatrix<3>() {
         SetFromComponents(c_rotation, c_translation);
      }
      
      CTransformationMatrix2(Real f_value0, Real f_value1, Real f_value2,
                             Real f_value3, Real f_value4, Real f_value5,
                             Real f_value6, Real f_value7, Real f_value8) : CSquareMatrix<3>() {
         SetFromValues(f_value0, f_value1, f_value2,
                       f_value3, f_value4, f_value5,
                       f_value6, f_value7, f_value8);               
      }
      
      void SetFromMatrix(const CMatrix<3,3>& c_matrix);
      
      void SetFromComponents(const CRotationMatrix2& c_rotation, const CVector2& c_translation);
      
      void SetFromValues(Real f_value0, Real f_value1, Real f_value2,
                         Real f_value3, Real f_value4, Real f_value5,
                         Real f_value6, Real f_value7, Real f_value8);
      
      void SetRotationMatrix(const CRotationMatrix2& c_rotation);
      
      CRotationMatrix2 GetRotationMatrix() const;
      
      void SetTranslationVector(const CVector2& c_translation);
      
      CVector2 GetTranslationVector() const;
   };
}

#endif
