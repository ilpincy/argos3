/**
 * @file <argos3/core/utility/math/rng.cpp>
 *
 * @brief This file provides the definition of
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "rng.h"
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <cstring>
#include <limits>
#include <cmath>

namespace argos {

   /****************************************/
   /****************************************/

   /* Period parameters */
   static const SInt32 N = 624;
   static const SInt32 M = 397;
   static const UInt32 MATRIX_A = 0x9908b0dfUL;   /* constant vector a */
   static const UInt32 UPPER_MASK = 0x80000000UL; /* most significant w-r bits */
   static const UInt32 LOWER_MASK = 0x7fffffffUL; /* least significant r bits */
   static const CRange<UInt32> INT_RANGE = CRange<UInt32>(0, 0xFFFFFFFFUL);

   std::map<std::string, CRandom::CCategory*> CRandom::m_mapCategories;

   /* Checks that a category exists. It internally creates an iterator that points to the category, if found.  */
#define CHECK_CATEGORY(category)                                        \
   std::map<std::string, CCategory*>::iterator itCategory = m_mapCategories.find(category); \
   if(itCategory == m_mapCategories.end()) {                            \
      THROW_ARGOSEXCEPTION("CRandom:: can't find category \"" << category << "\"."); \
   }

   /****************************************/
   /****************************************/

   CRandom::CRNG::CRNG(UInt32 un_seed) :
      m_unSeed(un_seed),
      m_punState(new UInt32[N]),
      m_nIndex(N+1) {
      Reset();
   }
   
   /****************************************/
   /****************************************/
   
   CRandom::CRNG::CRNG(const CRNG& c_rng) :
      m_unSeed(c_rng.m_unSeed),
      m_punState(new UInt32[N]),
      m_nIndex(c_rng.m_nIndex) {
      ::memcpy(m_punState, c_rng.m_punState, N * sizeof(UInt32));
   }

   /****************************************/
   /****************************************/

   CRandom::CRNG::~CRNG() {
      delete[] m_punState;
   }

   /****************************************/
   /****************************************/

   void CRandom::CRNG::Reset() {
      m_punState[0]= m_unSeed & 0xffffffffUL;
      for (m_nIndex = 1; m_nIndex < N; ++m_nIndex) {
         m_punState[m_nIndex] = 
            (1812433253UL * (m_punState[m_nIndex-1] ^ (m_punState[m_nIndex-1] >> 30)) + m_nIndex); 
         m_punState[m_nIndex] &= 0xffffffffUL;
      }
   }
   
   /****************************************/
   /****************************************/

   bool CRandom::CRNG::Bernoulli(Real f_true) {
      return Uniform32bit() < f_true * INT_RANGE.GetMax();
   }

   /****************************************/
   /****************************************/

   CRadians CRandom::CRNG::Uniform(const CRange<CRadians>& c_range) {
      CRadians cRetVal;
      INT_RANGE.MapValueIntoRange(cRetVal, Uniform32bit(), c_range);
      return cRetVal;
   }
   
   /****************************************/
   /****************************************/

   Real CRandom::CRNG::Uniform(const CRange<Real>& c_range) {
      Real fRetVal;
      INT_RANGE.MapValueIntoRange(fRetVal, Uniform32bit(), c_range);
      return fRetVal;
   }
   
   /****************************************/
   /****************************************/

   SInt32 CRandom::CRNG::Uniform(const CRange<SInt32>& c_range) {
      SInt32 nRetVal;
      INT_RANGE.MapValueIntoRange(nRetVal, Uniform32bit(), c_range);
      return nRetVal;
   }
   
   /****************************************/
   /****************************************/

   UInt32 CRandom::CRNG::Uniform(const CRange<UInt32>& c_range) {
      UInt32 unRetVal;
      INT_RANGE.MapValueIntoRange(unRetVal, Uniform32bit(), c_range);
      return unRetVal;
   }
   
   /****************************************/
   /****************************************/

   Real CRandom::CRNG::Exponential(Real f_mean) {
      static CRange<Real> fRange(0.0f, 1.0f);
      return -Log(Uniform(fRange)) * f_mean;
   }
   
   /****************************************/
   /****************************************/

   UInt32 CRandom::CRNG::Poisson(Real f_mean)
   {
      /* This is the Devroye method. 
         see http://www.nrbook.com/devroye/Devroye_files/chapter_six.pdf
      */
      UInt32 unRetVal = 0;
      Real fP, fS, fU;
      fP = Exp(-f_mean);
      fS = fP;
      fU = Uniform(CRange<Real>(0.0f, 1.0f));
      while(fU > fS) {
         unRetVal += 1;
         fP *= f_mean / unRetVal;
         fS += fP;
      }
      return unRetVal;
   }

   /****************************************/
   /****************************************/

   Real CRandom::CRNG::Gaussian(Real f_std_dev,
                                Real f_mean) {
      /* This is the Box-Muller method in its cartesian variant
         see http://www.dspguru.com/dsp/howtos/how-to-generate-white-gaussian-noise
      */
      static CRange<Real> fRange(-1.0f, 1.0f);
      Real fNum1, fNum2;
      Real fSquare;
      do {
         fNum1 = Uniform(fRange);
         fNum2 = Uniform(fRange);
         fSquare = fNum1 * fNum1 + fNum2 * fNum2;
      } while(fSquare >= 1);
      return f_mean + f_std_dev * fNum1 * Sqrt(-2.0f * Log(fSquare) / fSquare);
   }

   /****************************************/
   /****************************************/

   Real CRandom::CRNG::Rayleigh(Real f_sigma) {
      /* Draw a number uniformly from (0,1) --- bounds excluded */
      static CRange<Real> cUnitRange(0.0f, 1.0f);
      Real fValue;
      do {
         fValue = Uniform(cUnitRange);
      }
      while(! cUnitRange.WithinMinBoundExcludedMaxBoundExcluded(fValue));
      /* Calculate the value to return from the definition of Rayleigh distribution
       * http://en.wikipedia.org/wiki/Rayleigh_distribution#Generating_Rayleigh-distributed_random_variates
       */
      return f_sigma * Sqrt(-2.0f * Log(fValue));
   }

/****************************************/
   /****************************************/

   Real CRandom::CRNG::Lognormal(Real f_sigma, Real f_mu) {
      /* Draw a number uniformly from (0,1) */
      Real fValue;
      fValue = Gaussian(1,0);
      /* Calculate the value to return from the definition of Lognormal distribution
       * http://en.wikipedia.org/wiki/Log-normal_distribution#Generating_log-normally_distributed_random_variates
       */
      return std::exp(f_mu + f_sigma * fValue);
   }
   
   /****************************************/
   /****************************************/

   UInt32 CRandom::CRNG::Uniform32bit() {
      UInt32 y;
      static UInt32 mag01[2] = { 0x0UL, MATRIX_A };
      /* mag01[x] = x * MATRIX_A  for x=0,1 */
      
      if (m_nIndex >= N) { /* generate N words at one time */
         SInt32 kk;
         for (kk = 0; kk < N - M; ++kk) {
            y = (m_punState[kk] & UPPER_MASK) | (m_punState[kk+1] & LOWER_MASK);
            m_punState[kk] = m_punState[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
         }
         for (; kk < N - 1; ++kk) {
            y = (m_punState[kk] & UPPER_MASK) | (m_punState[kk+1] & LOWER_MASK);
            m_punState[kk] = m_punState[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
         }
         y = (m_punState[N-1] & UPPER_MASK) | (m_punState[0] & LOWER_MASK);
         m_punState[N-1] = m_punState[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];
         
         m_nIndex = 0;
      }
      
      y = m_punState[m_nIndex++];
      
      /* Tempering */
      y ^= (y >> 11);
      y ^= (y << 7) & 0x9d2c5680UL;
      y ^= (y << 15) & 0xefc60000UL;
      y ^= (y >> 18);
      
      return y;
   }
   
   /****************************************/
   /****************************************/

   CRandom::CCategory::CCategory(const std::string& str_id,
                                 UInt32 un_seed) :
      m_strId(str_id),
      m_unSeed(un_seed),
      m_cSeeder(un_seed),
      m_cSeedRange(1, std::numeric_limits<UInt32>::max()) {}

   /****************************************/
   /****************************************/

   CRandom::CCategory::~CCategory() {
      while(! m_vecRNGList.empty()) {
         delete m_vecRNGList.back();
         m_vecRNGList.pop_back();
      }
   }

   /****************************************/
   /****************************************/

   void CRandom::CCategory::SetSeed(UInt32 un_seed) {
      m_unSeed = un_seed;
      m_cSeeder.SetSeed(m_unSeed);
   }

   /****************************************/
   /****************************************/

   CRandom::CRNG* CRandom::CCategory::CreateRNG() {
      /* Get seed from internal RNG */
      UInt32 unSeed = m_cSeeder.Uniform(m_cSeedRange);
      /* Create new RNG */
      m_vecRNGList.push_back(new CRNG(unSeed));
      return m_vecRNGList.back();
   }

   /****************************************/
   /****************************************/

   void CRandom::CCategory::ResetRNGs() {
      /* Reset internal RNG */
      m_cSeeder.Reset();
      ReseedRNGs();
      /* Reset the RNGs */
      for(size_t i = 0; i < m_vecRNGList.size(); ++i) {
         m_vecRNGList[i]->Reset();
      }
   }

   /****************************************/
   /****************************************/

   void CRandom::CCategory::ReseedRNGs() {
      for(size_t i = 0; i < m_vecRNGList.size(); ++i) {
         /* Get seed from internal RNG */
         m_vecRNGList[i]->SetSeed(m_cSeeder.Uniform(m_cSeedRange));
      }
   }

   /****************************************/
   /****************************************/

   bool CRandom::CreateCategory(const std::string& str_category,
                                UInt32 un_seed) {
      /* Is there a category already? */
      std::map<std::string, CCategory*>::iterator itCategory = m_mapCategories.find(str_category);
      if(itCategory == m_mapCategories.end()) {
         /* No, create it */
         m_mapCategories.insert(
            std::pair<std::string,
            CRandom::CCategory*>(str_category,
                                 new CRandom::CCategory(str_category,
                                                        un_seed)));
         return true;
      }
      return false;
   }

   /****************************************/
   /****************************************/

   CRandom::CCategory& CRandom::GetCategory(const std::string& str_category) {
      CHECK_CATEGORY(str_category);
      return *(itCategory->second);
   }

   /****************************************/
   /****************************************/

   bool CRandom::ExistsCategory(const std::string& str_category) {
      try {
         CHECK_CATEGORY(str_category);
         return true;
      }
      catch(CARGoSException& ex) {
         return false;
      }
   }

   /****************************************/
   /****************************************/

   void CRandom::RemoveCategory(const std::string& str_category) {
      CHECK_CATEGORY(str_category);
      delete itCategory->second;
      m_mapCategories.erase(itCategory);
   }

   /****************************************/
   /****************************************/

   CRandom::CRNG* CRandom::CreateRNG(const std::string& str_category) {
      CHECK_CATEGORY(str_category);
      return itCategory->second->CreateRNG();
   }
   
   /****************************************/
   /****************************************/

   UInt32 CRandom::GetSeedOf(const std::string& str_category) {
      CHECK_CATEGORY(str_category);
      return itCategory->second->GetSeed();
   }

   /****************************************/
   /****************************************/

   void CRandom::SetSeedOf(const std::string& str_category,
                           UInt32 un_seed) {
      CHECK_CATEGORY(str_category);
      itCategory->second->SetSeed(un_seed);
   }

   /****************************************/
   /****************************************/

   void CRandom::Reset() {
      for(std::map<std::string, CCategory*>::iterator itCategory = m_mapCategories.begin();
          itCategory != m_mapCategories.end();
          ++itCategory) {
         itCategory->second->ResetRNGs();
      }
   }

   /****************************************/
   /****************************************/

}
