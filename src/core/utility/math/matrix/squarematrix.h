 /**
 * @file <argos3/core/utility/math/matrix/squarematrix.h>
 *
 * @brief Contains the definition of a NxN square matrix
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef SQUARE_MATRIX_H
#define SQUARE_MATRIX_H

#include <argos3/core/utility/math/matrix/matrix.h>

namespace argos {

   template <UInt32 DIM>
   class CSquareMatrix : public CMatrix<DIM, DIM> {
      
   public:
      CSquareMatrix() : CMatrix<DIM, DIM>() {}

      CSquareMatrix(const CMatrix<DIM, DIM>& c_other) :
         CMatrix<DIM, DIM>(c_other) {}

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

      /*
         Calculate the determinant
      */
      Real GetDeterminant() const {
         Real fDet = 0.0;
         CSquareMatrix<DIM - 1> cTempMatrix;
         for(UInt32 unColTempA = 0; unColTempA < DIM; unColTempA++) {
            for(UInt32 unRow = 1; unRow < DIM; unRow++) {
               UInt32 unColTempB = 0;
               for (UInt32 unCol = 0; unCol < DIM; unCol++) {
                  if (unCol == unColTempA)
                     continue;
                  cTempMatrix(unRow - 1, unColTempB) = CMatrix<DIM, DIM>::m_pfValues[unRow * DIM + unCol];
                  unColTempB++;
               }
            }
            fDet += std::pow(-1.0, unColTempA + 2u) * CMatrix<DIM, DIM>::m_pfValues[unColTempA] * cTempMatrix.GetDeterminant();
         }
         return fDet;
      }

      /**
       *  Find the cofactor matrix
       */
      CSquareMatrix<DIM> GetCofactorMatrix() const {
         CSquareMatrix<DIM> cCofactorMatrix;
         CSquareMatrix<DIM - 1> cTempMatrix;
         for(UInt32 unCofactorCol = 0; unCofactorCol < DIM; unCofactorCol++) {
            for(UInt32 unCofactorRow = 0; unCofactorRow < DIM; unCofactorRow++) {
               /* Form the adjoint */
               UInt32 unTempRow = 0;
               for(UInt32 unRow = 0; unRow < DIM; unRow++) {
                  if(unRow != unCofactorRow) {
                     UInt32 unTempCol = 0;
                     for(UInt32 unCol = 0; unCol < DIM; unCol++) {
                        if(unCol != unCofactorCol) {
                           cTempMatrix(unTempRow, unTempCol) = CMatrix<DIM, DIM>::m_pfValues[unRow * DIM + unCol];
                           unTempCol++;
                        }
                     }
                     unTempRow++;
                  }
               }
               /* Fill in the elements of the cofactor matrix */
               cCofactorMatrix(unCofactorRow, unCofactorCol) = 
                  std::pow(-1.0, unCofactorRow + unCofactorCol + 2u) * cTempMatrix.GetDeterminant();
            }
         }
         return cCofactorMatrix;
      }

      CSquareMatrix<DIM> GetTransposed() const {
         CSquareMatrix<DIM> cTransposedMatrix;
         for(UInt32 i = 0; i < DIM; i++) {
            for(UInt32 j = 0; j < DIM; j++) {
               cTransposedMatrix(j, i) = CMatrix<DIM, DIM>::m_pfValues[i * DIM + j];
            }
         }
         return cTransposedMatrix;
      }

      CSquareMatrix<DIM> GetInverse() const {
         CSquareMatrix<DIM> cInverseMatrix = GetCofactorMatrix().GetTransposed();
         cInverseMatrix *= 1.0 / GetDeterminant();
         return cInverseMatrix;
      }

   };

   template<>
   Real CSquareMatrix<1>::GetDeterminant() const;

   template<>
   Real CSquareMatrix<2>::GetDeterminant() const;

   template<>
   CSquareMatrix<2> CSquareMatrix<2>::GetCofactorMatrix() const;
}

#endif
