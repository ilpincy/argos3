/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_electromagnet_system_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_electromagnet_system_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/plugins/robots/builderbot/simulator/builderbot_electromagnet_system_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotElectromagnetSystemDefaultActuator::CBuilderBotElectromagnetSystemDefaultActuator() :
      m_pcElectromagnetSystemEntity(nullptr) {}

   /****************************************/
   /****************************************/

   CBuilderBotElectromagnetSystemDefaultActuator::~CBuilderBotElectromagnetSystemDefaultActuator() {}
   
   /****************************************/
   /****************************************/

   void CBuilderBotElectromagnetSystemDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcElectromagnetSystemEntity = 
         &(c_entity.GetComponent<CBuilderBotElectromagnetSystemEntity>("electromagnet_system"));
      m_pcElectromagnetSystemEntity->Enable();
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotElectromagnetSystemActuator::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot electromagnet system actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/

   void CBuilderBotElectromagnetSystemDefaultActuator::Update() {
      if(m_bUpdateReq) {
         /* disabled by default */
         CBuilderBotElectromagnetSystemEntity::EDischargeMode eDischargeMode =
            CBuilderBotElectromagnetSystemEntity::EDischargeMode::DISABLED;
         switch(m_eDischargeMode) {
         case EDischargeMode::CONSTRUCTIVE:
            eDischargeMode =
               CBuilderBotElectromagnetSystemEntity::EDischargeMode::CONSTRUCTIVE;
            break;
         case EDischargeMode::DESTRUCTIVE:
            eDischargeMode =
               CBuilderBotElectromagnetSystemEntity::EDischargeMode::DESTRUCTIVE;
            break;
         default:
            /* default to disabled (set during initialization) */
            break;
         }
         m_pcElectromagnetSystemEntity->SetDischargeMode(eDischargeMode);
      }
      m_bUpdateReq = false;
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemDefaultActuator::Reset() {
      m_bUpdateReq = false;
      m_eDischargeMode = EDischargeMode::DISABLED;
   }
   
   /****************************************/
   /****************************************/


   REGISTER_ACTUATOR(CBuilderBotElectromagnetSystemDefaultActuator,
                  "builderbot_electromagnet_system", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot electromagnet system actuator.",
                  "This actuator controls the electromagnet system of the builderbot.",
                  "Usable"
   );

}

   
