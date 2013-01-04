/**
 * @file <argos2/simulator/space/entities/distance_scanner_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli - <cpinciroli@ulb.ac.be>
 */

#include "distance_scanner_equipped_entity.h"
#include <argos2/simulator/physics_engines/physics_engine.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDistanceScannerEquippedEntity::CDistanceScannerEquippedEntity(CEntity* pc_parent) :
      CEntity(pc_parent),
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

   void CDistanceScannerEquippedEntity::UpdateRotation() {
      if(m_unMode == MODE_SPEED_CONTROL &&
         m_fRotationSpeed != 0.0f) {
         m_cRotation += CRadians(m_fRotationSpeed * CPhysicsEngine::GetSimulationClockTick());
         m_cRotation.UnsignedNormalize();
      }
   }

   /****************************************/
   /****************************************/

}
