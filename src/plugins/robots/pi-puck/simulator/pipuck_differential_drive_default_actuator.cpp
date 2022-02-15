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

   void CPiPuckDifferentialDriveDefaultActuator::SetLinearVelocity(Real f_left, Real f_right) {
      m_fTargetVelocityLeft = f_left;
      m_fTargetVelocityRight = f_right;
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CPiPuckDifferentialDriveDefaultActuator,
                     "pipuck_differential_drive", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "The Pi-Puck differential drive actuator.",
                     "The documentation for this actuator is \e]8;;https://github.com/ilpincy/argos3/blob/master/src/plugins/robots/pi-puck/README.md#pipuck_differential_drive-1\aavailable on Github\e]8;;\a.",
                     "Usable"
   );

   /****************************************/
   /****************************************/
   
}
