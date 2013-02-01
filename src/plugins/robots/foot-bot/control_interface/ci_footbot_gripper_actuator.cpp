/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_gripper_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_footbot_gripper_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/
   
   CRadians CCI_FootBotGripperActuator::LOCKED_NEGATIVE(-ARGOS_PI * 0.5f);
   CRadians CCI_FootBotGripperActuator::LOCKED_POSITIVE(ARGOS_PI * 0.5f);
   CRadians CCI_FootBotGripperActuator::UNLOCKED(0.0f);
   
   /****************************************/
   /****************************************/

}
