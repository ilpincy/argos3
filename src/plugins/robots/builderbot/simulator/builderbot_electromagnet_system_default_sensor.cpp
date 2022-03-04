/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_electromagnet_system_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_electromagnet_system_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/plugins/robots/builderbot/simulator/builderbot_electromagnet_system_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>


namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotElectromagnetSystemDefaultSensor::CBuilderBotElectromagnetSystemDefaultSensor() :
      m_pcElectromagnetSystemEntity(nullptr) {}
 
   /****************************************/
   /****************************************/

   CBuilderBotElectromagnetSystemDefaultSensor::~CBuilderBotElectromagnetSystemDefaultSensor() {}
   
   /****************************************/
   /****************************************/

   void CBuilderBotElectromagnetSystemDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      m_pcElectromagnetSystemEntity = 
         &(c_entity.GetComponent<CBuilderBotElectromagnetSystemEntity>("electromagnet_system"));
      m_pcElectromagnetSystemEntity->Enable();
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotElectromagnetSystemSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot electromagnet system sensor.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemDefaultSensor::Update() {
      m_fVoltage = m_pcElectromagnetSystemEntity->GetAccumulatedVoltage();
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemDefaultSensor::Reset() {
      m_fVoltage = 0.0f;
   }
   
   /****************************************/
   /****************************************/
   
   REGISTER_SENSOR(CBuilderBotElectromagnetSystemDefaultSensor,
                   "builderbot_electromagnet_system", "default",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "The builderbot electromagnet system sensor.",
                   "The documentation for this sensor is \e]8;;https://github.com/ilpincy/argos3/blob/master/src/plugins/robots/builderbot/README.md#builderbot_electromagnet_system\aavailable on Github\e]8;;\a.",
                   "Usable"
   );

}

   
