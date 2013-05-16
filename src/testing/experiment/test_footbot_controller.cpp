
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
   //m_pcWheels    = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
   m_pcRABAct    = GetActuator<CCI_RangeAndBearingActuator>     ("range_and_bearing");
   //m_pcProximity = GetSensor  <CCI_FootBotProximitySensor>      ("footbot_proximity");
   m_pcRABSens   = GetSensor  <CCI_RangeAndBearingSensor>       ("range_and_bearing");
   m_cGoStraightRange.Set(ToRadians(CDegrees(-10.0f)),
                          ToRadians(CDegrees(10.0f)));
   m_fDelta = 0.1f;
   m_pcRABAct->SetData(0, 17);
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
   // const CCI_FootBotProximitySensor::TReadings& tProxReadings = m_pcProximity->GetReadings();
   // m_cAccumulator.Set(0.0f, 0.0f);
   // for(size_t i = 0; i < tProxReadings.size(); ++i) {
   //    m_cAccumulator += CVector2(tProxReadings[i].Value, tProxReadings[i].Angle);
   // }
   // m_cAccumulator /= tProxReadings.size();
   // m_cAngle = m_cAccumulator.Angle();
   // if(m_cGoStraightRange.WithinMinBoundExcludedMaxBoundExcluded(m_cAngle) &&
   //    m_cAccumulator.Length() < m_fDelta) {
   //    m_pcWheels->SetLinearVelocity(5,5);
   // }
   // else if(m_cAngle > CRadians::ZERO) {
   //    m_pcWheels->SetLinearVelocity(5,0);
   // }
   // else {
   //    m_pcWheels->SetLinearVelocity(0,5);
   // }
}

/****************************************/
/****************************************/

REGISTER_CONTROLLER(CTestFootBotController, "test_footbot_controller");
