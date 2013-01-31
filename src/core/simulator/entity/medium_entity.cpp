/**
 * @file <argos3/core/simulator/entity/medium_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "medium_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CMediumEntity::CMediumEntity() :
      CEntity(NULL) {}

   /****************************************/
   /****************************************/

   CMediumEntity::CMediumEntity(const std::string& str_id) :
      CEntity(NULL, str_id) {}

   /****************************************/
   /****************************************/

   class CSpaceOperationAddMediumEntity : public CSpaceOperationAddEntity {
   public:
      void ApplyTo(CSpace& c_space, CMediumEntity& c_entity) {
         LOGERR << "[DEBUG] CSpaceOperationAddMediumEntity on " << c_entity.GetId() << std::endl;
         LOGERR.Flush();
         c_space.AddEntity(c_entity);
         c_space.AddMediumEntity(c_entity);
      }
   };
   REGISTER_SPACE_OPERATION(CSpaceOperationAddEntity,
                            CSpaceOperationAddMediumEntity,
                            CMediumEntity);

   class CSpaceOperationRemoveMediumEntity : public CSpaceOperationRemoveEntity {
   public:
      void ApplyTo(CSpace& c_space, CMediumEntity& c_entity) {
         LOGERR << "[DEBUG] CSpaceOperationRemoveMediumEntity on " << c_entity.GetId() << std::endl;
         LOGERR.Flush();
         c_space.RemoveMediumEntity(c_entity);
         c_space.RemoveEntity(c_entity);
      }
   };
   REGISTER_SPACE_OPERATION(CSpaceOperationRemoveEntity,
                            CSpaceOperationRemoveMediumEntity,
                            CMediumEntity);

   /****************************************/
   /****************************************/

}
