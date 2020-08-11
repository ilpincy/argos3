/**
 * @file <argos3/plugins/robots/mini-quadrotor/simulator/miniquadrotor_rotor_default_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef MINIQUADROTOR_ROTOR_ACTUATOR_DEFAULT_H
#define MINIQUADROTOR_ROTOR_ACTUATOR_DEFAULT_H

#include <string>
#include <map>
#include <memory>

namespace argos {
   class CMiniQuadrotorRotorDefaultActuator;
   class CRotorEquippedEntity;
}

#include <argos3/plugins/robots/mini-quadrotor/control_interface/ci_miniquadrotor_rotor_actuator.h>
#include <argos3/core/simulator/actuator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/robots/generic/simulator/noise_injector.h>

namespace argos {

   class CMiniQuadrotorRotorDefaultActuator : public CSimulatedActuator,
                                              public CCI_MiniQuadrotorRotorActuator {

   public:

      enum MINIQUADROTOR_ROTOR {
         NORTH_ROTOR = 0,
         WEST_ROTOR,
         SOUTH_ROTOR,
         EAST_ROTOR
      };

   public:

      CMiniQuadrotorRotorDefaultActuator();

      virtual ~CMiniQuadrotorRotorDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void SetRotorVelocities(const CCI_MiniQuadrotorRotorActuator::SVelocities& s_velocities);

      virtual void Update();

      virtual void Reset();

   protected:

      /** The rotor equipped entity */
      CRotorEquippedEntity* m_pcRotorEquippedEntity;

      /** Noise injector */
      std::unique_ptr<CNoiseInjector> m_pcNoiseInjector;
   };

}

#endif
