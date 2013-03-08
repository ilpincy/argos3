
/**
 * @file <argos3/testing/experiment/test_footbot_controller.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "test_footbot_controller.h"
#include <argos3/core/utility/logging/argos_log.h>

/****************************************/
/****************************************/

CTestFootBotController::CTestFootBotController() {
}

/****************************************/
/****************************************/

CTestFootBotController::~CTestFootBotController() {
}

/****************************************/
/****************************************/

void CTestFootBotController::Init(TConfigurationNode& t_tree) {
   m_pcWheels    = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
   m_pcLEDs      = GetActuator<CCI_LEDsActuator>                ("leds"  );
   m_pcProximity = GetSensor  <CCI_FootBotProximitySensor>      ("footbot_proximity");
}

/****************************************/
/****************************************/

void CTestFootBotController::Reset() {
}

/****************************************/
/****************************************/

void CTestFootBotController::Destroy() {
}

/****************************************/
/****************************************/

void CTestFootBotController::ControlStep() {
   m_pcWheels->SetLinearVelocity(4,5);
   m_pcLEDs->SetAllColors(CColor::RED);
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CTestFootBotController, "test_footbot_controller");
