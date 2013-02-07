/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_wheels_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FOOTBOT_WHEELS_ACTUATOR_H
#define FOOTBOT_WHEELS_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CFootBotWheelsActuator;
}

#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_wheels_actuator.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/simulator/entities/wheeled_entity.h>
#include <argos3/core/utility/math/rng.h>

namespace argos {

   class CFootBotWheelsActuator : public CSimulatedActuator<CFootBotEntity>,
                                  public CCI_FootBotWheelsActuator {

   public:

      enum FOOTBOT_WHEELS {
         FOOTBOT_LEFT_WHEEL = 0,
         FOOTBOT_RIGHT_WHEEL = 1
      };

   public:

      /**
       * @brief Constructor.
       */
      CFootBotWheelsActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CFootBotWheelsActuator() {}

      /**
       * @brief Sets entity that is equipped with these wheels.
       *
       * @param c_entity entity equipped with the wheels.
       */
      virtual void SetEntity(CEntity& c_entity);

      /**
       * @brief Initializes the wheels.
       *
       * @param t_tree XML configuration tree.
       */
      virtual void Init(TConfigurationNode& t_tree);

      /**
       * @brief Sets the linear velocity of the two wheels.
       * Velocities are expressed in cm per second, actuated values
       * are truncated is they exceed the maximum allowed.
       *
       * @param f_left_velocity desired left wheel velocity.
       * @param f_right_velocity desired right wheel velocity.
       */
      virtual void SetLinearVelocity(Real f_left_velocity,
                                     Real f_right_velocity);

      virtual void Update();

      virtual void Reset();

   private:

      CWheeledEntity* m_pcWheeledEntity;

      /** Random number generator */
      CRandom::CRNG* m_pcRNG;

      /** Noise parameters, at the moment noise is Gaussian */
      Real m_fNoiseStdDeviation;

      /** Adds noise to the wheels velocity */
      virtual void AddGaussianNoise();

   };

}

#endif
