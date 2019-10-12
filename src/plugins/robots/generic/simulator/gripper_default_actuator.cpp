/**
 * @file <argos3/plugins/simulator/actuators/gripper_default_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "gripper_default_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CGripperDefaultActuator::CGripperDefaultActuator() :
      m_pcGripperEquippedEntity(NULL) {}

   /****************************************/
   /****************************************/

   void CGripperDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcGripperEquippedEntity = &(c_entity.GetComponent<CGripperEquippedEntity>("gripper"));
      m_pcGripperEquippedEntity->Enable();
   }

   /****************************************/
   /****************************************/

   void CGripperDefaultActuator::Update() {
      m_pcGripperEquippedEntity->SetLockState(m_fLockState);
   }

   /****************************************/
   /****************************************/

   void CGripperDefaultActuator::Reset() {
      m_fLockState = 0.0;
   }

   /****************************************/
   /****************************************/

}

REGISTER_ACTUATOR(CGripperDefaultActuator,
                  "gripper", "default",
                  "Carlo Pinciroli [ilpincy@gmail.com]",
                  "1.0",
                  "The Gripper actuator.",

                  "This actuator controls a gripper. For a complete description of its\n"
                  "usage, refer to the ci_gripper_actuator.h file.\n\n"

                  "REQUIRED XML CONFIGURATION\n\n"
                  "  <controllers>\n"
                  "    ...\n"
                  "    <my_controller ...>\n"
                  "      ...\n"
                  "      <actuators>\n"
                  "        ...\n"
                  "        <gripper implementation=\"default\" />\n"
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

