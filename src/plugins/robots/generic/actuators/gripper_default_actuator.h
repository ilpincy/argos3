/**
 * @file <argos3/plugins/simulator/actuators/gripper_default_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef GRIPPER_DEFAULT_ACTUATOR_H
#define GRIPPER_DEFAULT_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CGripperDefaultActuator;
}

#include <argos3/plugins/robots/generic/control_interface/ci_gripper_actuator.h>
#include <argos3/plugins/simulator/entities/gripper_equipped_entity.h>
#include <argos3/core/simulator/actuator.h>

namespace argos {

   class CGripperDefaultActuator : public CSimulatedActuator,
                                   public CCI_GripperActuator {

   public:

      CGripperDefaultActuator();

      virtual ~CGripperDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Update();
      virtual void Reset();

   private:

      CGripperEquippedEntity* m_pcGripperEquippedEntity;

   };

}

#endif
