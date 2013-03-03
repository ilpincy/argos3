
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
      m_bEnabled(true),
      m_bCanBeEnabledIfDisabled(true) {
   }

   /****************************************/
   /****************************************/

   CEntity::CEntity(CComposableEntity* pc_parent,
                    const std::string& str_id) :
      m_pcParent(pc_parent),
      m_strId(str_id),
      m_bEnabled(true),
      m_bCanBeEnabledIfDisabled(true) {
   }

   /****************************************/
   /****************************************/

   void CEntity::Init(TConfigurationNode& t_tree) {
      /*
       * Set an ID if it has not been set yet
       * In this way, entities that are part of a composable can have an ID set by the parent
       */
      if(m_strId == "") {
         if(! HasParent()) {
            /*
             * Root-level entity
             */
            try {
               /* Get the id of the entity */
               GetNodeAttribute(t_tree, "id", m_strId);
            }
            catch(CARGoSException& ex) {
               THROW_ARGOSEXCEPTION_NESTED("Failed to initialize an entity.", ex);
            }
         }
         else {
            /*
             * Part of a component
             */
            m_strId = GetParent().GetId() + "." + GetTypeDescription();
         }
      }
   }

   /****************************************/
   /****************************************/

   void CEntity::SetEnabled(bool b_enabled) {
      /* No need to set the state if the new one is the same as the old one */
      if(m_bEnabled != b_enabled) {
         /* The wanted state if different from the current one */
         if(! b_enabled) {
            /* We can always disable an entity */
            m_bEnabled = false;
         }
         else if(m_bCanBeEnabledIfDisabled) {
            /* We can enable the entity */
            m_bEnabled = true;
         }
         /* The 'else' branch is not necessary,
            it corresponds to not enabling the entity when we can't */
      }
   }

   /****************************************/
   /****************************************/

   INIT_VTABLE_FOR(CEntity);

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CEntity);

   /****************************************/
   /****************************************/

}
