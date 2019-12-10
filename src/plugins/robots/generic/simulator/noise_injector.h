/**
 * @file <argos3/plugins/simulator/robots/generic/simulator/noise_injector.h>
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#ifndef NOISE_INJECTOR_H
#define NOISE_INJECTOR_H

#include <string>

#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

/**
 * @brief Utility class to standardize noise injection in robot sensors and
 * actuators. Has the following noise models:
 *
 * - uniform - A Uniform(a,b) distribution
 * - gaussian - A Guassian(mean, stddev) distribution
 */

class CNoiseInjector {

   public:

  struct SDocumentationQuerySpec {
    std::string strDocName;
    std::string strXMLParent;
    std::string strXMLTag;
    std::string strSAAType;
    bool bShowExamples;
  };
     CNoiseInjector();

     ~CNoiseInjector() {}

     /**
      * @brief Get the documentation for this class should appear whenever a
      * query involving a noise-injectable entity is requested.
      */
     static std::string GetQueryDocumentation(const SDocumentationQuerySpec& c_spec);

     /**
      * @brief Initialize noise injection by reading XML configuration.
      */
     void Init(TConfigurationNode& t_tree);

     /**
      * @brief Returns \c TRUE if noise injection has been configured, and \c
      * FALSE otherwise.
      */
     inline bool Enabled(void) { return m_bEnabled; }

     /**
      * @brief Inject noise according to configuration parameters.
      *
      * If noise injection has not been configured, returns 0.0.
      */
     Real InjectNoise();

     /**
      * @brief Configure Uniform(a,b) noise injection from the specified range.
      */
     void InitUniform(const CRange<Real>& c_range);

     /**
      * @brief Configure Gaussian(mean, stddev) noise injection from the
      * specified parameters.
      */
     void InitGaussian(Real f_mean, Real f_std_dev);

     /**
      * @brief Compute a Bernoulli event based on the configured noise model.
      *
      */
     bool BernoulliEvent(void);

   private:

     enum model {
       ekNONE,
       ekGAUSSIAN,
       ekUNIFORM
     };
     /**
      * @brief Configure Uniform(a,b) noise injection by reading XML
      * configuration.
      */
     void InitUniform(TConfigurationNode& t_tree);

     /**
      * @brief Configure Gaussian(mean, stddev) noise injection by reading XML
      * configuration.
      */
     void InitGaussian(TConfigurationNode& t_tree);

     /** Random number generator */
     CRandom::CRNG* m_pcRNG;

     /** Whether to add noise or not */
     bool m_bEnabled;

     /** The noise model: uniform or gaussian */
     model m_model;

     /** Uniform noise range */
     CRange<Real> m_cUniformRange;

     /** Gaussian noise mean */
     Real m_fGaussianMean;

     /** Gaussian noise standard deviation */
     Real m_fGaussianStdDev;
   };
}

#endif
