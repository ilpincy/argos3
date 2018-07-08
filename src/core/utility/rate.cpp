#include "rate.h"
#include <argos3/core/utility/logging/argos_log.h>

using namespace argos;

/****************************************/
/****************************************/

CRate::CRate(Real f_rate) {
   SetRate(f_rate);
   ::gettimeofday(&m_tPast, NULL);
}

/****************************************/
/****************************************/

void CRate::Sleep() {
   /* Get current time */
   ::gettimeofday(&m_tNow, NULL);
   /* Calculate difference between past call and now */
   timersub(&m_tNow, &m_tPast, &m_tDiff);
   UInt64 unMicroSecDiff =
      m_tDiff.tv_sec * 1000000 +
      m_tDiff.tv_usec;
   /* Sleep if necessary */
   if(unMicroSecDiff < m_unNominalPeriod) {
      m_tSleepPeriod.tv_sec = (m_unNominalPeriod - unMicroSecDiff) / 1e6;
      m_tSleepPeriod.tv_nsec = (m_unNominalPeriod - unMicroSecDiff) * 1000;
      ::nanosleep(&m_tSleepPeriod, NULL);
   }
   else {
      LOGERR << "[WARNING] Nominal rate "
             << m_fNominalRate
             << " loops per sec delayed by "
             << (unMicroSecDiff - m_unNominalPeriod)
             << " microseconds"
             << std::endl;
   }
   /* Update past for next call */
   ::gettimeofday(&m_tPast, NULL);
}

/****************************************/
/****************************************/

void CRate::SetRate(Real f_rate) {
   m_fNominalRate = Abs(f_rate);
   m_unNominalPeriod = 1e6 / m_fNominalRate;
}

/****************************************/
/****************************************/
