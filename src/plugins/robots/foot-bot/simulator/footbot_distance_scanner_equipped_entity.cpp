/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_distance_scanner_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "footbot_distance_scanner_equipped_entity.h"
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CFootBotDistanceScannerEquippedEntity::CFootBotDistanceScannerEquippedEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent),
      m_unMode(MODE_OFF),
      m_fRotationSpeed(0.0f) {
      Disable();
   }
   
   /****************************************/
   /****************************************/

   CFootBotDistanceScannerEquippedEntity::CFootBotDistanceScannerEquippedEntity(CComposableEntity* pc_parent,
                                                                  const std::string& str_id) :
      CEntity(pc_parent, str_id),
      m_unMode(MODE_OFF),
      m_fRotationSpeed(0.0f) {
      Disable();
   }
   
   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerEquippedEntity::Reset() {
      m_unMode = MODE_OFF;
      m_cRotation = CRadians::ZERO;
      m_fRotationSpeed = 0.0f;
   }

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerEquippedEntity::Update() {
      if(m_unMode == MODE_SPEED_CONTROL &&
         m_fRotationSpeed != 0.0f) {
         m_cRotation += CRadians(m_fRotationSpeed *
                                 CPhysicsEngine::GetSimulationClockTick());
         m_cRotation.UnsignedNormalize();
      }
   }

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerEquippedEntity::SetRotation(const CRadians& c_rotation) {
      m_cRotation = c_rotation;
      m_cRotation.UnsignedNormalize();
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CFootBotDistanceScannerEquippedEntity);

   /****************************************/
   /****************************************/

}
