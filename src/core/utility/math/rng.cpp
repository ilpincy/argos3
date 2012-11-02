/**
 * @file argos3/core/utility/math/rng.cpp
 *
 * @brief This file provides the definition of
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "rng.h"
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <cstring>
#include <limits>
#include <cmath>

#ifndef CROSSCOMPILING
#   include <gsl/gsl_randist.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   std::map<std::string, CRandom::CCategory*> CRandom::m_mapCategories;

#ifndef CROSSCOMPILING
   /* Creates an array of all the available generator types, terminated by a null pointer */
   const gsl_rng_type** CRandom::m_pptRNGTypes = gsl_rng_types_setup();
#endif

   /* Checks that a category exists. It internally creates an iterator that points to the category, if found.  */
#define CHECK_CATEGORY(category)                                        \
   std::map<std::string, CCategory*>::iterator itCategory = m_mapCategories.find(category); \
   if(itCategory == m_mapCategories.end()) {                            \
      THROW_ARGOSEXCEPTION("CRandom:: can't find category \"" << category << "\"."); \
   }

   /****************************************/
   /****************************************/

   CRandom::CRNG::CRNG(UInt32 un_seed,
                       const std::string& str_type) :
      m_unSeed(un_seed),
      m_strType(str_type),
      m_ptRNG(NULL),
#ifdef CROSSCOMPILING
      m_pchRNGState(NULL),
#endif
      m_pcIntegerRNGRange(NULL) {
      CreateRNG();
   }
   
   /****************************************/
   /****************************************/
   
   CRandom::CRNG::CRNG(CByteArray& c_buffer) :
      m_ptRNG(NULL),
#ifdef CROSSCOMPILING
      m_pchRNGState(NULL),
#endif
      m_pcIntegerRNGRange(NULL) {
      /* Get the seed of the generator */
      c_buffer >> m_unSeed;
      /* Get the type of the generator */
      c_buffer >> m_strType;
      /* Create the RNG */
      CreateRNG();
      /* Restore the state of the generator */
#ifndef CROSSCOMPILING
      size_t unStateSize = gsl_rng_size(m_ptRNG);
      UInt8* pStateBuffer = new UInt8[unStateSize];
      c_buffer.FetchBuffer(pStateBuffer, unStateSize);
      ::memcpy(gsl_rng_state(m_ptRNG), pStateBuffer, unStateSize);
      /* Cleanup */
      delete[] pStateBuffer;
#else
      c_buffer.FetchBuffer(reinterpret_cast<UInt8*>(m_pchRNGState), 256);
      setstate_r(m_pchRNGState, m_ptRNG);
#endif
   }

   /****************************************/
   /****************************************/

   CRandom::CRNG::CRNG(const CRNG& c_rng) :
      m_unSeed(c_rng.m_unSeed),
      m_strType(c_rng.m_strType),
      m_ptRNG(NULL),
#ifdef CROSSCOMPILING
      m_pchRNGState(NULL),
#endif
      m_pcIntegerRNGRange(new CRange<UInt32>(*c_rng.m_pcIntegerRNGRange)) {
      /* Clone RNG of original */
#ifndef CROSSCOMPILING
      m_ptRNG = gsl_rng_clone(c_rng.m_ptRNG);
#else
      m_ptRNG = new random_data;
      ::memset(m_ptRNG, 0, sizeof(random_data));
      m_pchRNGState = new char[256];
      SInt32 nError = initstate_r(m_unSeed, m_pchRNGState, 256, m_ptRNG);
      if(nError != 0) {
         THROW_ARGOSEXCEPTION("Unable to create random number generator (initstate_r returned " << nError << ").");
      }
      ::memcpy(m_pchRNGState, c_rng.m_pchRNGState, 256);
      setstate_r(m_pchRNGState, m_ptRNG);
#endif
   }

   /****************************************/
   /****************************************/

   CRandom::CRNG::~CRNG() {
      DisposeRNG();
   }

   /****************************************/
   /****************************************/

   void CRandom::CRNG::CreateRNG() {
#ifndef CROSSCOMPILING
      /* Look for RNG type in the RNG type list */
      bool bFound = false;
      const gsl_rng_type** pptRNGType = GetRNGTypes();
      while((!bFound) && (pptRNGType != NULL)) {
         if(m_strType == (*pptRNGType)->name) {
            bFound = true;
         }
         else {
            ++pptRNGType;
         }
      }
      if(!bFound) {
         /* RNG type not found, error! */
         THROW_ARGOSEXCEPTION("Unknown random number generator type '" << m_strType << "'.");
      }
      /* We found the wanted RNG type, create the actual RNG */
      m_ptRNG = gsl_rng_alloc(*pptRNGType);
      gsl_rng_set(m_ptRNG, m_unSeed);
      /* Initialize RNG range */
      m_pcIntegerRNGRange = new CRange<UInt32>(gsl_rng_min(m_ptRNG),
                                               gsl_rng_max(m_ptRNG));
#else
      /* Initialize RNG */
      m_ptRNG = new random_data;
      ::memset(m_ptRNG, 0, sizeof(random_data));
      m_pchRNGState = new char[256];
      SInt32 nError = initstate_r(m_unSeed, m_pchRNGState, 256, m_ptRNG);
      if(nError != 0) {
         THROW_ARGOSEXCEPTION("Unable to create random number generator (initstate_r returned " << nError << ").");
      }
      /* Initialize RNG range */
      m_pcIntegerRNGRange = new CRange<UInt32>(0, RAND_MAX);
#endif
   }

   /****************************************/
   /****************************************/

   void CRandom::CRNG::DisposeRNG() {
#ifndef CROSSCOMPILING
      gsl_rng_free(m_ptRNG);
#else
      delete m_ptRNG;
      delete[] m_pchRNGState;
#endif
      delete m_pcIntegerRNGRange;
   }

   /****************************************/
   /****************************************/

   void CRandom::CRNG::SaveState(CByteArray& c_buffer) {
      /* Dump the seed of the generator */
      c_buffer << m_unSeed;
      /* Dump the type of the generator */
      c_buffer << m_strType;
      /* Dump the state of the generator */
#ifndef CROSSCOMPILING
      UInt8* pStateBuffer = reinterpret_cast<UInt8*>(gsl_rng_state(m_ptRNG));
      c_buffer.AddBuffer(pStateBuffer, gsl_rng_size(m_ptRNG));
#else
      c_buffer.AddBuffer(reinterpret_cast<UInt8*>(m_pchRNGState), 256);
#endif
   }

   /****************************************/
   /****************************************/

   void CRandom::CRNG::LoadState(CByteArray& c_buffer) {
      /* Get the seed of the generator */
      c_buffer >> m_unSeed;
      /* Get the type of the generator */
      std::string strReadType;
      c_buffer >> strReadType;
#ifndef CROSSCOMPILING
      /* Check if the type read and the current type match */
      if(strReadType != m_strType) {
         /* The types don't match.
            We need to recreate the generator with the right type */
         m_strType = strReadType;
         DisposeRNG();
         CreateRNG();
      }
      /* Get the state of the generator */
      size_t unStateSize = gsl_rng_size(m_ptRNG);
      UInt8* pStateBuffer = new UInt8[unStateSize];
      c_buffer.FetchBuffer(pStateBuffer, unStateSize);
      /* Restore the state of the RNG */
      ::memcpy(gsl_rng_state(m_ptRNG), pStateBuffer, unStateSize);
      /* Cleanup */
      delete[] pStateBuffer;
#else
      c_buffer.FetchBuffer(reinterpret_cast<UInt8*>(m_pchRNGState), 256);
      setstate_r(m_pchRNGState, m_ptRNG);
#endif
   }
   
   /****************************************/
   /****************************************/

   void CRandom::CRNG::Reset() {
#ifndef CROSSCOMPILING
      gsl_rng_set(m_ptRNG, m_unSeed);
#else
      initstate_r(m_unSeed, m_pchRNGState, 256, m_ptRNG);
#endif
   }
   
   /****************************************/
   /****************************************/

   bool CRandom::CRNG::Bernoulli(Real f_true) {
#ifndef CROSSCOMPILING
      return gsl_rng_uniform(m_ptRNG) < f_true;
#else
      UInt32 unNumber;
      random_r(m_ptRNG, reinterpret_cast<int32_t*>(&unNumber));
      return m_pcIntegerRNGRange->NormalizeValue(unNumber) < f_true;
#endif
   }

   /****************************************/
   /****************************************/

   CRadians CRandom::CRNG::Uniform(const CRange<CRadians>& c_range) {
#ifndef CROSSCOMPILING
      return c_range.GetMin() + gsl_rng_uniform(m_ptRNG) * c_range.GetSpan();
#else
      UInt32 unNumber;
      random_r(m_ptRNG, reinterpret_cast<int32_t*>(&unNumber));
      CRadians cRetVal;
      m_pcIntegerRNGRange->MapValueIntoRange(cRetVal, unNumber, c_range);
      return cRetVal;
#endif
   }
   
   /****************************************/
   /****************************************/

   Real CRandom::CRNG::Uniform(const CRange<Real>& c_range) {
#ifndef CROSSCOMPILING
      return c_range.GetMin() + gsl_rng_uniform(m_ptRNG) * c_range.GetSpan();
#else
      UInt32 unNumber;
      random_r(m_ptRNG, reinterpret_cast<int32_t*>(&unNumber));
      Real fRetVal;
      m_pcIntegerRNGRange->MapValueIntoRange(fRetVal, unNumber, c_range);
      return fRetVal;
#endif
   }
   
   /****************************************/
   /****************************************/

   SInt32 CRandom::CRNG::Uniform(const CRange<SInt32>& c_range) {
#ifndef CROSSCOMPILING
      SInt32 nRetVal;
      m_pcIntegerRNGRange->MapValueIntoRange(nRetVal, gsl_rng_get(m_ptRNG), c_range);
      return nRetVal;
#else
      UInt32 unNumber;
      random_r(m_ptRNG, reinterpret_cast<int32_t*>(&unNumber));
      SInt32 nRetVal;
      m_pcIntegerRNGRange->MapValueIntoRange(nRetVal, unNumber, c_range);
      return nRetVal;
#endif
   }
   
   /****************************************/
   /****************************************/

   UInt32 CRandom::CRNG::Uniform(const CRange<UInt32>& c_range) {
#ifndef CROSSCOMPILING
      UInt32 unRetVal;
      m_pcIntegerRNGRange->MapValueIntoRange(unRetVal, gsl_rng_get(m_ptRNG), c_range);
      return unRetVal;
#else
      UInt32 unNumber;
      random_r(m_ptRNG, reinterpret_cast<int32_t*>(&unNumber));
      UInt32 unRetVal;
      m_pcIntegerRNGRange->MapValueIntoRange(unRetVal, unNumber, c_range);
      return unRetVal;
#endif
   }
   
   /****************************************/
   /****************************************/

   Real CRandom::CRNG::Exponential(Real f_mean) {
#ifndef CROSSCOMPILING
      return gsl_ran_exponential(m_ptRNG, f_mean);
#else      
      CRange<Real> fRange(0.0f, 1.0f);
      return -log(Uniform(fRange)) / f_mean;
#endif
   }
   
   /****************************************/
   /****************************************/

   Real CRandom::CRNG::Gaussian(Real f_std_dev,
                                Real f_mean) {
#ifndef CROSSCOMPILING
      return f_mean + gsl_ran_gaussian(m_ptRNG, f_std_dev);
#else
      /* This is the Box-Muller method in its cartesian variant
         see http://www.dspguru.com/dsp/howtos/how-to-generate-white-gaussian-noise
      */
      CRange<Real> fRange(-1.0f, 1.0f);
      Real fNum1, fNum2;
      Real fSquare;
      do {
         fNum1 = Uniform(fRange);
         fNum2 = Uniform(fRange);
         fSquare = fNum1 * fNum1 + fNum2 * fNum2;
      } while(fSquare >= 1);
      return f_mean + f_std_dev * fNum1;
#endif
   }

   /****************************************/
   /****************************************/

   Real CRandom::CRNG::Rayleigh(Real f_sigma) {
#ifndef CROSSCOMPILING
      return gsl_ran_rayleigh(m_ptRNG, f_sigma);
#else
      /* Draw a number uniformly from (0,1) --- bounds excluded */
      Real fValue;
      CRange<Real> cUnitRange(0.0f, 1.0f);
      do {
         fValue = Uniform(cUnitRange);
      }
      while(! cUnitRange.WithinMinBoundExcludedMaxBoundExcluded(fValue));
      /* Calculate the value to return from the definition of Rayleigh distribution
       * http://en.wikipedia.org/wiki/Rayleigh_distribution#Generating_Rayleigh-distributed_random_variates
       */
      return f_sigma * Sqrt(-2.0f * Log(fValue));
#endif
   }

/****************************************/
   /****************************************/

   Real CRandom::CRNG::Lognormal(Real f_sigma, Real f_mu) {
#ifndef CROSSCOMPILING
      return gsl_ran_lognormal(m_ptRNG, f_mu, f_sigma);
#else
      /* Draw a number uniformly from (0,1) */
      Real fValue;
      fValue = Gaussian(1,0);
      /* Calculate the value to return from the definition of Lognormal distribution
       * http://en.wikipedia.org/wiki/Log-normal_distribution#Generating_log-normally_distributed_random_variates
       */
      return std::exp(f_mu + f_sigma * fValue);
#endif
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

   CRandom::CCategory::CCategory(CByteArray& c_buffer) :
      m_unSeed(0),
      m_cSeeder(0),
      m_cSeedRange(1, std::numeric_limits<UInt32>::max()) {
      LoadState(c_buffer);
   }

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

   void CRandom::CCategory::SaveState(CByteArray& c_buffer) {
      /* Dump id */
      c_buffer << m_strId;
      /* Dump seed */
      c_buffer << m_unSeed;
      /* Dump number of RNG in this category */
      c_buffer << m_vecRNGList.size();
      /* Dump the state of each RNG */
      for(size_t i = 0; i < m_vecRNGList.size(); ++i) {
         m_vecRNGList[i]->SaveState(c_buffer);
      }
      /* Dump seeder */
      m_cSeeder.SaveState(c_buffer);
   }

   /****************************************/
   /****************************************/

   void CRandom::CCategory::LoadState(CByteArray& c_buffer) {
      /* Empty the RNG list */
      while(!m_vecRNGList.empty()) {
         delete m_vecRNGList.back();
         m_vecRNGList.pop_back();
      }
      /* Get id */
      c_buffer >> m_strId;
      /* Get seed */
      c_buffer >> m_unSeed;
      /* Get number of RNG in this category */
      size_t unRNGNum;
      c_buffer >> unRNGNum;
      /* Create new generators */
      for(size_t i = 0; i < unRNGNum; ++i) {
         m_vecRNGList.push_back(new CRNG(c_buffer));
      }
      /* Get seeder */
      m_cSeeder.LoadState(c_buffer);
   }

   /****************************************/
   /****************************************/

   CRandom::CRNG* CRandom::CCategory::CreateRNG(const std::string& str_type) {
      /* Get seed from internal RNG */
      UInt32 unSeed = m_cSeeder.Uniform(m_cSeedRange);
      /* Create new RNG */
      m_vecRNGList.push_back(new CRNG(unSeed, str_type));
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

   CRandom::CRNG* CRandom::CreateRNG(const std::string& str_category,
                                     const std::string& str_type) {
      CHECK_CATEGORY(str_category);
      return itCategory->second->CreateRNG(str_type);
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

   void CRandom::SaveState(CByteArray& c_buffer) {
      /* Dump the number of categories */
      c_buffer << m_mapCategories.size();
      /* Dump the categories */
      for(std::map<std::string, CCategory*>::iterator itCategory = m_mapCategories.begin();
          itCategory != m_mapCategories.end();
          ++itCategory) {
         itCategory->second->SaveState(c_buffer);
      }
   }

   /****************************************/
   /****************************************/

   void CRandom::LoadState(CByteArray& c_buffer) {
      /* Get the number of categories */
      size_t unCategories;
      c_buffer >> unCategories;
      /* Restore the categories */
      for(UInt32 i = 0; i < unCategories; ++i) {
         CCategory* pcCat = new CCategory(c_buffer);
         m_mapCategories.insert(
            std::pair<std::string, CRandom::CCategory*>(pcCat->GetId(), pcCat));
      }
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
