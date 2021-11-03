/**
 * @file <argos3/plugins/robots/pi-puck/hardware/pipuck_differential_drive_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "pipuck_differential_drive_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/plugins/robots/pi-puck/simulator/pipuck_differential_drive_entity.h>
#include <argos3/plugins/robots/pi-puck/simulator/pipuck_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPiPuckDifferentialDriveDefaultActuator::CPiPuckDifferentialDriveDefaultActuator() :
      m_pcDifferentialDriveEntity(nullptr),
      m_fTargetVelocityLeft(0.0f),
      m_fTargetVelocityRight(0.0f) {}

   /****************************************/
   /****************************************/

   CPiPuckDifferentialDriveDefaultActuator::~CPiPuckDifferentialDriveDefaultActuator() {}
   
   /****************************************/
   /****************************************/

   void CPiPuckDifferentialDriveDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcDifferentialDriveEntity = 
         &(c_entity.GetComponent<CPiPuckDifferentialDriveEntity>("differential_drive"));
      m_pcDifferentialDriveEntity->Enable();
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckDifferentialDriveDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PiPuckDifferentialDriveActuator::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the PiPuck differential drive actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/

   void CPiPuckDifferentialDriveDefaultActuator::Update() {
      m_pcDifferentialDriveEntity->SetTargetVelocityLeft(m_fTargetVelocityLeft);
      m_pcDifferentialDriveEntity->SetTargetVelocityRight(m_fTargetVelocityRight);
   }

   /****************************************/
   /****************************************/
   
   void CPiPuckDifferentialDriveDefaultActuator::Reset() {
      m_fTargetVelocityLeft = Real(0);
      m_fTargetVelocityRight = Real(0);
   }
   
   /****************************************/
   /****************************************/

   void CPiPuckDifferentialDriveDefaultActuator::SetTargetVelocityLeft(Real f_target_velocity_left) {
      m_fTargetVelocityLeft = f_target_velocity_left;
   }

   /****************************************/
   /****************************************/

   void CPiPuckDifferentialDriveDefaultActuator::SetTargetVelocityRight(Real f_target_velocity_right) {
      m_fTargetVelocityRight = f_target_velocity_right;
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CPiPuckDifferentialDriveDefaultActuator,
                     "pipuck_differential_drive", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "The pipuck differential drive actuator.",
                     "This actuator controls the differential drive of the pipuck.",
                     "Usable"
   );

   /****************************************/
   /****************************************/
   
}
