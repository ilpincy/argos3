/**
 * @file <argos3/core/simulator/entity/wifi_medium_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef WIFI_MEDIUM_ENTITY_H
#define WIFI_MEDIUM_ENTITY_H

namespace argos {
   class CWiFiMediumEntity;
}

#include <argos3/core/simulator/entity/medium_entity.h>

namespace argos {
   class CWiFiMediumEntity : public CMediumEntity {

   public:

      CWiFiMediumEntity(CEntity* pc_parent) :
         CMediumEntity(pc_parent) {}
      virtual ~CWiFiMediumEntity() {}

      virtual void Update() {}

      inline virtual std::string GetTypeDescription() const {
         return "wifi_medium_entity";
      }

   };
}

#endif
