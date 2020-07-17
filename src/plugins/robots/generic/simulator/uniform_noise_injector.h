/**
 * @file <argos3/plugins/simulator/robots/generic/simulator/uniform_noise_injector.h>
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#ifndef UNIFORM_NOISE_INJECTOR_H
#define UNIFORM_NOISE_INJECTOR_H


#include <argos3/core/utility/math/range.h>
#include <argos3/plugins/robots/generic/simulator/noise_injector.h>

namespace argos {

/**
 * @brief Utility class for injecting Uniform(a,b) noise into robot sensors and
 * actuators.
 */

class CUniformNoiseInjector : public CNoiseInjector {

   public:

     CUniformNoiseInjector() {}

     ~CUniformNoiseInjector() override {}

     /**
      * @brief Initialize noise injection by reading XML configuration.
      */
     void Init(TConfigurationNode& t_tree) override final;


     /**
      * @brief Inject noise according to configuration parameters.
      *
      * If noise injection has not been configured, returns 0.0.
      */
     Real InjectNoise() override final;

     /**
      * @brief Configure Uniform(a,b) noise injection from the specified range.
      */
     void InitFromRange(const CRange<Real>& c_range);

   private:

     /** Uniform noise range */
     CRange<Real> m_cUniformRange;
   };
}

#endif
