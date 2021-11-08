/**
 * @file <argos3/plugins/robots/pipuck/simulator/pipuck_differential_drive_default_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pipuck_differential_drive_default_sensor.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/plugins/robots/pi-puck/simulator/pipuck_differential_drive_entity.h>
#include <argos3/plugins/robots/pi-puck/simulator/pipuck_entity.h>


namespace argos {

   /****************************************/
   /****************************************/

   CPiPuckDifferentialDriveDefaultSensor::CPiPuckDifferentialDriveDefaultSensor() :
      m_pcDifferentialDriveEntity(nullptr),
      m_fVelocityLeft(0.0f),
      m_fVelocityRight(0.0f) {}
 
   /****************************************/
   /****************************************/

   CPiPuckDifferentialDriveDefaultSensor::~CPiPuckDifferentialDriveDefaultSensor() {}
   
   /****************************************/
   /****************************************/

   void CPiPuckDifferentialDriveDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      m_pcDifferentialDriveEntity = 
         &(c_entity.GetComponent<CPiPuckDifferentialDriveEntity>("differential_drive"));
      m_pcDifferentialDriveEntity->Enable();
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckDifferentialDriveDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PiPuckDifferentialDriveSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the PiPuck differential drive sensor.", ex);
      }
   }
  
   /****************************************/
   /****************************************/
   
   void CPiPuckDifferentialDriveDefaultSensor::Update() {
      m_fVelocityLeft = m_pcDifferentialDriveEntity->GetVelocityLeft();
      m_fVelocityRight = m_pcDifferentialDriveEntity->GetVelocityRight();
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckDifferentialDriveDefaultSensor::Reset() {
      m_fVelocityLeft = Real(0);
      m_fVelocityRight = Real(0);
   }
   
   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CPiPuckDifferentialDriveDefaultSensor,
                  "pipuck_differential_drive", "default",
                  "Michael Allwright [allsey87@gmail.com]",
                  "1.0",
                  "The pipuck differential drive sensor.",
                  "This sensor monitors the differential drive of the pipuck.",
                  "Usable"
   );

   /****************************************/
   /****************************************/
   
}

   
