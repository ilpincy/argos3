/**
 * @file <argos3/plugins/simulator/entities/quadrotor_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QUADROTOR_ENTITY_H
#define QUADROTOR_ENTITY_H

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CQuadRotorEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      enum EControlMethod {
         NO_CONTROL = 0,
         POSITION_CONTROL,
         SPEED_CONTROL
      };

      struct SPositionControlData {
         CVector3 Position;
         CRadians Yaw;

         SPositionControlData() {}
         SPositionControlData(const CVector3& c_position,
                              const CRadians& c_yaw);
      };

      struct SSpeedControlData {
         CVector3 Velocity;
         CRadians RotSpeed;

         SSpeedControlData() {}
         SSpeedControlData(const CVector3& c_velocity,
                           const CRadians& c_rot_speed);
      };

   public:

      CQuadRotorEntity(CComposableEntity* pc_parent);
      
      CQuadRotorEntity(CComposableEntity* pc_parent,
                       const std::string& str_id);

      virtual ~CQuadRotorEntity() {}

      virtual void Reset();

      EControlMethod GetControlMethod() const {
         return m_eControlMethod;
      }

      void SetControlMethod(EControlMethod e_control_method) {
         m_eControlMethod = e_control_method;
      }

      const SPositionControlData& GetPositionControlData() const {
         return m_sPositionControlData;
      }

      void SetPositionControlData(const SPositionControlData& s_data) {
         m_sPositionControlData = s_data;
      }

      const SSpeedControlData& GetSpeedControlData() const {
         return m_sSpeedControlData;
      }

      void SetSpeedControlData(const SSpeedControlData& s_data) {
         m_sSpeedControlData = s_data;
      }

      virtual std::string GetTypeDescription() const {
         return "quadrotor";
      }

   private:

      EControlMethod       m_eControlMethod;
      SPositionControlData m_sPositionControlData;
      SSpeedControlData    m_sSpeedControlData;

   };

}

#endif
