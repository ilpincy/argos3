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
   class CGroundSensorEquippedEntity;
   class CLEDEquippedEntity;
   class CLightSensorEquippedEntity;
   class CProximitySensorEquippedEntity;
   class CRABEquippedEntity;
   class CWiFiEquippedEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/wheeled_entity.h>

namespace argos {

   class CFootBotEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CFootBotEntity();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      virtual void UpdateComponents();
      
      inline CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }

      inline CDistanceScannerEquippedEntity& GetDistanceScannerEquippedEntity() {
         return *m_pcDistanceScannerEquippedEntity;
      }

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline CGripperEquippedEntity& GetGripperEquippedEntity() {
         return *m_pcGripperEquippedEntity;
      }

      inline CGroundSensorEquippedEntity& GetGroundSensorEquippedEntity() {
         return *m_pcGroundSensorEquippedEntity;
      }

      inline CLEDEquippedEntity& GetLEDEquippedEntity() {
         return *m_pcLEDEquippedEntity;
      }

      inline CLightSensorEquippedEntity& GetLightSensorEquippedEntity() {
         return *m_pcLightSensorEquippedEntity;
      }

      inline CProximitySensorEquippedEntity& GetProximitySensorEquippedEntity() {
         return *m_pcProximitySensorEquippedEntity;
      }

      inline CRABEquippedEntity& GetRABEquippedEntity() {
         return *m_pcRABEquippedEntity;
      }

      inline CWheeledEntity& GetWheeledEntity() {
         return *m_pcWheeledEntity;
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
         return "footbot";
      }

   private:

      void SetLEDPosition();

   private:

      CControllableEntity*            m_pcControllableEntity;
      CDistanceScannerEquippedEntity* m_pcDistanceScannerEquippedEntity;
      CEmbodiedEntity*                m_pcEmbodiedEntity;
      CGripperEquippedEntity*         m_pcGripperEquippedEntity;
      CGroundSensorEquippedEntity*    m_pcGroundSensorEquippedEntity;
      CLEDEquippedEntity*             m_pcLEDEquippedEntity;
      CLightSensorEquippedEntity*     m_pcLightSensorEquippedEntity;
      CProximitySensorEquippedEntity* m_pcProximitySensorEquippedEntity;
      CRABEquippedEntity*             m_pcRABEquippedEntity;
      CWheeledEntity*                 m_pcWheeledEntity;
      CWiFiEquippedEntity*            m_pcWiFiEquippedEntity;

      CRadians                        m_cTurretRotation;
      Real                            m_fTurretRotationSpeed;
      UInt8                           m_unTurretMode;

   };

}

#endif
