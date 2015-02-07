/**
 * @file <argos3/plugins/simulator/entities/rotor_equipped_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef ROTOR_EQUIPPED_ENTITY_H
#define ROTOR_EQUIPPED_ENTITY_H

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CRotorEquippedEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      CRotorEquippedEntity(CComposableEntity* pc_parent,
                           size_t un_num_rotors);

      CRotorEquippedEntity(CComposableEntity* pc_parent,
                           const std::string& str_id,
                           size_t un_num_rotors);

      virtual ~CRotorEquippedEntity();

      virtual void Reset();

      inline size_t GetNumRotors() const {
         return m_unNumRotors;
      }

      void SetRotor(UInt32 un_index,
                    const CVector3& c_position);

      const CVector3& GetRotorPosition(size_t un_index) const;

      inline const CVector3* GetRotorPositions() const {
         return m_pcRotorPositions;
      }

      Real GetRotorVelocity(size_t un_index) const;

      inline const Real* GetRotorVelocities() const {
         return m_pfRotorVelocities;
      }

      void SetVelocities(Real* pf_velocities);

      virtual std::string GetTypeDescription() const {
         return "rotors";
      }

   private:

      size_t m_unNumRotors;
      CVector3* m_pcRotorPositions;
      Real* m_pfRotorVelocities;

   };

}

#endif
