#include "loop_functions.h"
#include <argos3/plugins/robots/pi-puck/simulator/pipuck_entity.h>
#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_front_camera_sensor.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   bool CTestLoopFunctions::IsExperimentFinished() {
      if(GetSpace().GetSimulationClock() == 0) {
         return false;
      }
      else {
         CPiPuckEntity* pcPiPuck = nullptr;
         try {
            CEntity& cEntity = GetSpace().GetEntity("pipuck");
            pcPiPuck = dynamic_cast<CPiPuckEntity*>(&cEntity);
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Robot was not added to the simulator", ex);
         }
         if(pcPiPuck == nullptr) {
            THROW_ARGOSEXCEPTION("Robot was not a Pi-Puck");
         }
         CCI_Controller& cController = pcPiPuck->GetControllableEntity().GetController();
         CCI_PiPuckFrontCameraSensor& cFrontCamera =
            *cController.GetSensor<CCI_PiPuckFrontCameraSensor>("pipuck_front_camera");
         if(cFrontCamera.GetTags().size() != 1) {
            THROW_ARGOSEXCEPTION("Incorrect number of tags detected");
         }
         /* try to detect an LED 2 cm left from the center of the tag. This is a bit of a hack since
            rotation is ignored, but it is sufficient for testing the simulator */
         const CVector3& cLedPosition = cFrontCamera.GetTags()[0].Position + LED_OFFSET;
         if(cFrontCamera.DetectLed(cLedPosition) != CCI_PiPuckFrontCameraSensor::ELedState::Q3) {
            THROW_ARGOSEXCEPTION("Incorrect LED color detected");
         }
         return true;
      }
      return false;
   }

   /****************************************/
   /****************************************/

   const CVector3 CTestLoopFunctions::LED_OFFSET = CVector3::Y * 0.02;

   /****************************************/
   /****************************************/

   REGISTER_LOOP_FUNCTIONS(CTestLoopFunctions, "test_loop_functions");

}
