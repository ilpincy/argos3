#include "signal_processing.h"

namespace argos {

   /****************************************/
   /****************************************/
   
   CStats::CStats() :
      m_unCounter(0),
      m_fMean(0.0f),
      m_fSumOfSquareDiff(0.0f) {}

   /****************************************/
   /****************************************/

   void CStats::Append(Real f_value) {
      /*
       * Algorithm taken from:
       * http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Online_algorithm
       */
      ++m_unCounter;
      Real fDelta = f_value - m_fMean;
      m_fMean += fDelta / static_cast<Real>(m_unCounter);
      m_fSumOfSquareDiff += fDelta * (f_value - m_fMean);
   }

   /****************************************/
   /****************************************/
   
   Real CStats::GetVariance() const {
      return m_unCounter > 1 ?
         (m_fSumOfSquareDiff / static_cast<Real>(m_unCounter - 1))
         :
         0.0f;
   }

   /****************************************/
   /****************************************/

   CRCLowPassFilter::CRCLowPassFilter(Real f_smoothing_factor) :
      m_fSmoothingFactor(f_smoothing_factor),
      m_bInitialized(false) {}
   
   /****************************************/
   /****************************************/
   
   Real CRCLowPassFilter::Filter(Real f_input) {
      if(m_bInitialized) {
         m_fPreviousOutput = f_input + m_fSmoothingFactor * (m_fPreviousOutput - f_input);   		
      }
      else {
         m_fPreviousOutput = f_input;
         m_bInitialized = true;
      }
      return m_fPreviousOutput;
   }

   /****************************************/
   /****************************************/
   
   void CRCLowPassFilter::Reset() {
      m_bInitialized = false;
   }

   /****************************************/
   /****************************************/
   
}
