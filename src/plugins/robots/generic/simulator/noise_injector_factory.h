/**
 * @file <argos3/plugins/simulator/robots/generic/simulator/noise_injector_factory.h>
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#ifndef NOISE_INJECTOR_FACTORY_H
#define NOISE_INJECTOR_FACTORY_H

#include <memory>
#include <string>

#include <argos3/core/utility/configuration/argos_configuration.h>

namespace argos {

class CNoiseInjector;

/**
 * @brief Factory class to create different types of sensor and actuator noise
 * injection handlers from XML configuration.
 *
 * Supports:
 * - uniform - A Uniform(a,b) distribution: @ref CUniformNoiseInjector.
 * - gaussian - A Guassian(mean, stddev) distribution: @ref CGaussianNoiseInjector.
 * - none - A "null" distribution.
 */

class CNoiseInjectorFactory {

   public:

     CNoiseInjectorFactory() {}

     ~CNoiseInjectorFactory() {}

     /**
      * @brief Create an injector from the specified configuration node.
      */
     static std::unique_ptr<CNoiseInjector> Create(TConfigurationNode& t_tree);
};

}

#endif
