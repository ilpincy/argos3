/**
 *
 * @author Michael Allwright <mallwright@learnrobotics.io>
 */

#include "controller.h"

#include <argos3/plugins/robots/generic/control_interface/ci_simple_radios_actuator.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CTestController::Init(TConfigurationNode& t_tree) {
      m_pcSimpleRadioActuator = GetActuator<CCI_SimpleRadiosActuator>("simple_radios");
   }

   /****************************************/
   /****************************************/

   void CTestController::ControlStep() {
      CByteArray cMessage;
      cMessage << static_cast<UInt8>(0);
      m_pcSimpleRadioActuator->GetInterfaces()[0].Messages.emplace_back(cMessage);
   }

   /****************************************/
   /****************************************/

   REGISTER_CONTROLLER(CTestController, "test_controller");

}



