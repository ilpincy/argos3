 /**
 * @file <argos3/core/utility/math/matrix/squarematrix.cpp>
 *
 * @brief Contains the implementation of a NxN matrix
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "squarematrix.h"

namespace argos {

   /****************************************/
   /****************************************/   

   template<>
   Real CSquareMatrix<1>::GetDeterminant() const {
      return m_pfValues[0];
   }

   /****************************************/
   /****************************************/   

   template<>
   Real CSquareMatrix<2>::GetDeterminant() const {
      return m_pfValues[0] * m_pfValues[3] - m_pfValues[2] * m_pfValues[1];
   }

   /****************************************/
   /****************************************/   

   template<>
   CSquareMatrix<2> CSquareMatrix<2>::GetCofactorMatrix() const {
      Real pfValues[] = {m_pfValues[3], -m_pfValues[2], -m_pfValues[1], m_pfValues[0]};
      return CSquareMatrix<2>(pfValues);
   }

   /****************************************/
   /****************************************/   
}
