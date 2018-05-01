/**
 * @file <argos3/plugins/robots/prototype/simulator/prototype_joint_equipped_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "prototype_joint_equipped_entity.h"

namespace argos {

   /****************************************/
   /****************************************/

   CPrototypeJointEquippedEntity::CPrototypeJointEquippedEntity(CComposableEntity* pc_parent) :
      CComposableEntity(pc_parent) {}

   /****************************************/
   /****************************************/

   void CPrototypeJointEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CComposableEntity::Init(t_tree);
         /* Go through the joints */
         TConfigurationNodeIterator itJoint("joint");
         for(itJoint = itJoint.begin(&t_tree);
             itJoint != itJoint.end();
             ++itJoint) {
            /* for each <joint/> node */
            CPrototypeJointEntity* pcJointEntity = new CPrototypeJointEntity(this);
            pcJointEntity->Init(*itJoint);
            AddComponent(*pcJointEntity);
            m_vecJoints.push_back(pcJointEntity);
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize joint equipped entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   CPrototypeJointEntity& CPrototypeJointEquippedEntity::GetJoint(const std::string& str_id) {
      CPrototypeJointEntity::TVectorIterator itJoint =
         std::find_if(std::begin(m_vecJoints),
                      std::end(m_vecJoints),
                      [str_id] (const CPrototypeJointEntity* pc_joint) {
                         return (pc_joint->GetId() == str_id);
                      });
      ARGOS_ASSERT(itJoint != std::end(m_vecJoints),
                   "CPrototypeJointEquippedEntity::GetJoint(), id=\"" <<
                   GetId() <<
                   "\": joint not found: str_id = " <<
                   str_id);
      return **itJoint;
   }

   /****************************************/
   /****************************************/

   CPrototypeJointEntity& CPrototypeJointEquippedEntity::GetJoint(UInt32 un_index) {
      ARGOS_ASSERT(un_index < m_vecJoints.size(),
                   "CPrototypeJointEquippedEntity::GetJoint(), id=\"" <<
                   GetId() <<
                   "\": index out of bounds: un_index = " <<
                   un_index <<
                   ", m_vecJoints.size() = " <<
                   m_vecJoints.size());
      return *m_vecJoints[un_index];
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CPrototypeJointEquippedEntity);

   /****************************************/
   /****************************************/

}
