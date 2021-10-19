/**
 * @file <argos3/plugins/robots/builderbot/hardware/builderbot_differential_drive_default_actuator.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_differential_drive_default_actuator.h"

#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/plugins/robots/builderbot/simulator/builderbot_differential_drive_entity.h>
#include <argos3/plugins/robots/builderbot/simulator/builderbot_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotDifferentialDriveDefaultActuator::CBuilderBotDifferentialDriveDefaultActuator() :
      m_pcDifferentialDriveEntity(nullptr),
      m_fTargetVelocityLeft(0.0f),
      m_fTargetVelocityRight(0.0f) {}

   /****************************************/
   /****************************************/

   CBuilderBotDifferentialDriveDefaultActuator::~CBuilderBotDifferentialDriveDefaultActuator() {}
   
   /****************************************/
   /****************************************/

   void CBuilderBotDifferentialDriveDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcDifferentialDriveEntity = 
         &(c_entity.GetComponent<CBuilderBotDifferentialDriveEntity>("differential_drive"));
      m_pcDifferentialDriveEntity->Enable();
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotDifferentialDriveDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BuilderBotDifferentialDriveActuator::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in the BuilderBot differential drive actuator.", ex);
      }
   }
  
   /****************************************/
   /****************************************/

   void CBuilderBotDifferentialDriveDefaultActuator::Update() {
      m_pcDifferentialDriveEntity->SetTargetVelocityLeft(m_fTargetVelocityLeft);
      m_pcDifferentialDriveEntity->SetTargetVelocityRight(m_fTargetVelocityRight);
   }

   /****************************************/
   /****************************************/
   
   void CBuilderBotDifferentialDriveDefaultActuator::Reset() {
      m_fTargetVelocityLeft = Real(0);
      m_fTargetVelocityRight = Real(0);
   }
   
   /****************************************/
   /****************************************/

   void CBuilderBotDifferentialDriveDefaultActuator::SetTargetVelocityLeft(Real f_target_velocity_left) {
      m_fTargetVelocityLeft = f_target_velocity_left;
   }

   /****************************************/
   /****************************************/

   void CBuilderBotDifferentialDriveDefaultActuator::SetTargetVelocityRight(Real f_target_velocity_right) {
      m_fTargetVelocityRight = f_target_velocity_right;
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CBuilderBotDifferentialDriveDefaultActuator,
                     "builderbot_differential_drive", "default",
                     "Michael Allwright [allsey87@gmail.com]",
                     "1.0",
                     "The builderbot differential drive actuator.",
                     "This actuator controls the differential drive of the builderbot.",
                     "Usable"
   );

   /****************************************/
   /****************************************/
   
}
