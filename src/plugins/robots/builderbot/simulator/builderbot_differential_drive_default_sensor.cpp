/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_differential_drive_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_differential_drive_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/plugins/robots/builderbot/simulator/builderbot_differential_drive_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>


namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotDifferentialDriveDefaultSensor::CBuilderBotDifferentialDriveDefaultSensor() :
      m_pcDifferentialDriveEntity(nullptr),
      m_fVelocityLeft(0.0f),
      m_fVelocityRight(0.0f) {}
 
   /****************************************/
   /****************************************/

   CBuilderBotDifferentialDriveDefaultSensor::~CBuilderBotDifferentialDriveDefaultSensor() {}
   
   /****************************************/
   /****************************************/

   void CBuilderBotDifferentialDriveDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      m_pcDifferentialDriveEntity = 
         &(c_entity.GetComponent<CBuilderBotDifferentialDriveEntity>("differential_drive"));
      m_pcDifferentialDriveEntity->Enable();
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotDifferentialDriveDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotDifferentialDriveSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot differential drive sensor.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CBuilderBotDifferentialDriveDefaultSensor::Update() {
      m_fVelocityLeft = m_pcDifferentialDriveEntity->GetVelocityLeft();
      m_fVelocityRight = m_pcDifferentialDriveEntity->GetVelocityRight();
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotDifferentialDriveDefaultSensor::Reset() {
      m_fVelocityLeft = Real(0);
      m_fVelocityRight = Real(0);
   }
   
   /****************************************/
   /****************************************/
   
}

// TODO should this macro be called inside argos:: 
REGISTER_SENSOR(CBuilderBotDifferentialDriveDefaultSensor,
                  "builderbot_differential_drive", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The builderbot differential drive sensor.",
                  "This sensor monitors the differential drive of the builderbot.",
                  "Usable"
   );
   
