/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_turret_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "footbot_turret_entity.h"
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CFootBotTurretEntity::CFootBotTurretEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent),
      m_unMode(MODE_OFF),
      m_fRotationSpeed(0.0f) {
      Disable();
   }
   
   /****************************************/
   /****************************************/

   CFootBotTurretEntity::CFootBotTurretEntity(CComposableEntity* pc_parent,
                                              const std::string& str_id) :
      CEntity(pc_parent, str_id),
      m_unMode(MODE_OFF),
      m_fRotationSpeed(0.0f) {
      Disable();
   }
   
   /****************************************/
   /****************************************/

   void CFootBotTurretEntity::Reset() {
      m_unMode = MODE_OFF;
      m_cRotation = CRadians::ZERO;
      m_fRotationSpeed = 0.0f;
   }

   /****************************************/
   /****************************************/

   void CFootBotTurretEntity::SetRotation(const CRadians& c_rotation) {
      m_cRotation = c_rotation;
      m_cRotation.SignedNormalize();
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CFootBotTurretEntity);

   /****************************************/
   /****************************************/

}
