/**
 * @file <argos3/plugins/simulator/actuators/quadrotor_speed_default_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QUADROTOR_SPEED_ACTUATOR_DEFAULT_H
#define QUADROTOR_SPEED_ACTUATOR_DEFAULT_H

#include <string>
#include <map>

namespace argos {
   class CQuadRotorSpeedDefaultActuator;
}

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/actuator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/robots/generic/control_interface/ci_quadrotor_speed_actuator.h>
#include <argos3/plugins/simulator/entities/quadrotor_entity.h>

namespace argos {

   class CQuadRotorSpeedDefaultActuator : public CSimulatedActuator,
                                          public CCI_QuadRotorSpeedActuator {

   public:

      CQuadRotorSpeedDefaultActuator();
      virtual ~CQuadRotorSpeedDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void SetLinearVelocity(const CVector3& c_velocity);
      virtual void SetRotationalSpeed(const CRadians& c_speed);

      virtual void Update();
      virtual void Reset();

   protected:

      CQuadRotorEntity* m_pcQuadRotorEntity;
      CEmbodiedEntity* m_pcEmbodiedEntity;
      CQuadRotorEntity::SSpeedControlData m_sDesiredSpeedData;
      
   };

}

#endif
