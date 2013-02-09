/**
 * @file <argos3/plugins/simulator/entities/wifi_medium_entity.h>
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

      ENABLE_VTABLE();

   public:

      CWiFiMediumEntity();
      
      CWiFiMediumEntity(const std::string& str_id);
         
      virtual std::string GetTypeDescription() const {
         return "wifi_medium";
      }

   };
}

#endif
