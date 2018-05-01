/**
 * @file <argos3/plugins/robots/prototype/simulator/prototype_link_equipped_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PROTOTYPE_LINK_EQUIPPED_ENTITY_H
#define PROTOTYPE_LINK_EQUIPPED_ENTITY_H

namespace argos {
   class CPrototypeLinkEquippedEntity;
   class CPrototypeLinkEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/robots/prototype/simulator/prototype_link_entity.h>
#include <map>

namespace argos {

   class CPrototypeLinkEquippedEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CPrototypeLinkEquippedEntity(CComposableEntity* pc_parent);

      virtual ~CPrototypeLinkEquippedEntity() {}

      virtual void Init(TConfigurationNode& t_tree);
 
      CPrototypeLinkEntity& GetLink(UInt32 un_index);

      CPrototypeLinkEntity& GetLink(const std::string& str_id);

      inline CPrototypeLinkEntity::TVector& GetLinks() {
         return m_vecLinks;
      }

      CPrototypeLinkEntity& GetReferenceLink() {
         return *m_vecLinks[0];
      }

      virtual std::string GetTypeDescription() const {
         return "links";
      }

   private:

      CPrototypeLinkEntity::TVector m_vecLinks;
   };

}

#endif
