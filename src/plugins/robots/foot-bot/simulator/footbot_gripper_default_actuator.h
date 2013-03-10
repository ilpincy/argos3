/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_gripper_default_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FOOTBOT_GRIPPER_DEFAULT_ACTUATOR_H
#define FOOTBOT_GRIPPER_DEFAULT_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CFootBotGripperDefaultActuator;
}

#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_gripper_actuator.h>
#include <argos3/plugins/simulator/entities/gripper_equipped_entity.h>
#include <argos3/core/simulator/actuator.h>

namespace argos {

   class CFootBotGripperDefaultActuator : public CSimulatedActuator,
                                          public CCI_FootBotGripperActuator {

   public:

      CFootBotGripperDefaultActuator();

      virtual ~CFootBotGripperDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Update();
      virtual void Reset();

      virtual void EnableCheckForObjectGrippedRoutine() {}
      virtual void DisableCheckForObjectGrippedRoutine() {}

   private:

      CGripperEquippedEntity* m_pcGripperEquippedEntity;

   };

}

#endif
