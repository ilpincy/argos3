/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_electromagnet_system_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_ELECTROMAGNET_SYSTEM_ENTITY_H
#define BUILDERBOT_ELECTROMAGNET_SYSTEM_ENTITY_H

namespace argos {
   class CBuilderBotElectromagnetSystemEntity;
}

#include <argos3/core/simulator/entity/entity.h>

namespace argos {

   class CBuilderBotElectromagnetSystemEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      enum class EDischargeMode : UInt8 {
         CONSTRUCTIVE = 0,
         DESTRUCTIVE = 1,
         DISABLED = 2,
      };

   public:

      CBuilderBotElectromagnetSystemEntity(CComposableEntity* pc_parent,
                                           const std::string& str_id);

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotElectromagnetSystemEntity() {}

      virtual void Update();

      virtual void Reset();

      virtual std::string GetTypeDescription() const {
         return "electromagnet_system";
      }

      void SetDischargeMode(EDischargeMode e_discharge_mode) {       
         m_eDischargeMode = e_discharge_mode;
      }

      Real GetAccumulatedVoltage() const {
         return m_fAccumulatedVoltage;
      }

      /* for the physics engines */
      Real GetField() const;

   private:
      /* actuator interface */
      EDischargeMode m_eDischargeMode;
      
      /* sensor interface */
      Real m_fAccumulatedVoltage;
   };
}

#endif
