/**
 * @file <argos3/plugins/robots/drone/simulator/drone_flight_system_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_FLIGHT_SYSTEM_ENTITY_H
#define DRONE_FLIGHT_SYSTEM_ENTITY_H

namespace argos {
   class CDroneFlightSystemEntity;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CDroneFlightSystemEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      CDroneFlightSystemEntity(CComposableEntity* pc_parent);

      CDroneFlightSystemEntity(CComposableEntity* pc_parent,
                               const std::string& str_id);

      /**
       * @brief Destructor.
       */
      virtual ~CDroneFlightSystemEntity() {}

      virtual void Update() {}

      virtual void Reset();

      void SetPositionReading(const CVector3& c_reading) {
         m_cPositionReading = c_reading;
      }

      const CVector3& GetPositionReading() const {
         return m_cPositionReading;
      }

      void SetOrientationReading(const CVector3& c_reading) {
         m_cOrientationReading = c_reading;
      }

      const CVector3& GetOrientationReading() const {
         return m_cOrientationReading;
      }

      void SetVelocityReading(const CVector3& c_reading) {
         m_cVelocityReading = c_reading;
      }

      const CVector3& GetVelocityReading() const {
         return m_cVelocityReading;
      }

      void SetAngularVelocityReading(const CVector3& c_reading) {
         m_cAngularVelocityReading = c_reading;
      }

      const CVector3& GetAngularVelocityReading() const {
         return m_cAngularVelocityReading;
      }

      void SetTargetPosition(const CVector3& c_position) {
         m_cTargetPosition = c_position;
      }

      const CVector3& GetTargetPosition() const {
         return m_cTargetPosition;
      }

      void SetTargetYawAngle(const CRadians f_yaw_angle) {
         m_fTargetYawAngle = f_yaw_angle;
      }

      const CRadians& GetTargetYawAngle() const {
         return m_fTargetYawAngle;
      }

      virtual std::string GetTypeDescription() const {
         return "flight_system";
      }

   private:
      CVector3 m_cPositionReading;
      CVector3 m_cOrientationReading;
      CVector3 m_cVelocityReading;
      CVector3 m_cAngularVelocityReading;
      CVector3 m_cTargetPosition;
      CRadians m_fTargetYawAngle;
      
   };
}

#endif
