/**
 * @file <argos3/core/simulator/entity/wifi_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "wifi_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CWiFiEquippedEntity::CWiFiEquippedEntity(CComposableEntity* pc_parent) :
      CEntity(pc_parent) {}

   /****************************************/
   /****************************************/

   CWiFiEquippedEntity::CWiFiEquippedEntity(CComposableEntity* pc_parent,
                                            const std::string& str_id) :
      CEntity(pc_parent, str_id) {}

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CWiFiEquippedEntity);

   /****************************************/
   /****************************************/

}
