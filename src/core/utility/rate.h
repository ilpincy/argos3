#ifndef RATE_H
#define RATE_H

#include <time.h>
#include <sys/time.h>
#include <argos3/core/utility/math/general.h>

namespace argos {

   class CRate {
      
   public:

      CRate(Real f_rate);
      
      ~CRate() {}

      void Sleep();
      
      inline Real GetRate() const {
         return m_fNominalRate;
      }
      
      void SetRate(Real f_rate);

   private:

      Real m_fNominalRate;
      UInt64 m_unNominalPeriod;
      ::timespec m_tSleepPeriod;
      ::timeval m_tPast;
      ::timeval m_tNow;
      ::timeval m_tDiff;
      
   };
   
}

#endif // RATE_H
