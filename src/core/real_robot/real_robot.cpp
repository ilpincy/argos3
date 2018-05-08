#include "real_robot.h"
#include <argos3/core/utility/rate.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/control_interface/ci_controller.h>
#include <unistd.h>

using namespace argos;

/****************************************/
/****************************************/

CRealRobot::CRealRobot(const std::string& str_conf_fname,
                       const std::string& str_controller_id) :
   m_pcController(NULL) {
   /* Parse the .argos file */
   ticpp::Document tConfiguration;
   tConfiguration.LoadFile(str_conf_fname);
   m_tConfRoot = *tConfiguration.FirstChildElement();
   try {
      /*
       * Get the control rate
       */
      TConfigurationNode& tFramework = GetNode(m_tConfRoot, "framework");
      TConfigurationNode& tExperiment = GetNode(tFramework, "experiment");
      GetNodeAttribute(tExperiment, "ticks_per_second", m_fRate);
      /*
       * Create the controller
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
      /* Create the controller */
      m_pcController = ControllerMaker(strControllerTag);
   }
   catch(CARGoSException& ex) {
      LOGERR << ex.what() << std::endl;
   }
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
   /* Initialize the controller */
   InitController();
   /* Enforce the control rate */
   CRate cRate(m_fRate);
   /* Main loop */
   while(1) {
      /* Do useful work */
      Sense();
      Control();
      Act();
      /* Sleep to enforce control rate */
      cRate.Sleep();
   }
}

/****************************************/
/****************************************/

void CRealRobot::InitController() {
   /* Set the controller id using the machine hostname */
   char pchHostname[256];
   ::gethostname(pchHostname, 256);
   m_pcController->SetId(pchHostname);
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
   m_pcController->Init(*m_ptControllerConfRoot);
}

/****************************************/
/****************************************/
