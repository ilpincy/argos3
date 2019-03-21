/**
 * @file <argos3/plugins/simulator/entities/magnet_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef MAGNET_ENTITY_H
#define MAGNET_ENTITY_H

namespace argos {
   class CMagnetEntity;
}

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CMagnetEntity : public CEntity {

   public:

      ENABLE_VTABLE();

   public:

      CMagnetEntity(CComposableEntity* pc_parent);

      CMagnetEntity(CComposableEntity* pc_parent,
                    const std::string& str_id,
                    const CVector3& c_passive_field,
                    const CVector3& c_active_field);

      virtual ~CMagnetEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      /**
       * Returns the field of the magnet.
       * @return the field of the magnet.
       * @see SetField()
       */
      inline const CVector3& GetField() const {
         return m_cField;
      }

      /**
       * Returns the passive field with which the magnet was initialized.
       * When the simulation is reset, the magnet field is set to this value.
       * @return the field with which the magnet was initialized.
       * @see GetField()
       */
      inline const CVector3& GetPassiveField() const {
         return m_cPassiveField;
      }

      /**
       * Returns the active field with which the magnet was initialized.
       * This value is used by an actuator, that will update the field using the
       * expression: field = passive_field + active_field * current
       * @return the active field with which the magnet was initialized.
       * @see GetField()
       */
      inline const CVector3& GetActiveField() const {
         return m_cActiveField;
      }

      /**
       * Sets the current field of the magnet.
       * @param c_field the wanted field.
       * @see GetField()
       */
      inline void SetCurrent(Real f_current) {
         m_cField = m_cPassiveField + m_cActiveField * f_current;
      }

      virtual std::string GetTypeDescription() const {
         return "magnet";
      }

   protected:

      CVector3 m_cField;
      CVector3 m_cPassiveField;
      CVector3 m_cActiveField;

   };

   /****************************************/
   /****************************************/

}

#endif
