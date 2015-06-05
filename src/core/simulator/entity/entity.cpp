
/**
 * @file <argos3/core/simulator/entity/entity.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "entity.h"
#include "composable_entity.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CEntity::CEntity(CComposableEntity* pc_parent) :
      m_pcParent(pc_parent),
      m_bEnabled(true) {
   }

   /****************************************/
   /****************************************/

   CEntity::CEntity(CComposableEntity* pc_parent,
                    const std::string& str_id) :
      m_pcParent(pc_parent),
      m_strId(str_id),
      m_bEnabled(true) {
   }

   /****************************************/
   /****************************************/

   void CEntity::Init(TConfigurationNode& t_tree) {
      try {
         /*
          * Set the id of the entity from XML or type description
          */
         /* Was an id specified explicitly? */
         if(NodeAttributeExists(t_tree, "id")) {
            /* Yes, use that */
            GetNodeAttribute(t_tree, "id", m_strId);
         }
         else {
            /* No, derive it from the parent */
            if(m_pcParent != NULL) {
               UInt32 unIdCount = 0;
               while(GetParent().HasComponent(GetTypeDescription() +
                                              "[" + GetTypeDescription() +
                                              "_" + ToString(unIdCount) +
                                              "]")) {
                  ++unIdCount;
               }
               m_strId = GetTypeDescription() + "_" + ToString(unIdCount);
            }
            else {
               THROW_ARGOSEXCEPTION("Root entities must provide the identifier tag");
            }
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize an entity.", ex);
      }
   }

   /****************************************/
   /****************************************/

   std::string CEntity::GetContext() const {
      if(m_pcParent != NULL) {
         return GetParent().GetContext() + GetParent().GetId() + ".";
      }
      else {
         return "";
      }
   }

   /****************************************/
   /****************************************/
   
   CComposableEntity& CEntity::GetParent() {
      if(m_pcParent != NULL) {
         return *m_pcParent;
      }
      else {
         THROW_ARGOSEXCEPTION("Entity \"" << GetId() << "\" has no parent");
      }
   }

   /****************************************/
   /****************************************/

   const CComposableEntity& CEntity::GetParent() const {
      if(m_pcParent != NULL) {
         return *m_pcParent;
      }
      else {
         THROW_ARGOSEXCEPTION("Entity \"" << GetId() << "\" has no parent");
      }
   }

   /****************************************/
   /****************************************/

   CEntity& CEntity::GetRootEntity() {
      if(m_pcParent != NULL) {
         return m_pcParent->GetRootEntity();
      }
      else {
         return *this;
      }
   }

   /****************************************/
   /****************************************/
   
   const CEntity& CEntity::GetRootEntity() const {
      if(m_pcParent != NULL) {
         return m_pcParent->GetRootEntity();
      }
      else {
         return *this;
      }
   }

   /****************************************/
   /****************************************/

   void CEntity::SetEnabled(bool b_enabled) {
      m_bEnabled = b_enabled;
   }

   /****************************************/
   /****************************************/

   INIT_VTABLE_FOR(CEntity);

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CEntity);

   /****************************************/
   /****************************************/

}
