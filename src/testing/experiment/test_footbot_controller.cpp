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
   RLOG << "CTestFootBotController::Init("
        << t_tree
        << ")"
        << std::endl;
   m_pcWheels = GetActuator<CCI_FootBotWheelsActuator>("footbot_wheels");
}

/****************************************/
/****************************************/

void CTestFootBotController::Reset() {
   RLOG << "CTestFootBotController::Reset()"
        << std::endl;
}

/****************************************/
/****************************************/

void CTestFootBotController::Destroy() {
   RLOG << "CTestFootBotController::Destroy()"
        << std::endl;
}

/****************************************/
/****************************************/

void CTestFootBotController::ControlStep() {
   RLOG << "CTestFootBotController::ControlStep()"
        << std::endl;
   m_pcWheels->SetLinearVelocity(5,5);
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CTestFootBotController, "test_footbot_controller");
