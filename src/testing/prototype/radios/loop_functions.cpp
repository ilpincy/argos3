#include "loop_functions.h"
#include <argos3/plugins/robots/prototype/simulator/prototype_entity.h>
#include <argos3/plugins/robots/generic/control_interface/ci_simple_radios_sensor.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   bool CTestLoopFunctions::IsExperimentFinished() {
      /* wait five ticks before evaluating the test */
      if(GetSpace().GetSimulationClock() < 5) {
         return false;
      }
      else {
         CPrototypeEntity* pcNode0 = nullptr;
         CPrototypeEntity* pcNode1 = nullptr;
         try {
            pcNode0 = dynamic_cast<CPrototypeEntity*>(&GetSpace().GetEntity("node0"));
            pcNode1 = dynamic_cast<CPrototypeEntity*>(&GetSpace().GetEntity("node1"));
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Node was not added to the simulator", ex);
         }
         if(pcNode0 == nullptr) {
            THROW_ARGOSEXCEPTION("Node0 was not a prototype");
         }
         if(pcNode1 == nullptr) {
            THROW_ARGOSEXCEPTION("Node1 was not a prototype");
         }
         CCI_SimpleRadiosSensor* pcSimpleRadioSensorNode0 =
            pcNode0->GetControllableEntity().GetController().GetSensor<CCI_SimpleRadiosSensor>("simple_radios");
         CCI_SimpleRadiosSensor* pcSimpleRadioSensorNode1 =
            pcNode1->GetControllableEntity().GetController().GetSensor<CCI_SimpleRadiosSensor>("simple_radios");
         if(pcSimpleRadioSensorNode0->GetInterfaces()[0].Messages.size() != 0) {
            THROW_ARGOSEXCEPTION("node0 should have zero messages");
         }
         if(pcSimpleRadioSensorNode1->GetInterfaces()[0].Messages.size() != 1) {
            THROW_ARGOSEXCEPTION("node1 should have one message");
         }
         return true;
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_LOOP_FUNCTIONS(CTestLoopFunctions, "test_loop_functions");

}
