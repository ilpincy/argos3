/**
 * @file <argos3/plugins/robots/prototype/simulator/prototype_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PROTOTYPE_ENTITY_H
#define PROTOTYPE_ENTITY_H

namespace argos {
   class CEmbodiedEntity;
   class CControllableEntity;
   class CPrototypeLinkEquippedEntity;
   class CPrototypeJointEquippedEntity;
   class CTagEquippedEntity;
   class CRadioEquippedEntity;
   class CMagnetEquippedEntity;
   class CCameraEquippedEntity;
   class CProximitySensorEquippedEntity; 
   class CDirectionalLEDEquippedEntity;
   class CLEDEquippedEntity;
   class CPrototypeEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   class CPrototypeEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CPrototypeEntity();

      virtual ~CPrototypeEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline CPrototypeLinkEquippedEntity& GetLinkEquippedEntity() {
         return *m_pcLinkEquippedEntity;
      }

      inline CPrototypeJointEquippedEntity& GetJointEquippedEntity() {
         return *m_pcJointEquippedEntity;
      }

      inline bool HasControllableEntity() const {
         return (m_pcControllableEntity != nullptr);
      }

      inline CControllableEntity& GetControllableEntity() {
         ARGOS_ASSERT(m_pcControllableEntity != nullptr,
                      "CPrototypeEntity::GetControllableEntity(), id=\"" <<
                      GetId() + GetContext() <<
                      "\": was not initialized with a controllable entity");
         return *m_pcControllableEntity;
      }

      inline bool HasDirectionalLEDEquippedEntity() const {
         return (m_pcDirectionalLEDEquippedEntity != nullptr);
      }

      inline CDirectionalLEDEquippedEntity& GetDirectionalLEDEquippedEntity() {
         ARGOS_ASSERT(m_pcDirectionalLEDEquippedEntity != nullptr,
                      "CPrototypeEntity::GetDirectionalLEDEquippedEntity(), id=\"" <<
                      GetId() + GetContext() <<
                      "\": was not initialized with an directional LED equipped entity");
         return *m_pcDirectionalLEDEquippedEntity;
      }

      inline bool HasLEDEquippedEntity() const {
         return (m_pcLEDEquippedEntity != nullptr);
      }

      inline CLEDEquippedEntity& GetLEDEquippedEntity() {
         ARGOS_ASSERT(m_pcLEDEquippedEntity != nullptr,
                      "CPrototypeEntity::GetLEDEquippedEntity(), id=\"" <<
                      GetId() + GetContext() <<
                      "\": was not initialized with an LED equipped entity");
         return *m_pcLEDEquippedEntity;
      }

      inline bool HasTagEquippedEntity() const {
         return (m_pcTagEquippedEntity != nullptr);
      }

      inline CTagEquippedEntity& GetTagEquippedEntity() {
         ARGOS_ASSERT(m_pcTagEquippedEntity != nullptr,
                      "CPrototypeEntity::GetTagEquippedEntity(), id=\"" <<
                      GetId() + GetContext() <<
                      "\": was not initialized with a tag equipped entity");
         return *m_pcTagEquippedEntity;
      }

      inline bool HasRadioEquippedEntity() const {
         return (m_pcRadioEquippedEntity != nullptr);
      }

      inline CRadioEquippedEntity& GetRadioEquippedEntity() {
         ARGOS_ASSERT(m_pcRadioEquippedEntity != nullptr,
                      "CPrototypeEntity::GetRadioEquippedEntity(), id=\"" <<
                      GetId() + GetContext() <<
                      "\": was not initialized with a radio equipped entity");
         return *m_pcRadioEquippedEntity;
      }

      inline bool HasMagnetEquippedEntity() const {
         return (m_pcMagnetEquippedEntity != nullptr);
      }

      inline CMagnetEquippedEntity& GetMagnetEquippedEntity() {
         ARGOS_ASSERT(m_pcMagnetEquippedEntity != nullptr,
                      "CPrototypeEntity::GetMagnetEquippedEntity(), id=\"" <<
                      GetId() + GetContext() <<
                      "\": was not initialized with a magnet equipped entity");
         return *m_pcMagnetEquippedEntity;
      }

      inline bool HasProximitySensorEquippedEntity() const {
         return (m_pcProximitySensorEquippedEntity != nullptr);
      }

      inline CProximitySensorEquippedEntity& GetProximitySensorEquippedEntity() {
         ARGOS_ASSERT(m_pcProximitySensorEquippedEntity != nullptr,
                      "CPrototypeEntity::GetProximitySensorEquippedEntity(), id=\"" <<
                      GetId() + GetContext() <<
                      "\": was not initialized with a proximity sensor equipped entity");
         return *m_pcProximitySensorEquippedEntity;
      }

      virtual std::string GetTypeDescription() const {
         return "prototype";
      }

   private:

      CEmbodiedEntity*                 m_pcEmbodiedEntity;
      CControllableEntity*             m_pcControllableEntity;
      CPrototypeLinkEquippedEntity*    m_pcLinkEquippedEntity;
      CPrototypeJointEquippedEntity*   m_pcJointEquippedEntity;
      CDirectionalLEDEquippedEntity*   m_pcDirectionalLEDEquippedEntity;
      CLEDEquippedEntity*              m_pcLEDEquippedEntity;
      CTagEquippedEntity*              m_pcTagEquippedEntity;
      CRadioEquippedEntity*            m_pcRadioEquippedEntity;
      CMagnetEquippedEntity*           m_pcMagnetEquippedEntity;
      CProximitySensorEquippedEntity*  m_pcProximitySensorEquippedEntity;

   };

}

#endif
