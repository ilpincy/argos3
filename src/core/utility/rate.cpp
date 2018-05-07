#include "rate.h"
#include <argos3/core/utility/logging/argos_log.h>

using namespace argos;

/****************************************/
/****************************************/

CRate::CRate(Real f_rate) {
   SetRate(f_rate);
   ::gettimeofday(&m_tPast, NULL);
   DEBUG("start time: %ld:%ld\n", m_tPast.tv_sec, m_tPast.tv_usec);
}

/****************************************/
/****************************************/

void CRate::Sleep() {
   /* Get current time */
   ::gettimeofday(&m_tNow, NULL);
   DEBUG("current time: %ld:%ld\n", m_tNow.tv_sec, m_tNow.tv_usec);
   /* Calculate difference between past call and now */
   timersub(&m_tNow, &m_tPast, &m_tDiff);
   UInt64 unMicroSecDiff =
      m_tDiff.tv_sec * 1000000 +
      m_tPast.tv_usec;
   DEBUG("diff time: %ld:%ld\n", m_tDiff.tv_sec, m_tDiff.tv_usec);
   /* Sleep if necessary */
   if(unMicroSecDiff < m_unNominalPeriod) {
      m_tSleepPeriod.tv_sec = (m_unNominalPeriod - unMicroSecDiff) / 1e6;
      m_tSleepPeriod.tv_nsec = (m_unNominalPeriod - unMicroSecDiff) * 1000;
      DEBUG("sleep period: %ld:%ld\n", m_tSleepPeriod.tv_sec, m_tSleepPeriod.tv_nsec);
      ::nanosleep(&m_tSleepPeriod, NULL);
   }
   /* Update past for next call */
   m_tPast = m_tNow;
}

/****************************************/
/****************************************/

void CRate::SetRate(Real f_rate) {
   m_fNominalRate = Abs(f_rate);
   m_unNominalPeriod = 1e6 / m_fNominalRate;
}

/****************************************/
/****************************************/
