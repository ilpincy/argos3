/**
 * @file <argos3/plugins/simulator/entities/wheeled_entity.h>
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
                     size_t un_num_wheels);

      CWheeledEntity(CComposableEntity* pc_parent,
                     const std::string& str_id,
                     size_t un_num_wheels);

      virtual ~CWheeledEntity();

      virtual void Reset();

      inline size_t GetNumWheels() const {
         return m_unNumWheels;
      }

      void SetWheel(UInt32 un_index,
                    const CVector3& c_position,
                    Real f_radius);

      const CVector3& GetWheelPosition(size_t un_index) const;

      inline const CVector3* GetWheelPositions() const {
         return m_pcWheelPositions;
      }

      Real GetWheelRadius(size_t un_index) const;

      inline const Real* GetWheelRadia() const {
         return m_pfWheelRadia;
      }

      Real GetWheelVelocity(size_t un_index) const;

      inline const Real* GetWheelVelocities() const {
         return m_pfWheelVelocities;
      }

      void SetVelocities(Real* pf_velocities);

      virtual std::string GetTypeDescription() const {
         return "wheels";
      }

   private:

      size_t m_unNumWheels;
      CVector3* m_pcWheelPositions;
      Real* m_pfWheelRadia;
      Real* m_pfWheelVelocities;

   };

}

#endif
