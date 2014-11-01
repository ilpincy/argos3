#ifndef SIGNAL_PROCESSING_H
#define SIGNAL_PROCESSING_H

#include <argos3/core/utility/math/general.h>

namespace argos {

   /****************************************/
   /****************************************/

   /**
    * Calculates the mean, variance, and std deviation of a real-valued signal.
    */
   class CStats {

   public:
      
      CStats();
      
      /**
       * Returns the mean of the signal.
       * @return The mean of the signal.
       */
      inline Real GetMean() const {
         return m_fMean;
      }
      
      /**
       * Returns the variance of the signal.
       * At least two pieces of data must have been inserted for the variance to
       * be different from the default value, which is 0.
       * @return The variance of the signal.
       */
      Real GetVariance() const;

      /**
       * Returns the std deviation of the signal.
       * At least two pieces of data must have been inserted for the std deviation
       * to be different from the default value, which is 0.
       * @return The std deviation of the signal.
       */
      inline Real GetStdDev() const {
         return Sqrt(GetVariance());
      }

      /**
       * Appends a new piece of data and recalculates the statistics.
       * @param f_value The value to append.
       */
      void Append(Real f_value);
      
   private:
      
      UInt64 m_unCounter;
      Real m_fMean;
      Real m_fSumOfSquareDiff;
   };

   /****************************************/
   /****************************************/

   /**
    * A simple infinite-impulse response filter for real-valued signals.
    * @see http://en.wikipedia.org/wiki/Low-pass_filter#Simple_infinite_impulse_response_filter
    */
   class CRCLowPassFilter {

   public:

      /**
       * Class constructor.
       * @param f_smoothing_factor The smoothing factor of the RC flter.
       */
      CRCLowPassFilter(Real f_smoothing_factor);

      /**
       * Applies the filter to the given value.
       * @param f_input The new value of the signal to filter.
       * @return The filtered output of the signal to filter.
       */
      Real Filter(Real f_input);

      /**
       * Resets the filter.
       */
      void Reset();

   private:

      Real m_fSmoothingFactor;
      Real m_fPreviousOutput;
      bool m_bInitialized;
   };
   
   /****************************************/
   /****************************************/
   
}

#endif

