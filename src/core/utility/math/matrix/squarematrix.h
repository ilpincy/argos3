 /**
 * @file <argos3/core/utility/math/matrix/squarematrix.h>
 *
 * @brief Contains the definition of a NxN templated square matrix
 *
 * @author Michael Allwright <michael.allwright@upb.de>
 */

#ifndef SQUARE_MATRIX_H
#define SQUARE_MATRIX_H

#include "matrix.h"

namespace argos {

   template <UInt32 DIM>
   class CSquareMatrix : public CMatrix<DIM, DIM> {
      
   public:
      CSquareMatrix() : CMatrix<DIM, DIM>() {}
      
      void SetIdentityMatrix() {
         for(UInt32 i = 0; i < DIM; i++) {
            for(UInt32 j = 0; j < DIM; j++) {
               if(i == j) {
                  CMatrix<DIM, DIM>::m_pfValues[i * DIM + j] = 1;
               }
               else {
                  CMatrix<DIM, DIM>::m_pfValues[i * DIM + j] = 0;
               }
            }
         }
      }
      
      bool IsIdentityMatrix() {
         bool bIsIdentMat = true;
         for(UInt32 i = 0; i < DIM; i++) {
            for(UInt32 j = 0; j < DIM; j++) {
               if(i == j) {
                  if(CMatrix<DIM, DIM>::m_pfValues[i * DIM + j] != 1) {
                     bIsIdentMat = false;
                     break;
                  }
               }
               else {
                  if(CMatrix<DIM, DIM>::m_pfValues[i * DIM + j] != 0) {
                     bIsIdentMat = false;
                     break;
                  }
               }
            }
         }
         return bIsIdentMat;
      }
   };
}

#endif
