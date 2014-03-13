#include "test_eyebot_controller.h"
#include <argos3/core/utility/logging/argos_log.h>

/****************************************/
/****************************************/

CTestEyeBotController::CTestEyeBotController() {
   DEBUG_FUNCTION_ENTER;
   DEBUG_FUNCTION_EXIT;
}

/****************************************/
/****************************************/

CTestEyeBotController::~CTestEyeBotController() {
   DEBUG_FUNCTION_ENTER;
   DEBUG_FUNCTION_EXIT;
}

/****************************************/
/****************************************/

void CTestEyeBotController::Init(TConfigurationNode& t_tree) {
   DEBUG_FUNCTION_ENTER;
   // m_pcRABAct    = GetActuator<CCI_RangeAndBearingActuator>     ("range_and_bearing");
   // m_pcRABSens   = GetSensor  <CCI_RangeAndBearingSensor>       ("range_and_bearing");
   DEBUG_FUNCTION_EXIT;
}

/****************************************/
/****************************************/

void CTestEyeBotController::Reset() {
   DEBUG_FUNCTION_ENTER;
   DEBUG_FUNCTION_EXIT;
}

/****************************************/
/****************************************/

void CTestEyeBotController::Destroy() {
   DEBUG_FUNCTION_ENTER;
   DEBUG_FUNCTION_EXIT;
}

/****************************************/
/****************************************/

void CTestEyeBotController::ControlStep() {
   DEBUG_FUNCTION_ENTER;
   DEBUG_FUNCTION_EXIT;
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CTestEyeBotController, "test_eyebot_controller");
