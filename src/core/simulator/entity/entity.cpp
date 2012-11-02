#include "entity.h"

namespace argos {

   /****************************************/
   /****************************************/

   void CEntity::Init(TConfigurationNode& t_tree)
   {
      try {
         /* Get the id of the entity */
         GetNodeAttribute(t_tree, "id", m_strId);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize an entity.", ex);
      }
   }

   /****************************************/
   /****************************************/

}
