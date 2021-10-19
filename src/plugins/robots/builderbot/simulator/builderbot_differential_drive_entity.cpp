/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_differential_drive_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_differential_drive_entity.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotDifferentialDriveEntity::CBuilderBotDifferentialDriveEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent) {}

   /****************************************/
   /****************************************/

   CBuilderBotDifferentialDriveEntity::CBuilderBotDifferentialDriveEntity(CComposableEntity* pc_parent,
                                                                          const std::string& str_id) :
      CEntity(pc_parent, str_id),
      m_fTargetVelocityLeft(0.0f),
      m_fTargetVelocityRight(0.0f),
      m_fVelocityLeft(0.0f),
      m_fVelocityRight(0.0f) {}
  
   /****************************************/
   /****************************************/
    
   void CBuilderBotDifferentialDriveEntity::Reset() {
      m_fTargetVelocityLeft = 0.0f;
      m_fTargetVelocityRight = 0.0f;
      m_fVelocityLeft = 0.0f;
      m_fVelocityRight = 0.0f;
   }

   /****************************************/
   /****************************************/
   
   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CBuilderBotDifferentialDriveEntity);

   /****************************************/
   /****************************************/

}
   
