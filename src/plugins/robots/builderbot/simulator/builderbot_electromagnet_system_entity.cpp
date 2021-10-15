/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_electromagnet_system_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_electromagnet_system_entity.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/simulator/space/space.h>

#define CAPACITOR_DISCHARGE_RATE 1.3f
#define CAPACITOR_RECHARGE_RATE 0.38f
#define CAPACITOR_MAX_VOLTAGE 22.9f
#define CAPACITOR_MIN_VOLTAGE 6.5f
#define ELECTROMAGNET_PASSIVE_FIELD 1.0f
#define ELECTROMAGNET_ACTIVE_FIELD 0.1f

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotElectromagnetSystemEntity::CBuilderBotElectromagnetSystemEntity(CComposableEntity* pc_parent,
                                                                              const std::string& str_id) :
      CEntity(pc_parent, str_id),
      m_eDischargeMode(EDischargeMode::DISABLED),
      m_fAccumulatedVoltage(0.0f) {}

   /****************************************/
   /****************************************/
   
   void CBuilderBotElectromagnetSystemEntity::Update() {
      Real fDelta = 0.0f;
      /* recharge */
      fDelta += CAPACITOR_RECHARGE_RATE *
         (CAPACITOR_MAX_VOLTAGE - m_fAccumulatedVoltage);
      /* discharge */
      if(m_eDischargeMode != EDischargeMode::DISABLED) {
         fDelta -= CAPACITOR_DISCHARGE_RATE *
            (m_fAccumulatedVoltage - CAPACITOR_MIN_VOLTAGE);
      }
      /* scale with respect to the simulation tick */
      fDelta *= CPhysicsEngine::GetSimulationClockTick();
      /* update the accumulated voltage */
      m_fAccumulatedVoltage += fDelta;
   }

   /****************************************/
   /****************************************/
 
   void CBuilderBotElectromagnetSystemEntity::Reset() {
      m_eDischargeMode = EDischargeMode::DISABLED;
      m_fAccumulatedVoltage = 0.0f;
   }

   /****************************************/
   /****************************************/

   Real CBuilderBotElectromagnetSystemEntity::GetField() const {
      Real fField = ELECTROMAGNET_PASSIVE_FIELD;
      switch(m_eDischargeMode) {
      case EDischargeMode::DISABLED:
         break;
      case EDischargeMode::CONSTRUCTIVE:
         fField += (ELECTROMAGNET_ACTIVE_FIELD * m_fAccumulatedVoltage); 
         break;
      case EDischargeMode::DESTRUCTIVE:
         fField -= (ELECTROMAGNET_ACTIVE_FIELD * m_fAccumulatedVoltage); 
         break;
      }
      return fField;
   }

   /****************************************/
   /****************************************/
   
   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CBuilderBotElectromagnetSystemEntity);

   /****************************************/
   /****************************************/

}
   
