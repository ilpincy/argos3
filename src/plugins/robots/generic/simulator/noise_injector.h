/**
 * @file <argos3/plugins/simulator/robots/generic/simulator/noise_injector.h>
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#ifndef NOISE_INJECTOR_H
#define NOISE_INJECTOR_H

#include <string>
#include <memory>

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/configuration/argos_configuration.h>

namespace argos {

/**
 * @brief Base class for noise injection into robot sensors and
 * actuators, mainly specifying an interface for derived injectors.
 */
class CNoiseInjector {

   public:

     /**
      * @brief Specification for the documentation on how robot sensor/actuators
      * capable of receiving noise injection should be displayed when it is queried
      * by the user on the command line.
      */
     struct SDocumentationQuerySpec {
       /**
        * @brief The spoken word name of the robot facility capable of
        * receiving noise injection.
        */
       std::string strDocName;
       /**
        * @brief The XML parent tag of the robot facility capable of
        * receiving noise injection.
        */
       std::string strXMLParent;

       /**
        * @brief The XML tag of the robot facility capable of receiving noise
        * injection.
        */
       std::string strXMLTag;

       /**
        * @brief The type of the The XML parent tag of the robot facility
        * capable of receiving noise injection: [sensor, actuator].
        */
       std::string strSAAType;

       /**
        * @brief Should example XML configurations be shown for the robot
        * facility?
        */
       bool bShowExamples;
     };

     CNoiseInjector();

     virtual ~CNoiseInjector() {}

     /**
      * @brief Get the documentation for this class should appear whenever a
      * query involving a noise-injectable entity is requested.
      */
     static std::string GetQueryDocumentation(const SDocumentationQuerySpec& c_spec);

     /**
      * @brief Inject noise according to configuration parameters.
      *
      * If noise injection has not been configured it should return 0.0.
      */
     virtual Real InjectNoise() = 0;

     virtual void Init(TConfigurationNode& t_tree) = 0;

     /**
      * @brief Returns \c TRUE if noise injection has been configured, and \c
      * FALSE otherwise.
      */
     inline bool Enabled(void) { return m_bEnabled; }

     /**
      * @brief Compute a Bernoulli event based on the configured noise model.
      */
     bool BernoulliEvent(void);

   protected:

     inline void SetEnable(bool b_state) { m_bEnabled = b_state; }

     inline CRandom::CRNG* GetRNG() {
       return m_pcRNG;
     }

   private:

     /** Random number generator */
     CRandom::CRNG* m_pcRNG;

     /** Whether to add noise or not */
     bool m_bEnabled;
   };
}

#endif
