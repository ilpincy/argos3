/**
 * @file <argos3/plugins/robots/prototype/simulator/prototype_joint_equipped_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PROTOTYPE_JOINT_EQUIPPED_ENTITY_H
#define PROTOTYPE_JOINT_EQUIPPED_ENTITY_H

namespace argos {
   class CPrototypeJointEquippedEntity;
   class CPrototypeJointEntity;
}

#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/robots/prototype/simulator/prototype_joint_entity.h>
#include <map>

namespace argos {

   class CPrototypeJointEquippedEntity : public CComposableEntity {

   public:

      ENABLE_VTABLE();

   public:

      CPrototypeJointEquippedEntity(CComposableEntity* pc_parent);

      virtual ~CPrototypeJointEquippedEntity() {}

      virtual void Init(TConfigurationNode& t_tree);
      
      CPrototypeJointEntity& GetJoint(UInt32 un_index);

      CPrototypeJointEntity& GetJoint(const std::string& str_id);

      inline CPrototypeJointEntity::TVector& GetJoints() {
         return m_vecJoints;
      }

      virtual std::string GetTypeDescription() const {
         return "joints";
      }

   private:

      CPrototypeJointEntity::TVector m_vecJoints;
   };

}

#endif
