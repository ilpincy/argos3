/**
 * @file <argos3/plugins/simulator/entities/simple_radio_equipped_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "simple_radio_equipped_entity.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/media/simple_radio_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CSimpleRadioEquippedEntity::SInstance::SInstance(CSimpleRadioEntity& c_radio,
                                                    SAnchor& s_anchor,
                                                    const CVector3& c_offset) :
      Radio(c_radio),
      Anchor(s_anchor),
      Offset(c_offset) {}

   /****************************************/
   /****************************************/

   CSimpleRadioEquippedEntity::CSimpleRadioEquippedEntity(CComposableEntity* pc_parent) :
      CComposableEntity(pc_parent) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CSimpleRadioEquippedEntity::CSimpleRadioEquippedEntity(CComposableEntity* pc_parent,
                                                          const std::string& str_id) :
      CComposableEntity(pc_parent, str_id) {
      Disable();
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CComposableEntity::Init(t_tree);
         /* Go through the radop entries */
         TConfigurationNodeIterator itRadio("simple_radio");
         for(itRadio = itRadio.begin(&t_tree);
             itRadio != itRadio.end();
             ++itRadio) {
            /* Initialise the radio using the XML */
            auto* pcRadio = new CSimpleRadioEntity(this);
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
            auto& cBody = GetParent().GetComponent<CEmbodiedEntity>("body");
            /* Add the radio to this container */
            m_vecInstances.emplace_back(*pcRadio, cBody.GetAnchor(strAnchorId), cOffset);
            AddComponent(*pcRadio);
         }
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize simple radio equipped entity \"" <<
                                     GetContext() + GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioEquippedEntity::Enable() {
      /* Perform generic enable behavior */
      CComposableEntity::Enable();
      /* Enable anchors */
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.Anchor.Enable();
      }
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioEquippedEntity::Disable() {
      /* Perform generic disable behavior */
      CComposableEntity::Disable();
      /* Disable anchors */
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.Anchor.Disable();
      }
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioEquippedEntity::AddRadio(const std::string& str_id,
                                             const CVector3& c_offset,
                                             SAnchor& s_anchor,
                                             CSimpleRadioMedium& c_medium,
                                             Real f_transmit_range) {
      /* create the new radio entity */
      auto* pcRadio =
         new CSimpleRadioEntity(this,
                                str_id,
                                c_medium,
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

   CSimpleRadioEntity& CSimpleRadioEquippedEntity::GetRadio(UInt32 un_index) {
      ARGOS_ASSERT(un_index < m_vecInstances.size(),
                   "CSimpleRadioEquippedEntity::GetRadio(), id=\"" <<
                   GetContext() + GetId() <<
                   "\": index out of bounds: un_index = " <<
                   un_index <<
                   ", m_vecInstances.size() = " <<
                   m_vecInstances.size());
      return m_vecInstances[un_index].Radio;
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioEquippedEntity::UpdateComponents() {
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

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CSimpleRadioEquippedEntity);

   /****************************************/
   /****************************************/

}
