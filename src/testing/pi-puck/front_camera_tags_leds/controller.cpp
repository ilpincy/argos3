/**
 *
 * @author Michael Allwright <mallwright@learnrobotics.io>
 */

#include "controller.h"

#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_front_camera_sensor.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CTestController::Init(TConfigurationNode& t_tree) {
      m_pcFrontCameraSensor = GetSensor<CCI_PiPuckFrontCameraSensor>("pipuck_front_camera");
      Reset();
   }

   /****************************************/
   /****************************************/

   void CTestController::Reset() {
      m_pcFrontCameraSensor->Enable();
   }

   /****************************************/
   /****************************************/

   REGISTER_CONTROLLER(CTestController, "test_controller");

}



