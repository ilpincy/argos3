/**
 * @file <argos3/plugins/robots/prototype/simulator/prototype_link_equipped_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "prototype_link_equipped_entity.h"

namespace argos {

   /****************************************/
   /****************************************/

   CPrototypeLinkEquippedEntity::CPrototypeLinkEquippedEntity(CComposableEntity* pc_parent) :
      CComposableEntity(pc_parent) {}

   /****************************************/
   /****************************************/

   void CPrototypeLinkEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Initialize the parent */
         CComposableEntity::Init(t_tree);
         /* Initialize the links */
         TConfigurationNodeIterator itLink("link");
         for(itLink = itLink.begin(&t_tree);
             itLink != itLink.end();
             ++itLink) {
            CPrototypeLinkEntity* pcLinkEntity = new CPrototypeLinkEntity(this);
            pcLinkEntity->Init(*itLink);
            AddComponent(*pcLinkEntity);            
            m_vecLinks.push_back(pcLinkEntity);
         }
         /* Get the reference link */
         std::string strReferenceLink;
         GetNodeAttribute(t_tree, "ref", strReferenceLink);
         CPrototypeLinkEntity::TVectorIterator itReferenceLink =
            std::find_if(std::begin(m_vecLinks),
                         std::end(m_vecLinks),
                         [strReferenceLink] (const CPrototypeLinkEntity* pc_link) {
                            return (pc_link->GetId() == strReferenceLink);
                         });
         /* Move the reference link to the front of the vector */
         if(itReferenceLink == std::end(m_vecLinks)) {
            THROW_ARGOSEXCEPTION("The reference link \"" << strReferenceLink << "\" not found.");
         }
         else {
            if(itReferenceLink != std::begin(m_vecLinks)) {
               CPrototypeLinkEntity* pcReferenceLink = *itReferenceLink;
               *itReferenceLink = m_vecLinks.front();
               m_vecLinks.front() = pcReferenceLink;
            }
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize link equipped entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   CPrototypeLinkEntity& CPrototypeLinkEquippedEntity::GetLink(const std::string& str_id) {
      CPrototypeLinkEntity::TVectorIterator itLink =
         std::find_if(std::begin(m_vecLinks),
                      std::end(m_vecLinks),
                      [str_id] (const CPrototypeLinkEntity* pc_link) {
                         return (pc_link->GetId() == str_id);
                      });
      ARGOS_ASSERT(itLink != std::end(m_vecLinks),
                   "CPrototypeLinkEquippedEntity::GetLink(), id=\"" <<
                   GetId() <<
                   "\": link not found: str_id = " <<
                   str_id);
      return **itLink;
   }

   /****************************************/
   /****************************************/

   CPrototypeLinkEntity& CPrototypeLinkEquippedEntity::GetLink(UInt32 un_index) {
      ARGOS_ASSERT(un_index < m_vecLinks.size(),
                   "CPrototypeLinkEquippedEntity::GetLink(), id=\"" <<
                   GetId() <<
                   "\": index out of bounds: un_index = " <<
                   un_index <<
                   ", m_vecLinks.size() = " <<
                   m_vecLinks.size());
      return *m_vecLinks[un_index];
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CPrototypeLinkEquippedEntity);

   /****************************************/
   /****************************************/

}
