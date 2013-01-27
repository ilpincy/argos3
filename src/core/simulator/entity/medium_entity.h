/**
 * @file <argos3/core/simulator/entity/medium_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef MEDIUM_ENTITY_H
#define MEDIUM_ENTITY_H

namespace argos {
   class CMediumEntity;
}

#include <argos3/core/simulator/entity/entity.h>

namespace argos {
   class CMediumEntity : public CEntity {

   public:

      ENABLE_VTABLE();

      typedef std::vector<CMediumEntity*> TVector;
      typedef std::map<std::string, CMediumEntity*> TMap;

   public:

      CMediumEntity();

      CMediumEntity(const std::string& str_id);

      virtual std::string GetTypeDescription() const {
         return "medium_entity";
      }

   };

}

#endif
