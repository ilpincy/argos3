/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_lift_system_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_LIFT_SYSTEM_ENTITY_H
#define BUILDERBOT_LIFT_SYSTEM_ENTITY_H

namespace argos {
   class CBuilderBotLiftSystemEntity;
   struct SAnchor;
}

#include <argos3/core/simulator/entity/entity.h>

#include <array>

namespace argos {

   class CBuilderBotLiftSystemEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      enum class EState : UInt8 {
         INACTIVE = 0,
         POSITION_CONTROL = 1,
         SPEED_CONTROL = 2,
         CALIBRATION_SEARCH_TOP = 3,
         CALIBRATION_SEARCH_BOTTOM = 4,
      };

   public:

      CBuilderBotLiftSystemEntity(CComposableEntity* pc_parent,
                                  const std::string& str_id,
                                  SAnchor& s_anchor);

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotLiftSystemEntity() {}

      virtual void Update();

      virtual void Reset();

      SAnchor& GetAnchor() {
         return *m_psAnchor;
      }

      const SAnchor& GetAnchor() const {
         return *m_psAnchor;
      }

      virtual std::string GetTypeDescription() const {
         return "lift_system";
      }

      /* for the actuator interface */
      void SetTargetPosition(Real f_target_position) {       
         m_fTargetPosition = f_target_position;
         m_eState = EState::POSITION_CONTROL;
      }

      void Calibrate() {
         m_eState = EState::CALIBRATION_SEARCH_BOTTOM;
      }

      void Stop() {
         m_eState = EState::INACTIVE;
      }

      /* for the sensor interface */
      Real GetPosition() const {
         return m_fPosition;
      }

      const std::array<UInt8, 2>& GetLimitSwitches() const {
         return m_arrLimitSwitches;
      }

      EState GetState() const {
         return m_eState;
      }

      /* for the physics engines */
      Real GetTargetVelocity() const {
         return m_fTargetVelocity;
      }

   private:
      /* physics engine interface */
      SAnchor* m_psAnchor;
      Real m_fTargetVelocity;

      /* actuator interface */
      Real m_fTargetPosition;

      /* sensor interface */
      Real m_fPosition;
      std::array<UInt8, 2> m_arrLimitSwitches;
      EState m_eState;
   };
}

#endif
