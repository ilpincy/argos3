#include "rate.h"
#include <argos3/core/utility/logging/argos_log.h>

using namespace argos;

/****************************************/
/****************************************/

CRate::CRate(Real f_rate) {
  SetRate(f_rate);
}

/****************************************/
/****************************************/

UInt64 CRate::ElapsedUS() const {
  /* Get current time */
  ::timeval tNow;
  ::gettimeofday(&tNow, NULL);
  /* Calculate difference between past call and now */
  ::timeval tDiff;
  timersub(&tNow, &m_tPast, &tDiff);
  return tDiff.tv_sec * 1000000 + tDiff.tv_usec;  
}

/****************************************/
/****************************************/

Real CRate::ElapsedS() const {
  UInt64 unValue = ElapsedUS();
  Real fValue = static_cast<Real>(unValue) / 1e6;
  return fValue;
}

/****************************************/
/****************************************/

void CRate::Sleep() {
  UInt64 unMicroSecDiff = ElapsedUS();
  /* Sleep if necessary */
  if(unMicroSecDiff < m_unNominalPeriod) {
    ::timespec tSleepPeriod;
    tSleepPeriod.tv_sec = (m_unNominalPeriod - unMicroSecDiff) / 1e6;
    tSleepPeriod.tv_nsec = (m_unNominalPeriod - unMicroSecDiff) * 1000;
    ::nanosleep(&tSleepPeriod, NULL);
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
  ::gettimeofday(&m_tPast, NULL);
}

/****************************************/
/****************************************/
