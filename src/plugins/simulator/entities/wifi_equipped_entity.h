/**
 * @file <argos3/plugins/simulator/entities/wifi_equipped_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef WiFi_EQUIPPED_ENTITY_H
#define WiFi_EQUIPPED_ENTITY_H

#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CWiFiEquippedEntity : public CEntity {

   public:

      ENABLE_VTABLE();

      CWiFiEquippedEntity(CComposableEntity* pc_parent);

      CWiFiEquippedEntity(CComposableEntity* pc_parent,
                          const std::string& str_id);

      virtual std::string GetTypeDescription() const {
         return "wifi";
      }

   };

}

#endif
