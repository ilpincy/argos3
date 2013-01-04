/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FOOTBOT_ENTITY_H
#define FOOTBOT_ENTITY_H

namespace argos {
   class CControllableEntity;
   class CDistanceScannerEquippedEntity;
   class CEmbodiedEntity;
   class CFootBotEntity;
   class CGripperEquippedEntity;
   class CLedEquippedEntity;
   class CQuaternion;
   class CRABEquippedEntity;
   class CWiFiEquippedEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/wheeled_entity.h>

namespace argos {

   class CFootBotEntity : public CComposableEntity {

   public:

      CFootBotEntity();
      virtual ~CFootBotEntity();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      virtual CEntity& GetComponent(const std::string& str_component);
      virtual bool HasComponent(const std::string& str_component);

      virtual void UpdateComponents();
      
      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }

      inline CWheeledEntity<2>& GetWheeledEntity() {
         return *m_pcWheeledEntity;
      }

      inline CLedEquippedEntity& GetLEDEquippedEntity() {
         return *m_pcLEDEquippedEntity;
      }

      inline CGripperEquippedEntity& GetGripperEquippedEntity() {
         return *m_pcGripperEquippedEntity;
      }

      inline CDistanceScannerEquippedEntity& GetDistanceScannerEquippedEntity() {
         return *m_pcDistanceScannerEquippedEntity;
      }

      inline CRABEquippedEntity& GetRABEquippedEntity() {
         return *m_pcRABEquippedEntity;
      }

      inline CWiFiEquippedEntity& GetWiFiEquippedEntity() {
         return *m_pcWiFiEquippedEntity;
      }

      inline const CRadians& GetTurretRotation() const {
         return m_cTurretRotation;
      }

      inline void SetTurretRotation(const CRadians& c_rotation) {
         m_cTurretRotation = c_rotation;
      }

      inline Real GetTurretRotationSpeed() const {
         return m_fTurretRotationSpeed;
      }

      inline void SetTurretRotationSpeed(Real f_speed) {
         m_fTurretRotationSpeed = f_speed;
      }

      inline UInt8 GetTurretMode() const {
         return m_unTurretMode;
      }

      inline void SetTurretMode(UInt8 un_mode) {
         m_unTurretMode = un_mode;
      }

      virtual std::string GetTypeDescription() const {
         return "footbot_entity";
      }

   private:

      void SetLedPosition();

   private:

      CEmbodiedEntity*                m_pcEmbodiedEntity;
      CControllableEntity*            m_pcControllableEntity;
      CWheeledEntity<2>*              m_pcWheeledEntity;
      CLedEquippedEntity*             m_pcLEDEquippedEntity;
      CGripperEquippedEntity*         m_pcGripperEquippedEntity;
      CDistanceScannerEquippedEntity* m_pcDistanceScannerEquippedEntity;
      CRABEquippedEntity*             m_pcRABEquippedEntity;
      CWiFiEquippedEntity*            m_pcWiFiEquippedEntity;

      CRadians                        m_cTurretRotation;
      Real                            m_fTurretRotationSpeed;
      UInt8                           m_unTurretMode;

   };

}

#endif
