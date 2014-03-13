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
         POSITION_CONTROL
      };

      struct SPositionControlData {
         CVector3 Position;
         CRadians Yaw;
      };

   public:

      CQuadRotorEntity(CComposableEntity* pc_parent,
                       Real f_arm_length);
      
      CQuadRotorEntity(CComposableEntity* pc_parent,
                       const std::string& str_id,
                       Real f_arm_length);

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

      virtual std::string GetTypeDescription() const {
         return "quadrotor";
      }

   private:

      Real m_fArmLength;
      EControlMethod m_eControlMethod;
      SPositionControlData m_sPositionControlData;

   };

}

#endif
