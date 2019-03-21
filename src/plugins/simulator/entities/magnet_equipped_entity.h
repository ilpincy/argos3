/**
 * @file <argos3/plugins/simulator/entities/magnet_equipped_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef MAGNET_EQUIPPED_ENTITY_H
#define MAGNET_EQUIPPED_ENTITY_H

namespace argos {
   class CMagnetEquippedEntity;
   class CMagnetEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/magnet_entity.h>
#include <vector>

namespace argos {

   /**
    * A container of CMagnetEntity.
    * <p>
    * This is a convenience class that acts a container of CMagnetEntity objects. It
    * is mostly useful when a robot is equipped with a number of magnets, and you
    * want to manage them comfortably.
    * </p>
    * @see CMagnetEntity
    */
   class CMagnetEquippedEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      struct SInstance {
         CMagnetEntity& Magnet;
         SAnchor& Anchor;
         CVector3 Offset;
         SInstance(CMagnetEntity& c_magnet,
                   SAnchor& s_anchor,
                   const CVector3& c_position_offset);
         using TVector = std::vector<SInstance>;
      };

   public:

      /**
       * Class constructor.
       * @param pc_parent The parent entity.
       */
      CMagnetEquippedEntity(CComposableEntity* pc_parent);

      /**
       * Class constructor.
       * @param pc_parent The parent entity.
       * @param str_id The id of this entity.
       */
      CMagnetEquippedEntity(CComposableEntity* pc_parent,
                            const std::string& str_id);

      virtual ~CMagnetEquippedEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Enable();

      virtual void Disable();

      /**
       * Returns an magnet by numeric index.
       * @param un_index The index of the wanted magnet.
       * @return A magnet by numeric index.
       * @see GetAllMagnets()
       */
      CMagnetEntity& GetMagnet(UInt32 un_index);

      CMagnetEntity& AddMagnet(const std::string& str_id, 
                               SAnchor& s_anchor,
                               const CVector3& c_position_offset,
                               const CVector3& c_passive_field,
                               const CVector3& c_active_field = CVector3());

      SInstance::TVector& GetInstances() {
         return m_vecInstances;
      }

      virtual std::string GetTypeDescription() const {
         return "magnets";
      }

   protected:

      virtual void UpdateComponents() {}

   protected:

      /** A list of the magnets managed by this entity */
      SInstance::TVector m_vecInstances;
   };

}

#endif
