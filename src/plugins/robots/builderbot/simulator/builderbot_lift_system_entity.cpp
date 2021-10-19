/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_lift_system_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "builderbot_lift_system_entity.h"

#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/simulator/space/space.h>

#define LOWER_LIMIT_SWITCH_THRESHOLD 0.0010f
#define UPPER_LIMIT_SWITCH_THRESHOLD 0.1375f
#define LIFT_ALLOWABLE_ERROR 0.001f
#define LIFT_MIN_VELOCITY 0.010f
#define LIFT_MAX_VELOCITY 0.018f
#define LIFT_GAIN 0.50f

namespace argos {

   /****************************************/
   /****************************************/

   CBuilderBotLiftSystemEntity::CBuilderBotLiftSystemEntity(CComposableEntity* pc_parent,
                                                            const std::string& str_id,
                                                            SAnchor& s_anchor) :
      CEntity(pc_parent, str_id),
      m_psAnchor(&s_anchor),
      m_fTargetVelocity(0.0f),
      m_fTargetPosition(0.0f),
      m_fPosition(0.0f),
      m_arrLimitSwitches({0u, 0u}),
      m_eState(EState::INACTIVE) {}

   /****************************************/
   /****************************************/
   
   void CBuilderBotLiftSystemEntity::Update() {
      /* get the position from the anchor */
      m_fPosition = m_psAnchor->Position.GetZ() -
         m_psAnchor->OffsetPosition.GetZ();
      /* update the limit switches */
      m_arrLimitSwitches[0] =
         m_fPosition > UPPER_LIMIT_SWITCH_THRESHOLD ? 1 : 0;
      m_arrLimitSwitches[1] =
         m_fPosition < LOWER_LIMIT_SWITCH_THRESHOLD ? 1 : 0;
      /* step state machine */
      switch(m_eState) {
         case EState::INACTIVE:
         m_fTargetVelocity = 0;
         break;
      case EState::POSITION_CONTROL:
         if(std::fabs(m_fTargetPosition - m_fPosition) < LIFT_ALLOWABLE_ERROR) {
            m_eState = EState::INACTIVE;
            m_fTargetVelocity = 0;
         }
         else {
            m_fTargetVelocity = (m_fTargetPosition - m_fPosition) * LIFT_GAIN;
            if(std::fabs(m_fTargetVelocity) > LIFT_MAX_VELOCITY) {
               m_fTargetVelocity = std::copysign(LIFT_MAX_VELOCITY, m_fTargetVelocity);
            }
            if(std::fabs(m_fTargetVelocity) < LIFT_MIN_VELOCITY) {
               m_fTargetVelocity = std::copysign(LIFT_MIN_VELOCITY, m_fTargetVelocity);
            }
            /* check limit switches */
            if((m_fTargetVelocity > 0 && m_arrLimitSwitches[0] == 1) || 
               (m_fTargetVelocity < 0 && m_arrLimitSwitches[1] == 1)) {
               m_eState = EState::INACTIVE;
               m_fTargetVelocity = 0;
            }
         }
         break;
      case EState::CALIBRATION_SEARCH_TOP:
         m_fTargetVelocity = LIFT_MAX_VELOCITY;
         if(m_arrLimitSwitches[0] == 1) {
            m_eState = EState::INACTIVE;
         }
         break;
      case EState::CALIBRATION_SEARCH_BOTTOM:
         m_fTargetVelocity = -LIFT_MAX_VELOCITY;
         if(m_arrLimitSwitches[1] == 1) {
            m_eState = EState::CALIBRATION_SEARCH_TOP;
         }
         break;
      default:
         THROW_ARGOSEXCEPTION("Unimplemented state reached in the lift system entity");
         break;
      }
   }

   /****************************************/
   /****************************************/
 
   void CBuilderBotLiftSystemEntity::Reset() {
      m_fTargetVelocity = 0.0f;
      m_fTargetPosition = 0.0f;
      m_fPosition = 0.0f;
      m_arrLimitSwitches = {0u, 0u};
      m_eState = EState::INACTIVE;
   }

   /****************************************/
   /****************************************/
   
   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CBuilderBotLiftSystemEntity);

   /****************************************/
   /****************************************/

}
   
