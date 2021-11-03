/**
 * @file <argos3/plugins/robots/drone/simulator/drone_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_ENTITY_H
#define DRONE_ENTITY_H

namespace argos {
   class CControllableEntity;
   class CDirectionalLEDEquippedEntity;
   class CDroneFlightSystemEntity;
   class CEmbodiedEntity;
   class CRadioEquippedEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   class CDroneEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CDroneEntity();

      virtual ~CDroneEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual std::string GetTypeDescription() const {
         return "drone";
      }

      inline CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }

      inline CDirectionalLEDEquippedEntity& GetDirectionalLEDEquippedEntity() {
         return *m_pcDirectionalLEDEquippedEntity;
      }

      inline const CDirectionalLEDEquippedEntity& GetDirectionalLEDEquippedEntity() const {
         return *m_pcDirectionalLEDEquippedEntity;
      }

      inline CDroneFlightSystemEntity& GetFlightSystemEntity() {
         return *m_pcFlightSystemEntity;
      }

      inline const CDroneFlightSystemEntity& GetFlightSystemEntity() const {
         return *m_pcFlightSystemEntity;
      }

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline const CEmbodiedEntity& GetEmbodiedEntity() const {
         return *m_pcEmbodiedEntity;
      }

      inline CRadioEquippedEntity& GetRadioEquippedEntity() {
         return *m_pcRadioEquippedEntity;
      }

      inline const CRadioEquippedEntity& GetRadioEquippedEntity() const {
         return *m_pcRadioEquippedEntity;
      }

      inline bool IsDebug() const {
         return m_bDebug;
      }

      virtual void UpdateComponents();

   private:
      /* components */
      CControllableEntity*           m_pcControllableEntity;
      CDirectionalLEDEquippedEntity* m_pcDirectionalLEDEquippedEntity;
      CEmbodiedEntity*               m_pcEmbodiedEntity;
      CDroneFlightSystemEntity*      m_pcFlightSystemEntity;
      CRadioEquippedEntity*          m_pcRadioEquippedEntity;
      /* constants */
      static const Real WIFI_TRANSMISSION_RANGE;
      /* debug flag */
      bool m_bDebug;
   };

}

#endif
