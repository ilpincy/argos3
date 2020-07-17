/**
 * @file <argos3/plugins/robots/generic/uniform_noise_injector.cpp>
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#include <argos3/plugins/robots/generic/simulator/uniform_noise_injector.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CUniformNoiseInjector::Init(TConfigurationNode& t_tree) {
     /* Parse noise level */
     Real fNoiseLevel = 0.0f;
     GetNodeAttribute(t_tree, "level", fNoiseLevel);
     if (fNoiseLevel < 0.0f) {
       THROW_ARGOSEXCEPTION("Uniform noise level must be specified as non-negative value");
     } else if (fNoiseLevel > 0.0f) {
       m_cUniformRange.Set(-fNoiseLevel, fNoiseLevel);
       SetEnable(true);
     }
   }

   /****************************************/
   /****************************************/

   void CUniformNoiseInjector::InitFromRange(const CRange<Real>& c_range) {
     m_cUniformRange = c_range;
     SetEnable(true);
   }

   /****************************************/
   /****************************************/

   Real CUniformNoiseInjector::InjectNoise() {
     if (Enabled()) {
       return GetRNG()->Uniform(m_cUniformRange);
     }
     return 0.0;
   }

} // namespace argos
