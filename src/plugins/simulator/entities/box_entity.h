/**
 * @file <argos3/plugins/simulator/entities/box_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef BOX_ENTITY_H
#define BOX_ENTITY_H

namespace argos {
   class CBoxEntity;
}

#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/led_equipped_entity.h>

namespace argos {

   class CBoxEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

      CBoxEntity();

      CBoxEntity(const std::string& str_id,
                 const CVector3& c_position,
                 const CQuaternion& c_orientation,
                 bool b_movable,
                 const CVector3& c_size,
                 Real f_mass = 1.0f);

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline CLEDEquippedEntity& GetLEDEquippedEntity() {
         return *m_pcLEDEquippedEntity;
      }

      inline const CVector3& GetSize() const {
         return m_cSize;
      }

      inline void SetSize(const CVector3& c_size) {
         m_cSize = c_size;
      }

      inline Real GetMass() const {
         return m_fMass;
      }

      inline void SetMass(Real f_mass) {
         m_fMass = f_mass;
      }

      virtual std::string GetTypeDescription() const {
         return "box_entity";
      }

   private:

      CEmbodiedEntity*      m_pcEmbodiedEntity;
      CLEDEquippedEntity*   m_pcLEDEquippedEntity;
      CVector3              m_cSize;
      Real                  m_fMass;

   };

}

#endif
