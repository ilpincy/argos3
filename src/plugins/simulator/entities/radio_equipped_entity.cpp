/**
 * @file <argos3/plugins/simulator/entities/radio_equipped_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "radio_equipped_entity.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/media/radio_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CRadioEquippedEntity::SInstance::SInstance(CRadioEntity& c_radio,
                                              SAnchor& s_anchor,
                                              const CVector3& c_offset) :
      Radio(c_radio),
      Anchor(s_anchor),
      Offset(c_offset) {}

   /****************************************/
   /****************************************/

   CRadioEquippedEntity::CRadioEquippedEntity(CComposableEntity* pc_parent) :
      CComposableEntity(pc_parent) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CRadioEquippedEntity::CRadioEquippedEntity(CComposableEntity* pc_parent,
                                              const std::string& str_id) :
      CComposableEntity(pc_parent, str_id) {
      Disable();
   }

   /****************************************/
   /****************************************/

   void CRadioEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CComposableEntity::Init(t_tree);
         /* Go through the radop entries */
         TConfigurationNodeIterator itRadio("radio");
         for(itRadio = itRadio.begin(&t_tree);
             itRadio != itRadio.end();
             ++itRadio) {
            /* Initialise the radio using the XML */
            CRadioEntity* pcRadio = new CRadioEntity(this);
            pcRadio->Init(*itRadio);
            CVector3 cOffset;
            GetNodeAttribute(*itRadio, "position", cOffset);
            /* Parse and look up the anchor */
            std::string strAnchorId;
            GetNodeAttribute(*itRadio, "anchor", strAnchorId);
            /*
             * NOTE: here we get a reference to the embodied entity
             * This line works under the assumption that:
             * 1. the RadioEquippedEntity has a parent;
             * 2. the parent has a child whose id is "body"
             * 3. the "body" is an embodied entity
             * If any of the above is false, this line will bomb out.
             */
            CEmbodiedEntity& cBody = GetParent().GetComponent<CEmbodiedEntity>("body");
            /* Add the radio to this container */
            m_vecInstances.emplace_back(*pcRadio, cBody.GetAnchor(strAnchorId), cOffset);
            AddComponent(*pcRadio);
         }
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize radio equipped entity \"" <<
                                     GetContext() + GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CRadioEquippedEntity::Enable() {
      /* Perform generic enable behavior */
      CComposableEntity::Enable();
      /* Enable anchors */
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.Anchor.Enable();
      }
   }

   /****************************************/
   /****************************************/

   void CRadioEquippedEntity::Disable() {
      /* Perform generic disable behavior */
      CComposableEntity::Disable();
      /* Disable anchors */
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.Anchor.Disable();
      }
   }

   /****************************************/
   /****************************************/

   void CRadioEquippedEntity::AddRadio(const std::string& str_id,
                                       const CVector3& c_offset,
                                       SAnchor& s_anchor,
                                       Real f_transmit_range) {
      /* create the new radio entity */
      CRadioEntity* pcRadio =
         new CRadioEntity(this,
                          str_id,
                          f_transmit_range);
      /* add it to the instances vector */
      m_vecInstances.emplace_back(*pcRadio,
                                  s_anchor,
                                  c_offset);
      /* inform the base class about the new entity */
      AddComponent(*pcRadio);
      UpdateComponents();
   }


   /****************************************/
   /****************************************/

   CRadioEntity& CRadioEquippedEntity::GetRadio(UInt32 un_index) {
      ARGOS_ASSERT(un_index < m_vecInstances.size(),
                   "CRadioEquippedEntity::GetRadio(), id=\"" <<
                   GetContext() + GetId() <<
                   "\": index out of bounds: un_index = " <<
                   un_index <<
                   ", m_vecInstances.size() = " <<
                   m_vecInstances.size());
      return m_vecInstances[un_index].Radio;
   }

   /****************************************/
   /****************************************/

   void CRadioEquippedEntity::UpdateComponents() {
      CVector3 cPosition;
      for(SInstance& s_instance : m_vecInstances) {
         if(s_instance.Radio.IsEnabled()) {
            cPosition = s_instance.Offset;
            cPosition.Rotate(s_instance.Anchor.Orientation);
            cPosition += s_instance.Anchor.Position;
            s_instance.Radio.SetPosition(cPosition);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CRadioEquippedEntity::SetMedium(CRadioMedium& c_medium) {
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.Radio.SetMedium(c_medium);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CRadioEquippedEntity);

   /****************************************/
   /****************************************/

}
