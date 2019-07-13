/**
 * @file <argos3/core/utility/math/rng.h>
 *
 * @brief This file provides the definition of the random number generator of ARGoS.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef ARGOS_RANDOM
#define ARGOS_RANDOM

namespace argos {
   class CRandom;
}

#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/math/range.h>
#include <map>

namespace argos {

/**
 * The ARGoS random number generator.
 * <p>
 * It is meant to be used not only inside ARGoS, but also from controllers, loop functions
 * as well as external applications.
 * </p>
 * <p>
 * In general, RNGs have the greatest impact on the reproducibility of experiments.
 * However, once the random seed has been set, the series of random numbers generated
 * by an RNG is fixed. In this way, experiments are reproducible.
 * To further complicate things, ARGoS is a multi-threaded program. Multi-threading and
 * RNGs create issues to reproducibility. In fact, if the RNGs were shared among the
 * threads, it would be impossible to predict the order in which the RNGs are accessed,
 * because thread scheduling is not controllable nor predictable.
 * For this reason, a common solution is to assign a separated RNG to each thread. For
 * ARGoS, this is not a viable method because we want the number of threads to be
 * set by the user and we want the outcome of an experiment to be reproducible no matter
 * how many threads are used.
 * </p>
 * <p>
 * All these considerations brought us to the design of a RNG with a different approach.
 * The class argos::CRandom is a static factory that creates argos::CRandom::CRNG
 * objects. If a component (sensor, actuator, controller, etc.) needs to get random
 * numbers, it has to first create a local RNG class (typically, at init time). Internally,
 * the factory assigns a seed to it.
 * </p>
 * <p>
 * To allow for usage from different components (such as ARGoS main code, an evolutionary
 * algorithm, controllers, etc.) RNGs are divided up in categories. The ARGoS core uses
 * the <tt>argos</tt> category. When ARGoS is reset, all the RNGs in this category are reset
 * as well, but not the RNGs in other categories. Categories give the user complete power on the
 * RNGs, while ensuring compartmentalization.
 * </p>
 * <p>
 * If all you want is drawing random numbers inside a controller, you can safely create new
 * argos::CRandom::CRNG objects in the <tt>argos</tt> category. If, instead, you intend to
 * wrap ARGoS around an optimization algorithm and want to maintain control of the way random
 * number generators are managed, it is necessary for you to create your own category.
 * </p>
 * <p>
 * To create a new RNG inside the <tt>argos</tt> category, this is enough:
 * </p>
 * <pre>
 * argos::CRandom::CRNG* m_pcRNG = argos::CRandom::CreateRNG("argos");
 * </pre>
 * <p>
 * When you want to use random numbers with a custom category, you need to
 * first call the function providing a label for the category and a base seed:
 * </p>
 * <pre>
 * argos::CRandom::CreateCategory("my_category", my_seed);
 * </pre>
 * <p>
 * Once the category is created, you can get a new RNG with a call to the function
 * </p>
 * <pre>
 * argos::CRandom::CRNG* m_pcRNG = argos::CRandom::CreateRNG("my_category");
 * </pre>
*/
   class CRandom {

   public:

      /**
       * The RNG.
       * This class is the real random number generator. You need an instance of this class
       * to be able to generate random numbers.
       */
      class CRNG {

      public:

         /**
          * Class constructor.
          * To create a new RNG from user code, never use this method. Use CreateRNG() instead.
          * @param un_seed the seed of the RNG.
          * @param str_type the type of RNG to use. By default, Mersenne Twister is used. For a list of available RNG types, see GetRNGTypes().
          */
         CRNG(UInt32 un_seed);

         /**
          * Class copy constructor.
          * To create a new RNG from user code, never use this method. Use CreateRNG() instead.
          */
         CRNG(const CRNG& c_rng);

         /**
          * Class destructor.
          */
         virtual ~CRNG();

         /**
          * Returns the seed of this RNG.
          * @return the seed of this RNG.
          */
         inline UInt32 GetSeed() const throw() {
            return m_unSeed;
         }

         /**
          * Sets the seed of this RNG.
          * This method does not reset the RNG. You must call Reset() explicitly.
          * @param un_seed the new seed for this RNG.
          * @see Reset()
          */
         inline void SetSeed(UInt32 un_seed) throw() {
            m_unSeed = un_seed;
         }

         /**
          * Reset the RNG.
          * Reset the RNG to the current seed value.
          */
         void Reset();

         /**
          * Returns a random value from a Bernoulli distribution.
          * @param f_true the probability to return a 1.
          * @returns a random value from a Bernoulli distribution (<tt>true</tt>/<tt>false</tt>).
          */
         bool Bernoulli(Real f_true = 0.5);
         
         /**
          * Returns a random value from a uniform distribution.
          * @param c_range the range of values to draw one from.
          * @return a random value from the range [min,max).
          */
         CRadians Uniform(const CRange<CRadians>& c_range);
         
         /**
          * Returns a random value from a uniform distribution.
          * @param c_range the range of values to draw one from.
          * @return a random value from the range [min,max).
          */
         Real Uniform(const CRange<Real>& c_range);
         
         /**
          * Returns a random value from a uniform distribution.
          * @param c_range the range of values to draw one from.
          * @return a random value from the range [min,max).
          */
         SInt32 Uniform(const CRange<SInt32>& c_range);
         
         /**
          * Returns a random value from a uniform distribution.
          * @param c_range the range of values to draw one from.
          * @return a random value from the range [min,max).
          */
         UInt32 Uniform(const CRange<UInt32>& c_range);
         
         /**
          * Returns a random value from an exponential distribution.
          * @param f_mean the mean of the exponential distribution.
          * @return a random value from an exponential distribution.
          */
         Real Exponential(Real f_mean);

         /**
          * Returns a random value from a Poisson distribution.
          * @param f_mean the mean of the Poisson distribution.
          * @return a random value from the Poisson distribution.
          */
         UInt32 Poisson(Real f_mean);
         
         /**
          * Returns a random value from a Gaussian distribution.
          * @param f_std_dev the standard deviation of the Gaussian distribution.
          * @param f_mean the mean of the Gaussian distribution.
          * @return a random value from the Gaussian distribution.
          */
         Real Gaussian(Real f_std_dev, Real f_mean = 0.0f);
         
         /**
          * Returns a random value from a Rayleigh distribution.
          * @param f_sigma parameter sigma of the distribution
          * @return a random value from the Rayleigh distribution.
          */
         Real Rayleigh(Real f_sigma);
         
         /**
          * Returns a random value from a Lognormal distribution.
          * @param f_sigma parameter sigma of the distribution
          * @param f_mu parameter mu of the distribution
          * @return a random value from the Lognormal distribution.
          */
         Real Lognormal(Real f_sigma, Real f_mu);

         /**
          * Shuffles the values of the given vector in-place.
          * @param vec_data The vector whose values must be shuffled.
          */
         template <typename T>
         void Shuffle(std::vector<T>& vec_data) {
            UInt32 j;
            for(UInt32 i = 0; i < vec_data.size()-1; ++i) {
               j = i + Uniform(CRange<UInt32>(0, vec_data.size() - i));
               T tTmp = vec_data[i];
               vec_data[i] = vec_data[j];
               vec_data[j] = tTmp;
            }
         }

      private:

         /*
          * Generates a random 32bit unsigned integer.
          * Used internally by all other functions.
          */
         UInt32 Uniform32bit();

      private:

         UInt32 m_unSeed;
         UInt32* m_punState;
         SInt32 m_nIndex;

      };

      /**
       * The RNG category.
       * This class stores a specific category of RNGs.
       */
      class CCategory {

      public:

         /**
          * Class constructor.
          * @param str_id the id of the category.
          * @param un_seed the seed of the category.
          */
         CCategory(const std::string& str_id,
                   UInt32 un_seed);

         /**
          * Class destructor.
          */
         virtual ~CCategory();

         /**
          * Returns the id of the category.
          * @return the id of the category.
          */
         inline const std::string& GetId() const throw() {
            return m_strId;
         }
         /**
          * Sets the new id of the category.
          * @param str_id the new id of the category.
          */
         void SetId(const std::string& str_id) {
            m_strId = str_id;
         }

         /**
          * Returns the seed of the category.
          * @return the seed of the category.
          */
         inline UInt32 GetSeed() const {
            return m_unSeed;
         }
         /**
          * Sets the new seed of the category.
          * This method does not reset the RNGs. You must call Reset() explicitly.
          * @param un_seed the new seed of the category.
          * @see Reset()
          */
         void SetSeed(UInt32 un_seed);

         /**
          * Creates a new RNG inside this category.
          * @return the pointer to a new RNG inside this category.
          */
         CRNG* CreateRNG();

         /**
          * Resets the RNGs in this category.
          */
         void ResetRNGs();

         /**
          * Sets new seed for the RNGs in this category.
          * This method does not reset the RNGs. You must call Reset() explicitly.
          * @see Reset()
          */
         void ReseedRNGs();

      private:

         std::string m_strId;
         std::vector<CRNG*> m_vecRNGList;
         UInt32 m_unSeed;
         CRNG m_cSeeder;
         CRange<UInt32> m_cSeedRange;
      };

   public:

      /**
       * Creates a new category.
       * @param str_category the id of the category.
       * @param un_seed the base seed of the category.
       * @return <tt>true</tt> if the category was created; <tt>false</tt> if a category with the passed id exists already.
       */
      static bool CreateCategory(const std::string& str_category,
                                 UInt32 un_seed);
      /**
       * Returns a reference to the wanted category.
       * @param str_category the id of the category.
       * @return a reference to the wanted category.
       */
      static CCategory& GetCategory(const std::string& str_category);

      /**
       * Returns <tt>true</tt> if the given category exists in the pool.
       * @param str_category the id of the category.       
       * @return <tt>true</tt> if the given category exists in the pool.
       */
      static bool ExistsCategory(const std::string& str_category);

      /**
       * Removes the wanted category.
       * @param str_category the id of the category.
       */
      static void RemoveCategory(const std::string& str_category);

      /**
       * Creates a new RNG inside the given category.
       * @param str_category the id of the category.
       * @return the pointer to a new RNG inside this category.
       */
      static CRNG* CreateRNG(const std::string& str_category);

      /**
       * Returns the seed of the wanted category.
       * @param str_category the id of the category.
       * @return the seed of the wanted category.
       */
      static UInt32 GetSeedOf(const std::string& str_category);

      /**
       * Sets the new seed of the wanted category.
       * This method does not reset the RNGs. You must call Reset() explicitly.
       * @param str_category the id of the category.
       * @param un_seed the new seed of the wanted category.
       * @see Reset()
       */
      static void SetSeedOf(const std::string& str_category,
                            UInt32 un_seed);

      /**
       * Resets all the RNG categories.
       */
      static void Reset();

   private:

      static std::map<std::string, CCategory*> m_mapCategories;
   };

}

#endif
