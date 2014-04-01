/**
 * @file <argos3/plugins/simulator/actuators/quadrotor_position_default_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QUADROTOR_POSITION_ACTUATOR_DEFAULT_H
#define QUADROTOR_POSITION_ACTUATOR_DEFAULT_H

#include <string>
#include <map>

namespace argos {
   class CQuadRotorPositionDefaultActuator;
}

#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/actuator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/robots/generic/control_interface/ci_quadrotor_position_actuator.h>
#include <argos3/plugins/simulator/entities/quadrotor_entity.h>

namespace argos {

   class CQuadRotorPositionDefaultActuator : public CSimulatedActuator,
                                     public CCI_QuadRotorPositionActuator {

   public:

      CQuadRotorPositionDefaultActuator();
      virtual ~CQuadRotorPositionDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void SetAbsolutePosition(const CVector3& c_pos);
      virtual void SetRelativePosition(const CVector3& c_pos);
      virtual void SetAbsoluteYaw(const CRadians& c_yaw);
      virtual void SetRelativeYaw(const CRadians& c_yaw);

      virtual void Update();
      virtual void Reset();

   protected:

      CQuadRotorEntity* m_pcQuadRotorEntity;
      CEmbodiedEntity* m_pcEmbodiedEntity;
      CQuadRotorEntity::SPositionControlData m_sDesiredPosData;
      
   };

}

#endif
