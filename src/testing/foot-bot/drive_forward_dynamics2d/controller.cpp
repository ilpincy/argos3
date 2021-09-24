/**
 *
 * @author Michael Allwright <mallwright@learnrobotics.io>
 */

#include "controller.h"

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



