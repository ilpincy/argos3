/**
 * @file <argos3/plugins/robots/generic/gaussian_noise_injector.cpp>
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#include <argos3/plugins/robots/generic/simulator/gaussian_noise_injector.h>

namespace argos {

   /****************************************/
   /****************************************/

   CGaussianNoiseInjector::CGaussianNoiseInjector() :
    m_fGaussianMean(0.0),
    m_fGaussianStdDev(1.0) {}

   /****************************************/
   /****************************************/

   void CGaussianNoiseInjector::Init(TConfigurationNode& t_tree) {
     /* Parse stddev */
     GetNodeAttributeOrDefault(t_tree,
                               "stddev",
                               m_fGaussianStdDev,
                               m_fGaussianStdDev);
     if (m_fGaussianStdDev < 0.0f) {
       THROW_ARGOSEXCEPTION("Gaussian standard deviation cannot be negative");
     }
     /* parse mean */
     GetNodeAttributeOrDefault(t_tree,
                               "mean",
                               m_fGaussianMean,
                               m_fGaussianMean);
     SetEnable(true);
   }

   /****************************************/
   /****************************************/

   void CGaussianNoiseInjector::InitFromValues(Real f_mean, Real f_std_dev) {
     m_fGaussianMean = f_mean;
     m_fGaussianStdDev = f_std_dev;
     SetEnable(true);
   }

   /****************************************/
   /****************************************/

   Real CGaussianNoiseInjector::InjectNoise() {
     if (Enabled()) {
       return GetRNG()->Gaussian(m_fGaussianStdDev, m_fGaussianMean);
     }
     return 0.0;
   }

} // namespace argos
