/**
 * @file <argos3/plugins/robots/foot-bot/simulator/distance_scanner_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "distance_scanner_equipped_entity.h"
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDistanceScannerEquippedEntity::CDistanceScannerEquippedEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent),
      m_unMode(MODE_OFF),
      m_fRotationSpeed(0.0f) {}
   
   /****************************************/
   /****************************************/

   CDistanceScannerEquippedEntity::CDistanceScannerEquippedEntity(CComposableEntity* pc_parent,
                                                                  const std::string& str_id) :
      CEntity(pc_parent, str_id),
      m_unMode(MODE_OFF),
      m_fRotationSpeed(0.0f) {}
   
   /****************************************/
   /****************************************/

   void CDistanceScannerEquippedEntity::Reset() {
      m_unMode = MODE_OFF;
      m_cRotation = CRadians::ZERO;
      m_fRotationSpeed = 0.0f;
   }

   /****************************************/
   /****************************************/

   void CDistanceScannerEquippedEntity::Update() {
      if(m_unMode == MODE_SPEED_CONTROL &&
         m_fRotationSpeed != 0.0f) {
         m_cRotation += CRadians(m_fRotationSpeed * CPhysicsEngine::GetSimulationClockTick());
         m_cRotation.UnsignedNormalize();
      }
   }

   /****************************************/
   /****************************************/

   void CDistanceScannerEquippedEntity::SetRotation(const CRadians& c_rotation) {
      m_cRotation = c_rotation;
      m_cRotation.UnsignedNormalize();
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CDistanceScannerEquippedEntity);

   /****************************************/
   /****************************************/

}
