/**
 * @file <argos3/plugins/robots/prototype/simulator/prototype_joint_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "prototype_joint_entity.h"
#include <argos3/plugins/robots/prototype/simulator/prototype_link_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CPrototypeJointEntity::CPrototypeJointEntity(CComposableEntity* pc_parent) :
      CComposableEntity(pc_parent),
      m_pcParentLink(nullptr),
      m_cParentLinkJointPosition(CVector3::ZERO),
      m_cParentLinkJointOrientation(CRadians::ZERO, CVector3::Z),
      m_pcChildLink(nullptr),
      m_cChildLinkJointPosition(CVector3::ZERO),
      m_cChildLinkJointOrientation(CRadians::ZERO, CVector3::Z),
      m_bDisableCollision(true),
      m_eType(EType::FIXED),
      m_cJointAxis(CVector3::Z),
      m_bHasLimit(false) {}

   /****************************************/
   /****************************************/

   void CPrototypeJointEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CComposableEntity::Init(t_tree);
         /* Get parent and child links */
         CPrototypeLinkEquippedEntity& cLinkEquippedEntity = 
            GetParent().GetParent().GetComponent<CPrototypeLinkEquippedEntity>("links");
         /* Get the disable collisions with parent flag */
         GetNodeAttributeOrDefault(t_tree,
                                   "disable_collision",
                                   m_bDisableCollision,
                                   m_bDisableCollision);
         /* Get joint type */
         std::string strJointType;
         GetNodeAttribute(t_tree, "type", strJointType);
         if(strJointType == "fixed") {
            m_eType = EType::FIXED;
         }
         else if(strJointType == "prismatic") {
            m_eType = EType::PRISMATIC;
            GetNodeAttribute(t_tree, "axis", m_cJointAxis);
            if(NodeAttributeExists(t_tree, "limit")) {
               m_bHasLimit = true;
               GetNodeAttribute(t_tree, "limit", m_uLimit.Prismatic);
            }
         }
         else if(strJointType == "revolute") {
            m_eType = EType::REVOLUTE;
            GetNodeAttribute(t_tree, "axis", m_cJointAxis);
            if(NodeAttributeExists(t_tree, "limit")) {
               m_bHasLimit = true;
               CRange<CDegrees> cJointLimit;
               GetNodeAttribute(t_tree, "limit", cJointLimit);
               m_uLimit.Revolute = CRange<CRadians>(ToRadians(cJointLimit.GetMin()),
                                                    ToRadians(cJointLimit.GetMax()));
            }
         }
         else if(strJointType == "spherical") {
            m_eType = EType::SPHERICAL;
         }
         else {
            THROW_ARGOSEXCEPTION("Joint type \"" << strJointType << "\" is not implemented");
         }
         /* parse the parent node */
         TConfigurationNode& tJointParentNode = GetNode(t_tree, "parent");
         std::string strJointParentLink;
         GetNodeAttribute(tJointParentNode, "link", strJointParentLink);
         m_pcParentLink = &(cLinkEquippedEntity.GetLink(strJointParentLink));
         GetNodeAttributeOrDefault(tJointParentNode,
                                   "position",
                                   m_cParentLinkJointPosition,
                                   m_cParentLinkJointPosition);
         GetNodeAttributeOrDefault(tJointParentNode,
                                   "orientation",
                                   m_cParentLinkJointOrientation,
                                   m_cParentLinkJointOrientation);
         /* parse the child node */
         TConfigurationNode& tJointChildNode = GetNode(t_tree, "child");
         std::string strJointChildLink;
         GetNodeAttribute(tJointChildNode, "link", strJointChildLink);
         m_pcChildLink = &(cLinkEquippedEntity.GetLink(strJointChildLink));
         GetNodeAttributeOrDefault(tJointChildNode,
                                   "position",
                                   m_cChildLinkJointPosition,
                                   m_cChildLinkJointPosition);
         GetNodeAttributeOrDefault(tJointChildNode,
                                   "orientation",
                                   m_cChildLinkJointOrientation,
                                   m_cChildLinkJointOrientation);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error while initializing joint entity", ex);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CPrototypeJointEntity);

   /****************************************/
   /****************************************/

}
