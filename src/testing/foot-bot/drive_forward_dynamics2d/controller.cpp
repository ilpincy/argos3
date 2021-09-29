/**
 *
 * @author Michael Allwright <mallwright@learnrobotics.io>
 */

#include "controller.h"

#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CTestController::Init(TConfigurationNode& t_tree) {
      CCI_DifferentialSteeringActuator* pcActuator =
         GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
      pcActuator->SetLinearVelocity(10.0, 10.0); // 10 cm per second forwards
   }

   /****************************************/
   /****************************************/

   REGISTER_CONTROLLER(CTestController, "test_controller");

}



