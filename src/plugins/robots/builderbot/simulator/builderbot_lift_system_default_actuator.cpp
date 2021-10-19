/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_lift_system_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_lift_system_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/plugins/robots/builderbot/simulator/builderbot_lift_system_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotLiftSystemDefaultActuator::CBuilderBotLiftSystemDefaultActuator() :
      m_pcLiftSystemEntity(nullptr) {}

   /****************************************/
   /****************************************/

   CBuilderBotLiftSystemDefaultActuator::~CBuilderBotLiftSystemDefaultActuator() {}
   
   /****************************************/
   /****************************************/

   void CBuilderBotLiftSystemDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcLiftSystemEntity = 
         &(c_entity.GetComponent<CBuilderBotLiftSystemEntity>("lift_system"));
      m_pcLiftSystemEntity->Enable();
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotLiftSystemDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotLiftSystemActuator::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot lift system actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/

   void CBuilderBotLiftSystemDefaultActuator::Update() {
      if(m_bStopReq) {
         m_pcLiftSystemEntity->Stop();
      }
      else if(m_bCalibrationReq) {
         m_pcLiftSystemEntity->Calibrate();
      }
      else if(m_bSetPositionReq) {
         Real fTargetPositionRaw = ConvertToRaw(m_fTargetPosition);
         m_pcLiftSystemEntity->SetTargetPosition(fTargetPositionRaw);
      }
      m_bStopReq = false;
      m_bCalibrationReq = false;
      m_bSetPositionReq = false;
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotLiftSystemDefaultActuator::Reset() {
      m_bStopReq = false;
      m_bCalibrationReq = false;
      m_bSetPositionReq = false;
   }
   
   /****************************************/
   /****************************************/


   REGISTER_ACTUATOR(CBuilderBotLiftSystemDefaultActuator,
                  "builderbot_lift_system", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot lift system actuator.",
                  "This actuator controls the lift system of the builderbot.",
                  "Usable"
   );

}

   
