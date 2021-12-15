/**
 * @file <argos3/plugins/robots/pi-puck/simulator/pipuck_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_ENTITY_H
#define PIPUCK_ENTITY_H

namespace argos {
   class CPiPuckDifferentialDriveEntity;
   class CControllableEntity;
   class CDirectionalLEDEquippedEntity;
   class CEmbodiedEntity;
   class CSimpleRadioEquippedEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   class CPiPuckEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CPiPuckEntity() :
         CComposableEntity(nullptr),
         m_pcControllableEntity(nullptr),
         m_pcEmbodiedEntity(nullptr),
         m_pcDifferentialDriveEntity(nullptr),
         m_pcSimpleRadioEquippedEntity(nullptr),
         m_bDebug(false) {}

      CPiPuckEntity(const std::string& str_id,
                    const std::string& str_controller_id,
                    const CVector3& c_position,
                    const CQuaternion& c_orientation,
                    bool b_debug = false,
                    const std::string& str_wifi_medium = "",
                    const std::string& str_led_medium = "");

      virtual ~CPiPuckEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual std::string GetTypeDescription() const {
         return "pipuck";
      }

      inline CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }

      inline CPiPuckDifferentialDriveEntity& GetDifferentialDriveEntity() {
         return *m_pcDifferentialDriveEntity;
      }

      inline const CPiPuckDifferentialDriveEntity& GetDifferentialDriveEntity() const {
         return *m_pcDifferentialDriveEntity;
      }

      inline CDirectionalLEDEquippedEntity& GetDirectionalLEDEquippedEntity() {
         return *m_pcDirectionalLEDEquippedEntity;
      }

      inline const CDirectionalLEDEquippedEntity& GetDirectionalLEDEquippedEntity() const {
         return *m_pcDirectionalLEDEquippedEntity;
      }

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline const CEmbodiedEntity& GetEmbodiedEntity() const {
         return *m_pcEmbodiedEntity;
      }

      inline CSimpleRadioEquippedEntity& GetSimpleRadioEquippedEntity() {
         return *m_pcSimpleRadioEquippedEntity;
      }

      inline const CSimpleRadioEquippedEntity& GetSimpleRadioEquippedEntity() const {
         return *m_pcSimpleRadioEquippedEntity;
      }

      inline bool IsDebug() const {
         return m_bDebug;
      }

      virtual void UpdateComponents();

   private:

      CControllableEntity*            m_pcControllableEntity;
      CDirectionalLEDEquippedEntity*  m_pcDirectionalLEDEquippedEntity;
      CEmbodiedEntity*                m_pcEmbodiedEntity;
      CPiPuckDifferentialDriveEntity* m_pcDifferentialDriveEntity;
      CSimpleRadioEquippedEntity*     m_pcSimpleRadioEquippedEntity;

      bool m_bDebug;

      static const CVector3 WIFI_OFFSET_POSITION;
      static const Real     WIFI_TRANSMISSION_RANGE;
   };

}

#endif
