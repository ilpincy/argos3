
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
   RLOG << "[DEBUG] CTestFootBotController::Init("
        << t_tree
        << ")"
        << std::endl;
   m_pcWheels    = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
   m_pcLEDs      = GetActuator<CCI_LEDsActuator>                ("leds"  );
   m_pcProximity = GetSensor  <CCI_FootBotProximitySensor>      ("footbot_proximity");
}

/****************************************/
/****************************************/

void CTestFootBotController::Reset() {
   RLOG << "[DEBUG] CTestFootBotController::Reset()"
        << std::endl;
}

/****************************************/
/****************************************/

void CTestFootBotController::Destroy() {
   RLOG << "[DEBUG] CTestFootBotController::Destroy()"
        << std::endl;
}

/****************************************/
/****************************************/

void CTestFootBotController::ControlStep() {
   RLOG << "[DEBUG] CTestFootBotController::ControlStep()"
        << std::endl;
   m_pcWheels->SetLinearVelocity(4,5);
   m_pcLEDs->SetAllColors(CColor::RED);
   RLOGERR << "[DEBUG] Proximity = " << m_pcProximity->GetReadings()[0];
   for(size_t i = 1; i < m_pcProximity->GetReadings().size(); ++i) {
      LOGERR << "," << m_pcProximity->GetReadings()[i];
   }
   LOGERR << std::endl;
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CTestFootBotController, "test_footbot_controller");
