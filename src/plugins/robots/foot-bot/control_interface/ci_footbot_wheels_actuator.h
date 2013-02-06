/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_wheels_actuator.h>
 *
 * @brief This file provides the definition of the footbot wheels actuator.
 *
 * This file provides the definition of the footbot wheels actuator.
 * This actuator sets the speed of the wheels. On the real robot, the speed is
 * set by a PID controller, which has a specific period here referred to as
 * T_PID. The actual value accepted by the robot as speed is expressed in pulse
 * per pid period.
 * In the common interface, the user can specify the linear velocity of the robot,
 * that is the distance covered by the robot (in cm) per second. This value is
 * related to the pulse input to the motors. Therefore, here we provide conversion
 * factors that are also used in other related sensors, such as the footbot wheel
 * speed sensor and the footbot encoder sensor.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CCI_FOOTBOT_WHEELS_ACTUATOR_H
#define CCI_FOOTBOT_WHEELS_ACTUATOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_FootBotWheelsActuator;
}

#include <argos3/core/control_interface/ci_differential_steering_actuator.h>
#include <argos3/core/utility/math/range.h>

namespace argos {

   class CCI_FootBotWheelsActuator : virtual public CCI_DifferentialSteeringActuator {

   public:

      virtual ~CCI_FootBotWheelsActuator() {
      }

   protected:

      /** period of the PID speed controller, in ms */
      static Real T_PID;

      /** conversion factors got from experimental data */
      static Real CM_TO_PULSE;
      static Real PULSE_TO_CM;

      /** speed conversion factors */
      static Real PULSE_TPID_TO_CM_SEC;
      static Real CM_SEC_TO_PULSE_TPID;

   };

}

#endif
