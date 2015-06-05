/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_gripper_default_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "footbot_gripper_default_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CFootBotGripperDefaultActuator::CFootBotGripperDefaultActuator() :
      m_pcGripperEquippedEntity(NULL) {}

   /****************************************/
   /****************************************/

   void CFootBotGripperDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcGripperEquippedEntity = &(c_entity.GetComponent<CGripperEquippedEntity>("gripper"));
      m_pcGripperEquippedEntity->Enable();
   }

   /****************************************/
   /****************************************/

   void CFootBotGripperDefaultActuator::Update() {
      m_pcGripperEquippedEntity->SetLockState(Abs(m_cAperture) / CRadians::PI_OVER_TWO);
   }

   /****************************************/
   /****************************************/

   void CFootBotGripperDefaultActuator::Reset() {
      Unlock();
   }

   /****************************************/
   /****************************************/

}

REGISTER_ACTUATOR(CFootBotGripperDefaultActuator,
                  "footbot_gripper", "default",
                  "Carlo Pinciroli [ilpincy@gmail.com]",
                  "1.0",
                  "The footbot gripper actuator.",
                  "This actuator controls the foot-bot gripper. For a complete description of its\n"
                  "usage, refer to the ci_footbot_gripper_actuator.h file.\n\n"
                  "REQUIRED XML CONFIGURATION\n\n"
                  "  <controllers>\n"
                  "    ...\n"
                  "    <my_controller ...>\n"
                  "      ...\n"
                  "      <actuators>\n"
                  "        ...\n"
                  "        <footbot_gripper implementation=\"default\" />\n"
                  "        ...\n"
                  "      </actuators>\n"
                  "      ...\n"
                  "    </my_controller>\n"
                  "    ...\n"
                  "  </controllers>\n\n"
                  "OPTIONAL XML CONFIGURATION\n\n"
                  "None.\n",
                  "Usable"
   );

