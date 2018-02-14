 /**
 * @file <argos3/core/utility/math/matrix/matrix.h>
 *
 * @brief Contains the definition of a NxM matrix
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <argos3/core/utility/math/general.h>
#include <argos3/core/utility/math/angles.h>
#include <cmath>
#include <iomanip>

namespace argos {

   template <UInt32 ROWS, UInt32 COLS>
   class CMatrix {

   /* Matrices of different dimensions can access each others data */
   template <UInt32 SMROWS, UInt32 SMCOLS> friend class CMatrix;
   
   /* The specified derived classes are allowed to access members of the base class */
   friend class CRotationMatrix2;
   friend class CTransformationMatrix2;
   friend class CRotationMatrix3;
   friend class CTransformationMatrix3;
      
   public:
      CMatrix() {
         SetZero();
      }
      
      CMatrix(const Real* pf_values) {
         Set(pf_values);
      }
      
      CMatrix(const CMatrix<ROWS,COLS>& c_matrix) {
         Set(c_matrix.m_pfValues);
      }
      
      CMatrix<ROWS,COLS>& operator=(const CMatrix<ROWS,COLS>& c_matrix) {
         if(this != &c_matrix) {
            Set(c_matrix.m_pfValues);
         }
         return *this;
      }
          
      inline Real& operator()(UInt32 un_row,
                              UInt32 un_col) {
         ARGOS_ASSERT(un_row < ROWS && un_col < COLS,
                      "Matrix index out of bounds: un_row = " <<
                      un_row <<
                      ", un_col = " <<
                      un_col);
         return m_pfValues[un_row * COLS + un_col];
      }
      
      inline Real operator()(UInt32 un_row,
                             UInt32 un_col) const {
         ARGOS_ASSERT(un_row < ROWS && un_col < COLS,
                      "Matrix index out of bounds: un_row = " <<
                      un_row <<
                      ", un_col = " <<
                      un_col);
         return m_pfValues[un_row * COLS + un_col];
      }
      
      inline Real operator()(UInt32 un_idx) const {
         ARGOS_ASSERT(un_idx < ROWS * COLS,
                      "Matrix index out of bounds: un_idx = " <<
                      un_idx);
         return m_pfValues[un_idx];
      }

      inline Real& operator()(UInt32 un_idx) {
         ARGOS_ASSERT(un_idx < ROWS * COLS,
                      "Matrix index out of bounds: un_idx = " <<
                      un_idx);
         return m_pfValues[un_idx];
      }
      
      void Set(const Real* f_values) {
         for(UInt32 i = 0; i < ROWS * COLS; i++)
            m_pfValues[i] = f_values[i];
      }

      void SetZero() {
         for(UInt32 i = 0; i < ROWS * COLS; i++)
            m_pfValues[i] = 0.0;
      }
      
      CMatrix<COLS, ROWS> GetTransposed() const {
         Real fNewValues[COLS * ROWS];
         for(UInt32 i = 0; i < ROWS; i++)
            for(UInt32 j = 0; j < COLS; j++)
               fNewValues[j * ROWS + i] = m_pfValues[i * COLS + j];

         return CMatrix<COLS, ROWS>(fNewValues);
      }
      
      template <UInt32 SMROWS, UInt32 SMCOLS>
      void GetSubMatrix(CMatrix<SMROWS, SMCOLS>& c_sub_matrix,
                                            UInt32 un_offset_row, 
                                            UInt32 un_offset_col) const {
         ARGOS_ASSERT((SMROWS - 1 + un_offset_row) < ROWS &&
                      (SMCOLS - 1 + un_offset_col) < COLS,
                      "Submatrix range is out of bounds: cannot extract a " <<
                      SMROWS << "x" << SMCOLS << " submatrix from a " <<
                      ROWS << "x" << COLS << " matrix with offsets " <<
                      " un_offset_row = " <<
                      un_offset_row <<
                      ", un_offset_col = " <<
                      un_offset_col);
                      
         for(UInt32 i = 0; i < SMROWS; i++)
            for(UInt32 j = 0; j < SMCOLS; j++)
               c_sub_matrix.m_pfValues[i * SMCOLS + j] = m_pfValues[(i + un_offset_row) * COLS + (j + un_offset_col)];
      }
      
      bool operator==(const CMatrix<ROWS, COLS>& c_matrix) const {
         for(UInt32 i = 0; i < ROWS * COLS; i++) {
            if(m_pfValues[i] != c_matrix.m_pfValues[i])
               return false;
         }
         return true;
      }

      CMatrix<ROWS, COLS>& operator+=(const CMatrix<ROWS, COLS>& c_matrix) {
         for(UInt32 i = 0; i < ROWS * COLS; i++) {
            m_pfValues[i] += c_matrix.m_pfValues[i];
         }
         return *this;
      }
      
      CMatrix<ROWS, COLS>& operator-=(const CMatrix<ROWS, COLS>& c_matrix) {
         for(UInt32 i = 0; i < ROWS * COLS; i++) {
            m_pfValues[i] -= c_matrix.m_pfValues[i];
         }
         return *this;
      }
      
      CMatrix<ROWS, COLS>& operator*=(Real f_scale) {
         for(UInt32 i = 0; i < ROWS * COLS; i++) {
            m_pfValues[i] *= f_scale;
         }
         return *this;
      }
      
      CMatrix<ROWS, COLS> operator+(const CMatrix<ROWS, COLS>& c_matrix) const {
         CMatrix<ROWS, COLS> cResult = (*this);
         cResult += c_matrix;
         return cResult;
      }

      CMatrix<ROWS, COLS> operator-(const CMatrix<ROWS, COLS>& c_matrix) const {
         CMatrix<ROWS, COLS> cResult = (*this);
         cResult -= c_matrix;
         return cResult;
      }

      CMatrix<ROWS, COLS> operator-() const {
         CMatrix<ROWS, COLS> cResult;
         cResult -= (*this);
         return cResult;
      }
      
      CMatrix<ROWS, COLS>& operator*=(const CMatrix<COLS, COLS>& c_matrix) {
         Real fNewValues[ROWS * COLS];
         for(UInt32 i = 0; i < ROWS; i++) {
            for(UInt32 j = 0; j < COLS; j++) {
               fNewValues[i * COLS + j] = 0.0f;
               for(UInt32 k = 0; k < COLS; k++) {
                  fNewValues[i * COLS + j] += m_pfValues[i * COLS + k] * c_matrix.m_pfValues[k * COLS + j];
               }
            }
         }
         Set(fNewValues);
         return *this;
      }
      
      template <UInt32 OTRCOLS>
      CMatrix<ROWS, OTRCOLS> operator*(const CMatrix<COLS, OTRCOLS>& c_matrix) const {
         Real fNewValues[ROWS * OTRCOLS];
         for(UInt32 i = 0; i < ROWS; i++) {
            for(UInt32 j = 0; j < OTRCOLS; j++) {
               fNewValues[i * OTRCOLS + j] = 0.0f;
               for(UInt32 k = 0; k < COLS; k++) {
                  fNewValues[i * OTRCOLS + j] += m_pfValues[i * COLS + k] * c_matrix.m_pfValues[k * OTRCOLS + j];
               }
            }
         }
         return CMatrix<ROWS, OTRCOLS>(fNewValues);
      }

      friend std::ostream& operator<<(std::ostream& c_os,
                                      const CMatrix& c_matrix) {
         
         std::ios_base::fmtflags unInitalFlags = c_os.flags();
         std::streamsize nInitalPrec = c_os.precision();
         
         c_os.setf(std::ios::fixed);
         c_os.precision(2);
         
         for(UInt32 i = 0; i < ROWS; i++) {
            c_os << "| ";
            for(UInt32 j = 0; j < COLS; j++) {
               c_os << std::setw(6) << c_matrix(i, j) << " ";
            }
            c_os << "|" << std::endl;
         }
         
         c_os.flags(unInitalFlags);
         c_os.precision(nInitalPrec);
         return c_os;
      }
      
   protected:

      Real m_pfValues[ROWS * COLS];

   };
}

#endif
