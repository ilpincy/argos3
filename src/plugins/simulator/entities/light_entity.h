/**
 * @file <argos3/plugins/simulator/entities/light_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef LIGHT_ENTITY_H
#define LIGHT_ENTITY_H

namespace argos {
   class CLightEntity;
   class CLedEquippedEntity;
}

#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/simulator/entity/led_equipped_entity.h>

namespace argos {

   class CLightEntity : public CPositionalEntity {

   public:

      ENABLE_VTABLE();

      CLightEntity();
      virtual ~CLightEntity() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      CLedEquippedEntity& GetLEDEquippedEntity() {
         return m_cLEDEquippedEntity;
      }

      CLedEntity& GetLEDEntity() {
         return m_cLEDEquippedEntity.GetLED(0);
      }

      virtual void Update() {}

      virtual void SetPosition(const CVector3& c_position) {
         CPositionalEntity::SetPosition(c_position);
         m_cLEDEquippedEntity.SetLedPosition(0, c_position);
      }

      virtual void SetColor(const CColor& c_color) {
         m_cLEDEquippedEntity.SetLedColor(0, c_color);
      }

      virtual std::string GetTypeDescription() const {
         return "light_entity";
      }

      inline Real GetIntensity() const {
         return m_fIntensity;
      }

      inline void SetIntensity(Real f_intensity) {
         m_fIntensity = f_intensity;
      }

   protected:

      CLedEquippedEntity m_cLEDEquippedEntity;
      Real m_fInitIntensity;
      Real m_fIntensity;
      CColor m_cInitColor;

   };

}

#endif
