/**
 * @file <argos3/plugins/simulator/robots/generic/simulator/gaussian_noise_injector.h>
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#ifndef GAUSSIAN_NOISE_INJECTOR_H
#define GAUSSIAN_NOISE_INJECTOR_H

#include <string>

#include <argos3/plugins/robots/generic/simulator/noise_injector.h>

namespace argos {

/**
 * @brief Utility class for injecting Guassian(mean, stddev) noise into robot
 * sensors and actuators.
 */
class CGaussianNoiseInjector : public CNoiseInjector {

   public:

     CGaussianNoiseInjector();

     ~CGaussianNoiseInjector() override {}

     /**
      * @brief Initialize noise injection by reading XML configuration.
      */
     void Init(TConfigurationNode& t_tree) override final;

     Real InjectNoise() override final;

     /**
      * @brief Configure Gaussian(mean, stddev) noise injection from the
      * specified parameters.
      */
     void InitFromValues(Real f_mean, Real f_std_dev);

   private:

     /** Gaussian noise mean */
     Real m_fGaussianMean;

     /** Gaussian noise standard deviation */
     Real m_fGaussianStdDev;
   };
}

#endif
