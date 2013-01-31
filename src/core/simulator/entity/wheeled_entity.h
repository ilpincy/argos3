/**
 * @file <argos3/core/simulator/entity/wheeled_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef WHEELED_ENTITY_H
#define WHEELED_ENTITY_H

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CWheeledEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      CWheeledEntity(CComposableEntity* pc_parent,
                     UInt32 un_num_wheels);

      CWheeledEntity(CComposableEntity* pc_parent,
                     const std::string& str_id,
                     UInt32 un_num_wheels);

      virtual ~CWheeledEntity();

      virtual void Reset();

      void GetSpeed(Real* pf_speeds);

      void SetSpeed(Real* pf_speeds);

      const CVector3& GetWheelPosition(UInt32 un_index) const {
         return m_pcWheelPositions[un_index];
      }

      void SetWheelPosition(UInt32 un_index,
                            const CVector3& c_position) {
         m_pcWheelPositions[un_index] = c_position;
      }

      virtual std::string GetTypeDescription() const {
         return "wheeled_entity";
      }

   private:

      UInt32 m_unNumWheels;
      CVector3* m_pcWheelPositions;
      Real* m_pfWheelSpeeds;

   };

}

#endif
