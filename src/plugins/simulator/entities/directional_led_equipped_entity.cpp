/**
 * @file <argos3/plugins/simulator/entities/directional_led_equipped_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "directional_led_equipped_entity.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/media/directional_led_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDirectionalLEDEquippedEntity::SInstance::SInstance(CDirectionalLEDEntity& c_led,
                                                       SAnchor& s_anchor,
                                                       const CVector3& c_position_offset,
                                                       const CQuaternion& c_orientation_offset) :
      LED(c_led),
      Anchor(s_anchor),
      PositionOffset(c_position_offset),
      OrientationOffset(c_orientation_offset) {}

   /****************************************/
   /****************************************/

   CDirectionalLEDEquippedEntity::CDirectionalLEDEquippedEntity(CComposableEntity* pc_parent) :
      CComposableEntity(pc_parent) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CDirectionalLEDEquippedEntity::CDirectionalLEDEquippedEntity(CComposableEntity* pc_parent,
                                                                const std::string& str_id) :
      CComposableEntity(pc_parent, str_id) {
      Disable();
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CComposableEntity::Init(t_tree);
         /* Go through the led entries */
         TConfigurationNodeIterator itLED("directional_led");
         for(itLED = itLED.begin(&t_tree);
             itLED != itLED.end();
             ++itLED) {
            /* Initialise the LED using the XML */
            CDirectionalLEDEntity* pcLED = new CDirectionalLEDEntity(this);
            pcLED->Init(*itLED);
            CVector3 cPositionOffset;
            GetNodeAttribute(*itLED, "position", cPositionOffset);
            CQuaternion cOrientationOffset;
            GetNodeAttribute(*itLED, "orientation", cOrientationOffset);
            /* Parse and look up the anchor */
            std::string strAnchorId;
            GetNodeAttribute(*itLED, "anchor", strAnchorId);
            /*
             * NOTE: here we get a reference to the embodied entity
             * This line works under the assumption that:
             * 1. the DirectionalLEDEquippedEntity has a parent;
             * 2. the parent has a child whose id is "body"
             * 3. the "body" is an embodied entity
             * If any of the above is false, this line will bomb out.
             */
            CEmbodiedEntity& cBody =
               GetParent().GetComponent<CEmbodiedEntity>("body");
            /* Add the LED to this container */
            m_vecInstances.emplace_back(*pcLED,
                                        cBody.GetAnchor(strAnchorId),
                                        cPositionOffset,
                                        cOrientationOffset);
            AddComponent(*pcLED);
         }
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize directional LED equipped entity \"" <<
                                     GetContext() + GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEquippedEntity::AddLED(const std::string& str_id,
                                              const CVector3& c_position,
                                              const CQuaternion& c_orientation,
                                              SAnchor& s_anchor,
                                              const CRadians& c_observable_angle,
                                              const CColor& c_color) {
      /* create the new directional LED entity */
      CDirectionalLEDEntity* pcLED =
         new CDirectionalLEDEntity(this,
                                   str_id,
                                   c_position,
                                   c_orientation,
                                   c_observable_angle,
                                   c_color);
      /* add it to the instances vector */
      m_vecInstances.emplace_back(*pcLED,
                                  s_anchor,
                                  c_position,
                                  c_orientation);
      /* inform the base class about the new entity */
      AddComponent(*pcLED);
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEquippedEntity::Enable() {
      /* Perform generic enable behavior */
      CComposableEntity::Enable();
      /* Enable anchors */
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.Anchor.Enable();
      }
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEquippedEntity::Disable() {
      /* Perform generic disable behavior */
      CComposableEntity::Disable();
      /* Disable anchors */
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.Anchor.Disable();
      }
   }

   /****************************************/
   /****************************************/

   CDirectionalLEDEntity& CDirectionalLEDEquippedEntity::GetLED(UInt32 un_index) {
      ARGOS_ASSERT(un_index < m_vecInstances.size(),
                   "CLEDEquippedEntity::GetLED(), id=\"" <<
                   GetContext() + GetId() <<
                   "\": index out of bounds: un_index = " <<
                   un_index <<
                   ", m_vecInstances.size() = " <<
                   m_vecInstances.size());
      return m_vecInstances[un_index].LED;
   }

   /****************************************/
   /****************************************/

   const CDirectionalLEDEntity& CDirectionalLEDEquippedEntity::GetLED(UInt32 un_index) const {
      ARGOS_ASSERT(un_index < m_vecInstances.size(),
                   "CLEDEquippedEntity::GetLED(), id=\"" <<
                   GetContext() + GetId() <<
                   "\": index out of bounds: un_index = " <<
                   un_index <<
                   ", m_vecInstances.size() = " <<
                   m_vecInstances.size());
      return m_vecInstances[un_index].LED;
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEquippedEntity::SetLEDColor(UInt32 un_index,
                                                   const CColor& c_color) {
      ARGOS_ASSERT(un_index < m_vecInstances.size(),
                   "CLEDEquippedEntity::SetLEDColor(), id=\"" <<
                   GetContext() + GetId() <<
                   "\": index out of bounds: un_index = " <<
                   un_index <<
                   ", m_vecInstances.size() = " <<
                   m_vecInstances.size());
      m_vecInstances[un_index].LED.SetColor(c_color);
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEquippedEntity::SetLEDColors(const CColor& c_color) {
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.LED.SetColor(c_color);
      }
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEquippedEntity::SetLEDColors(const std::vector<CColor>& vec_colors) {
      if(vec_colors.size() == m_vecInstances.size()) {
         for(UInt32 i = 0; i < vec_colors.size(); ++i) {
            m_vecInstances[i].LED.SetColor(vec_colors[i]);
         }
      }
      else {
         THROW_ARGOSEXCEPTION(
            "CDirectionalLEDEquippedEntity::SetLEDColors(), id=\"" <<
            GetContext() + GetId() <<
            "\": number of LEDs (" <<
            m_vecInstances.size() <<
            ") does not equal the passed color vector size (" <<
            vec_colors.size() <<
            ")");
      }
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEquippedEntity::UpdateComponents() {
      /* LED position wrt global reference frame */
      CVector3 cLEDPosition;
      CQuaternion cLEDOrientation;
      for(SInstance& s_instance : m_vecInstances) {
         if(s_instance.LED.IsEnabled()) {
            cLEDPosition = s_instance.PositionOffset;
            cLEDPosition.Rotate(s_instance.Anchor.Orientation);
            cLEDPosition += s_instance.Anchor.Position;
            cLEDOrientation = s_instance.Anchor.Orientation *
               s_instance.OrientationOffset;
            s_instance.LED.MoveTo(cLEDPosition, cLEDOrientation);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEquippedEntity::SetMedium(CDirectionalLEDMedium& c_medium) {
      for(SInstance& s_instance : m_vecInstances) {
         s_instance.LED.SetMedium(c_medium);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CDirectionalLEDEquippedEntity);

   /****************************************/
   /****************************************/

}
