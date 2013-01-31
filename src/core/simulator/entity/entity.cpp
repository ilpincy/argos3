
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
      m_pcParent(pc_parent) {
   }

   /****************************************/
   /****************************************/

   CEntity::CEntity(CComposableEntity* pc_parent,
                    const std::string& str_id) :
      m_pcParent(pc_parent),
      m_strId(str_id) {
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

   INIT_VTABLE_FOR(CEntity);

   /****************************************/
   /****************************************/

   REGISTER_SPACE_OPERATION(CSpaceOperationAddEntity,
                            CSpaceOperationAddBaseEntity,
                            CEntity);

   REGISTER_SPACE_OPERATION(CSpaceOperationRemoveEntity,
                            CSpaceOperationRemoveBaseEntity,
                            CEntity);

   /****************************************/
   /****************************************/

}
