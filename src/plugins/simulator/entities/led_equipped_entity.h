/**
 * @file <argos3/plugins/simulator/entities/led_equipped_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef LED_EQUIPPED_ENTITY_H
#define LED_EQUIPPED_ENTITY_H

namespace argos {
   class CLEDEquippedEntity;
   class CLEDEntity;
}

#include <argos3/core/simulator/entity/led_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <map>

namespace argos {

   class CLEDEquippedEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

      typedef std::map<std::string, CLEDEquippedEntity*> TMap;

   public:

      CLEDEquippedEntity(CComposableEntity* pc_parent,
                         CPositionalEntity* pc_reference);

      CLEDEquippedEntity(CComposableEntity* pc_parent,
                         const std::string& str_id,
                         CPositionalEntity* pc_reference);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Update() {}

      void AddLED(const CVector3& c_position,
                  const CColor& c_color = CColor::BLACK);

      void AddLEDRing(const CVector3& c_center,
                      Real f_radius,
                      const CRadians& c_start_angle,
                      UInt32 un_num_leds,
                      const CColor& c_color = CColor::BLACK);

      CLEDEntity& GetLED(UInt32 un_index);

      inline CLEDEntity::TList& GetAllLEDs() {
         return m_tLEDs;
      }

      void SetLEDPosition(UInt32 un_index,
                          const CVector3& c_position);

      void SetLEDColor(UInt32 un_index,
                       const CColor& c_color);

      void SetAllLEDsColors(const CColor& c_color);

      void SetAllLEDsColors(const std::vector<CColor>& vec_colors);

      bool HasReferenceEntity() const {
         return m_pcReferenceEntity != NULL;
      }

      CPositionalEntity& GetReferenceEntity() {
         return *m_pcReferenceEntity;
      }

      virtual std::string GetTypeDescription() const {
         return "leds";
      }

   protected:

      virtual void UpdateComponents();

   protected:

      CLEDEntity::TList m_tLEDs;
      std::vector<CVector3> m_vecLEDOffsetPositions;
      CPositionalEntity* m_pcReferenceEntity;

   };

}

#endif
