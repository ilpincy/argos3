#include "real_robot.h"
#include <argos3/core/utility/rate.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/control_interface/ci_controller.h>
#include <signal.h>
#include <unistd.h>

using namespace argos;

/****************************************/
/****************************************/

CRealRobot* CRealRobot::m_pcInstance = NULL;

/****************************************/
/****************************************/

CRealRobot::CRealRobot() :
  m_pcController(NULL) {
  /* Set instance */
  m_pcInstance = this;
}

/****************************************/
/****************************************/

void CRealRobot::Init(const std::string& str_conf_fname,
                      const std::string& str_controller_id) {
  /* Parse the .argos file */
  m_tConfiguration.LoadFile(str_conf_fname);
  m_tConfRoot = *m_tConfiguration.FirstChildElement();
  /*
   * Install signal handlers
   */
  ::signal(SIGINT, Cleanup);
  ::signal(SIGQUIT, Cleanup);
  ::signal(SIGABRT, Cleanup);
  ::signal(SIGTERM, Cleanup);
  /*
   * Get the control rate
   */
  TConfigurationNode& tFramework = GetNode(m_tConfRoot, "framework");
  TConfigurationNode& tExperiment = GetNode(tFramework, "experiment");
  GetNodeAttribute(tExperiment, "ticks_per_second", m_fRate);
  /*
   * Parse XML to identify the controller to run
   */
  std::string strControllerId, strControllerTag;
  TConfigurationNode& tControllers = GetNode(m_tConfRoot, "controllers");
  TConfigurationNodeIterator itControllers;
  /* Search for the controller tag with the given id */
  for(itControllers = itControllers.begin(&tControllers);
      itControllers != itControllers.end() && strControllerTag == "";
      ++itControllers) {
    GetNodeAttribute(*itControllers, "id", strControllerId);
    if(strControllerId == str_controller_id) {
      strControllerTag = itControllers->Value();
      m_ptControllerConfRoot = &(*itControllers);
    }
  }
  /* Make sure we found the tag */
  if(strControllerTag == "") {
    THROW_ARGOSEXCEPTION("Can't find controller with id \"" << str_controller_id << "\"");
  }
  /*
   * Initialize the robot
   */
  LOG << "[INFO] Robot initialization start" << std::endl;
  InitRobot();
  LOG << "[INFO] Robot initialization done" << std::endl;
  /*
   * Initialize the controller
   */
  LOG << "[INFO] Controller type '" << strControllerTag << "', id '" << str_controller_id << "' initialization start" << std::endl;
#ifdef ARGOS_DYNAMIC_LIBRARY_LOADING
  m_pcController = CFactory<CCI_Controller>::New(strControllerTag);
#else
  m_pcController = ControllerMaker(strControllerTag);
#endif
  /* Set the controller id using the machine hostname */
  char pchHostname[256];
  pchHostname[255] = '\0';
  ::gethostname(pchHostname, 255);
  m_pcController->SetId(pchHostname);
  LOG << "[INFO] Controller id set to '" << pchHostname << "'" << std::endl;
  /* Go through actuators */
  TConfigurationNode& tActuators = GetNode(*m_ptControllerConfRoot, "actuators");
  TConfigurationNodeIterator itAct;
  for(itAct = itAct.begin(&tActuators);
      itAct != itAct.end();
      ++itAct) {
    /* itAct->Value() is the name of the current actuator */
    CCI_Actuator* pcCIAct = MakeActuator(itAct->Value());
    if(pcCIAct == NULL) {
      THROW_ARGOSEXCEPTION("Unknown actuator \"" << itAct->Value() << "\"");
    }
    pcCIAct->Init(*itAct);
    m_pcController->AddActuator(itAct->Value(), pcCIAct);
  }
  /* Go through sensors */
  TConfigurationNode& tSensors = GetNode(*m_ptControllerConfRoot, "sensors");
  TConfigurationNodeIterator itSens;
  for(itSens = itSens.begin(&tSensors);
      itSens != itSens.end();
      ++itSens) {
    /* itSens->Value() is the name of the current sensor */
    CCI_Sensor* pcCISens = MakeSensor(itSens->Value());
    if(pcCISens == NULL) {
      THROW_ARGOSEXCEPTION("Unknown sensor \"" << itSens->Value() << "\"");
    }
    pcCISens->Init(*itSens);
    m_pcController->AddSensor(itSens->Value(), pcCISens);
  }
  /* Configure the controller */
  m_pcController->Init(GetNode(*m_ptControllerConfRoot, "params"));
  LOG << "[INFO] Controller type '" << strControllerTag << "', id '" << str_controller_id << "' initialization done" << std::endl;
}

/****************************************/
/****************************************/

CRealRobot::~CRealRobot() {
  if(m_pcController)
    delete m_pcController;
}

/****************************************/
/****************************************/

void CRealRobot::Control() {
  m_pcController->ControlStep();
}

/****************************************/
/****************************************/

void CRealRobot::Execute() {
  /* Enforce the control rate */
  CRate cRate(m_fRate);
  /* Main loop */
  LOG << "[INFO] Control loop running" << std::endl;
  /* Save current time */
  ::timeval tPast, tNow, tDiff;
  Real fElapsed;
  ::gettimeofday(&tPast, NULL);
  while(1) {
    /* Get elapsed time */
    ::gettimeofday(&tNow, NULL);
    timersub(&tNow, &tPast, &tDiff);
    fElapsed = static_cast<Real>(tDiff.tv_sec * 1000000 + tDiff.tv_usec) / 1e6;
    tPast = tNow;
    /* Do useful work */
    Sense(fElapsed);
    Control();
    Act(fElapsed);
    /* Sleep to enforce control rate */
    cRate.Sleep();
  }
}

/****************************************/
/****************************************/

void CRealRobot::Cleanup(int) {
  LOG << "[INFO] Stopping controller" << std::endl;
  if(m_pcInstance != NULL) {
    m_pcInstance->Destroy();
    delete m_pcInstance;
  }
  LOG << "[INFO] All done" << std::endl;
  exit(0);
}

/****************************************/
/****************************************/
