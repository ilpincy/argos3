/**
 * @file <argos3/plugins/simulator/entities/wifi_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "wifi_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CWiFiEquippedEntity::CWiFiEquippedEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CWiFiEquippedEntity::CWiFiEquippedEntity(CComposableEntity* pc_parent,
                                            const std::string& str_id) :
      CEntity(pc_parent, str_id) {
      Disable();
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CWiFiEquippedEntity);

   /****************************************/
   /****************************************/

}
