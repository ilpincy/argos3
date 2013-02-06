/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_wheels_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_footbot_wheels_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/
   
   // period of the PID speed controller, in seconds
   Real CCI_FootBotWheelsActuator::T_PID = 0.005;
   // conversion factors got from experimental data
   Real CCI_FootBotWheelsActuator::CM_TO_PULSE = 24156.0 / 50.0; // [pulse / cm]
   Real CCI_FootBotWheelsActuator::PULSE_TO_CM = 1.0 / CM_TO_PULSE; // [cm / pulse]
   // speed conversion factors
   Real CCI_FootBotWheelsActuator::PULSE_TPID_TO_CM_SEC = PULSE_TO_CM / T_PID;
   Real CCI_FootBotWheelsActuator::CM_SEC_TO_PULSE_TPID = 1.0
      / PULSE_TPID_TO_CM_SEC;

   /****************************************/
   /****************************************/

}
