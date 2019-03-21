/**
 * @file <argos3/plugins/simulator/entities/magnet_equipped_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "magnet_equipped_entity.h"

namespace argos {

   /****************************************/
   /****************************************/

   CMagnetEquippedEntity::SInstance::SInstance(CMagnetEntity& c_magnet,
                                               SAnchor& s_anchor,
                                               const CVector3& c_offset) :
      Magnet(c_magnet),
      Anchor(s_anchor),
      Offset(c_offset) {}

   /****************************************/
   /****************************************/

   CMagnetEquippedEntity::CMagnetEquippedEntity(CComposableEntity* pc_parent) :
      CComposableEntity(pc_parent) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CMagnetEquippedEntity::CMagnetEquippedEntity(CComposableEntity* pc_parent,
                                                const std::string& str_id) :
      CComposableEntity(pc_parent, str_id) {
      Disable();
   }

   /****************************************/
   /****************************************/

   void CMagnetEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CComposableEntity::Init(t_tree);
         /* Go through the magnet entries */
         TConfigurationNodeIterator itMagnet("magnet");
         for(itMagnet = itMagnet.begin(&t_tree);
             itMagnet != itMagnet.end();
             ++itMagnet) {
            /* Initialise the Magnet using the XML */
            CMagnetEntity* pcMagnet = new CMagnetEntity(this);
            pcMagnet->Init(*itMagnet);
            CVector3 cOffset;
            GetNodeAttribute(*itMagnet, "offset", cOffset);
            /* Parse and look up the anchor */
            std::string strAnchorId;
            GetNodeAttribute(*itMagnet, "anchor", strAnchorId);
            /*
             * NOTE: here we get a reference to the embodied entity
             * This line works under the assumption that:
             * 1. the MagnetEquippedEntity has a parent;
             * 2. the parent has a child whose id is "body"
             * 3. the "body" is an embodied entity
             * If any of the above is false, this line will bomb out.
             */
            CEmbodiedEntity& cBody = GetParent().GetComponent<CEmbodiedEntity>("body");
            /* Add the magnet to this container */
            m_vecInstances.emplace_back(*pcMagnet,
                                        cBody.GetAnchor(strAnchorId),
                                        cOffset);
            AddComponent(*pcMagnet);
         }
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize magnet equipped entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CMagnetEquippedEntity::Enable() {
      CComposableEntity::Enable();
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.Magnet.Enable();
      }
   }

   /****************************************/
   /****************************************/

   void CMagnetEquippedEntity::Disable() {
      CComposableEntity::Disable();
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.Magnet.Disable();
      }
   }

   /****************************************/
   /****************************************/

   CMagnetEntity& CMagnetEquippedEntity::GetMagnet(UInt32 un_index) {
      ARGOS_ASSERT(un_index < m_vecInstances.size(),
                   "CMagnetEquippedEntity::GetMagnet(), id=\"" <<
                   GetId() <<
                   "\": index out of bounds: un_index = " <<
                   un_index <<
                   ", m_vecInstances.size() = " <<
                   m_vecInstances.size());
      return m_vecInstances[un_index].Magnet;
   }

   /****************************************/
   /****************************************/

   CMagnetEntity& CMagnetEquippedEntity::AddMagnet(const std::string& str_id,
                                                   SAnchor& s_anchor,
                                                   const CVector3& c_position_offset,
                                                   const CVector3& c_passive_field,
                                                   const CVector3& c_active_field) {
      CMagnetEntity* pcMagnet = new CMagnetEntity(this, str_id, c_passive_field, c_active_field);
      m_vecInstances.emplace_back(*pcMagnet, s_anchor, c_position_offset);
      AddComponent(*pcMagnet);
      return *pcMagnet;
   }
   
   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CMagnetEquippedEntity);

   /****************************************/
   /****************************************/

}
