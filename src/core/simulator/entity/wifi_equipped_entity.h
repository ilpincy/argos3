/**
 * @file <argos3/core/simulator/space/entities/wifi_equipped_entity.h>
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

      CWiFiEquippedEntity(CEntity* pc_parent) :
         CEntity(pc_parent) {}

      virtual ~CWiFiEquippedEntity() {}

      virtual void Reset() {}

      virtual void Update() {}

      virtual std::string GetTypeDescription() const {
         return "wifi_equipped_entity";
      }

   };

}

#endif
