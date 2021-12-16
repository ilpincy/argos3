/**
 * @file <argos3/plugins/robots/builderbot/simulator/block_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BLOCK_ENTITY_H
#define BLOCK_ENTITY_H

namespace argos {
   class CControllableEntity;
   class CEmbodiedEntity;
   class CDirectionalLEDEquippedEntity;
   class CSimpleRadioEquippedEntity;
   class CTagEquippedEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   class CBlockEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CBlockEntity();

      virtual ~CBlockEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual std::string GetTypeDescription() const {
         return "block";
      }

      inline CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline CDirectionalLEDEquippedEntity& GetDirectionalLEDEquippedEntity() {
         return *m_pcDirectionalLEDEquippedEntity;
      }

      inline const CDirectionalLEDEquippedEntity& GetDirectionalLEDEquippedEntity() const {
         return *m_pcDirectionalLEDEquippedEntity;
      }

      inline CTagEquippedEntity& GetTagEquippedEntity() {
         return *m_pcTagEquippedEntity;
      }

      inline const CTagEquippedEntity& GetTagEquippedEntity() const {
         return *m_pcTagEquippedEntity;
      }

      inline CSimpleRadioEquippedEntity& GetSimpleRadioEquippedEntity() {
         return *m_pcSimpleRadioEquippedEntity;
      }

      inline const CSimpleRadioEquippedEntity& GetSimpleRadioEquippedEntity() const {
         return *m_pcSimpleRadioEquippedEntity;
      }

      virtual void UpdateComponents();

   private:

      CControllableEntity*              m_pcControllableEntity;
      CDirectionalLEDEquippedEntity*    m_pcDirectionalLEDEquippedEntity;
      CEmbodiedEntity*                  m_pcEmbodiedEntity;
      CTagEquippedEntity*               m_pcTagEquippedEntity;
      CSimpleRadioEquippedEntity*       m_pcSimpleRadioEquippedEntity;

      /* constants */
      static const Real TAG_SIDE_LENGTH;
      static const Real NFC_TRANSMISSION_RANGE;
      static const Real BLOCK_SIDE_LENGTH;
      static const std::array<std::tuple<std::string, CVector3, CQuaternion>, 6> FACE_DESCRIPTORS;
      static const std::array<CVector3, 4> LED_DESCRIPTORS;
   };

}

#endif
