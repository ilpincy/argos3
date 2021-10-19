/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_lift_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_lift_system_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/plugins/robots/builderbot/simulator/builderbot_lift_system_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>


namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotLiftSystemDefaultSensor::CBuilderBotLiftSystemDefaultSensor() :
      m_pcLiftSystemEntity(nullptr) {}
 
   /****************************************/
   /****************************************/

   CBuilderBotLiftSystemDefaultSensor::~CBuilderBotLiftSystemDefaultSensor() {}
   
   /****************************************/
   /****************************************/

   void CBuilderBotLiftSystemDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      m_pcLiftSystemEntity = 
         &(c_entity.GetComponent<CBuilderBotLiftSystemEntity>("lift_system"));
      m_pcLiftSystemEntity->Enable();
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotLiftSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotLiftSystemSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot lift system sensor.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotLiftSystemDefaultSensor::Update() {
      m_fPosition = m_pcLiftSystemEntity->GetPosition();
      m_arrLimitSwitches = m_pcLiftSystemEntity->GetLimitSwitches();
      m_unState = static_cast<UInt8>(m_pcLiftSystemEntity->GetState());
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotLiftSystemDefaultSensor::Reset() {
      m_fPosition = 0.0f;
      m_arrLimitSwitches = {0u, 0u};
      m_unState = static_cast<UInt8>(CBuilderBotLiftSystemEntity::EState::INACTIVE);
   }
   
   /****************************************/
   /****************************************/
   
   REGISTER_SENSOR(CBuilderBotLiftSystemDefaultSensor,
                   "builderbot_lift_system", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The builderbot lift system sensor.",
                   "This sensor monitors the lift system of the builderbot.",
                   "Usable"
   );

}

   
