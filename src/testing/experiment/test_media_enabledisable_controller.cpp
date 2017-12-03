#include "test_media_enabledisable_controller.h"
#include <argos3/core/utility/logging/argos_log.h>

/****************************************/
/****************************************/

CTestMediaEnableDisableController::CTestMediaEnableDisableController() {
}

/****************************************/
/****************************************/

CTestMediaEnableDisableController::~CTestMediaEnableDisableController() {
}

/****************************************/
/****************************************/

void CTestMediaEnableDisableController::Init(TConfigurationNode& t_node) {
   DEBUG_FUNCTION_ENTER;
   DEBUG("ROBOT %s\n", GetId().c_str());
   try {
      m_pcLEDs  = GetActuator<CCI_LEDsActuator           >("leds"             );
      m_pcRABA  = GetActuator<CCI_RangeAndBearingActuator>("range_and_bearing");
      m_pcRABS  = GetSensor  <CCI_RangeAndBearingSensor  >("range_and_bearing");
      m_pcLight = GetSensor  <CCI_FootBotLightSensor     >("footbot_light"    );
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing the controller for robot \"" << GetId() << "\"", ex);
   }
   DEBUG_FUNCTION_EXIT;
}

/****************************************/
/****************************************/

void CTestMediaEnableDisableController::ControlStep() {
   DEBUG_FUNCTION_ENTER;
   DEBUG("ROBOT %s\n", GetId().c_str());
   DEBUG_FUNCTION_EXIT;
}

/****************************************/
/****************************************/

void CTestMediaEnableDisableController::Reset() {
   DEBUG_FUNCTION_ENTER;
   DEBUG("ROBOT %s\n", GetId().c_str());
   DEBUG_FUNCTION_EXIT;
}

/****************************************/
/****************************************/

void CTestMediaEnableDisableController::Destroy() {
   DEBUG_FUNCTION_ENTER;
   DEBUG("ROBOT %s\n", GetId().c_str());
   DEBUG_FUNCTION_EXIT;
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CTestMediaEnableDisableController, "test_media_enabledisable_controller")
