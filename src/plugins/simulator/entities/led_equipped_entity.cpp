/**
 * @file <argos3/plugins/simulator/entities/led_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "led_equipped_entity.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/media/led_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CLEDEquippedEntity::SActuator::SActuator(CLEDEntity& c_led,
                                            const CVector3& c_offset,
                                            SAnchor& s_anchor) :
      LED(c_led),
      Offset(c_offset),
      Anchor(s_anchor) {}

   /****************************************/
   /****************************************/

   CLEDEquippedEntity::CLEDEquippedEntity(CComposableEntity* pc_parent) :
      CComposableEntity(pc_parent) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CLEDEquippedEntity::CLEDEquippedEntity(CComposableEntity* pc_parent,
                                          const std::string& str_id) :
      CComposableEntity(pc_parent, str_id) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CLEDEquippedEntity::~CLEDEquippedEntity() {
      while(! m_tLEDs.empty()) {
         delete m_tLEDs.back();
         m_tLEDs.pop_back();
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CComposableEntity::Init(t_tree);
         /* Go through the led entries */
         CVector3 cPosition;
         CColor cColor;
         TConfigurationNodeIterator itLED("led");
         for(itLED = itLED.begin(&t_tree);
             itLED != itLED.end();
             ++itLED) {
            /* Initialise the LED using the XML */
            CLEDEntity* pcLED = new CLEDEntity(this);
            pcLED->Init(*itLED);
            /* Parse the offset */
            CVector3 cOffset;
            GetNodeAttribute(*itLED, "offset", cOffset);
            /* Parse and look up the anchor */
            std::string strAnchorId;
            GetNodeAttribute(*itLED, "anchor", strAnchorId);
            /*
             * NOTE: here we get a reference to the embodied entity
             * This line works under the assumption that:
             * 1. the LEDEquippedEntity has a parent;
             * 2. the parent has a child whose id is "body"
             * 3. the "body" is an embodied entity
             * If any of the above is false, this line will bomb out.
             */
            CEmbodiedEntity& cBody = GetParent().GetComponent<CEmbodiedEntity>("body");
            /* Add the LED to this container */
            m_tLEDs.push_back(new SActuator(*pcLED, cOffset, cBody.GetAnchor(strAnchorId)));
            AddComponent(*pcLED);
         }
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize LED equipped entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::Reset() {
      for(SActuator::TList::iterator it = m_tLEDs.begin();
          it != m_tLEDs.end();
          ++it) {
         (*it)->LED.Reset();
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::Enable() {
      CEntity::Enable();
      for(size_t i = 0; i < m_tLEDs.size(); ++i) {
         m_tLEDs[i]->Anchor.Enable();
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::Disable() {
      CEntity::Disable();
      for(size_t i = 0; i < m_tLEDs.size(); ++i) {
         m_tLEDs[i]->Anchor.Disable();
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::AddLED(const CVector3& c_offset,
                                   SAnchor& s_anchor,
                                   const CColor& c_color) {
      CLEDEntity* pcLED =
         new CLEDEntity(
            this,
            std::string("led_") + ToString(m_tLEDs.size()),
            c_offset,
            c_color);
      m_tLEDs.push_back(new SActuator(*pcLED, c_offset, s_anchor));
      AddComponent(*pcLED);
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::AddLEDRing(const CVector3& c_center,
                                       Real f_radius,
                                       const CRadians& c_start_angle,
                                       UInt32 un_num_leds,
                                       SAnchor& s_anchor,
                                       const CColor& c_color) {
      CRadians cLEDSpacing = CRadians::TWO_PI / un_num_leds;
      CRadians cAngle;
      CVector3 cOffset;
      for(UInt32 i = 0; i < un_num_leds; ++i) {
         cAngle = c_start_angle + i * cLEDSpacing;
         cAngle.SignedNormalize();
         cOffset.Set(f_radius, 0.0f, 0.0f);
         cOffset.RotateZ(cAngle);
         cOffset += c_center;
         AddLED(cOffset, s_anchor, c_color);
      }
   }

   /****************************************/
   /****************************************/

   CLEDEntity& CLEDEquippedEntity::GetLED(UInt32 un_index) {
      ARGOS_ASSERT(un_index < m_tLEDs.size(),
                   "CLEDEquippedEntity::GetLED(), id=\"" <<
                   GetId() <<
                   "\": index out of bounds: un_index = " <<
                   un_index <<
                   ", m_tLEDs.size() = " <<
                   m_tLEDs.size());
      return m_tLEDs[un_index]->LED;
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::SetLEDOffset(UInt32 un_index,
                                         const CVector3& c_offset) {
      ARGOS_ASSERT(un_index < m_tLEDs.size(),
                   "CLEDEquippedEntity::SetLEDPosition(), id=\"" <<
                   GetId() <<
                   "\": index out of bounds: un_index = " <<
                   un_index <<
                   ", m_tLEDs.size() = " <<
                   m_tLEDs.size());
      m_tLEDs[un_index]->Offset = c_offset;
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::SetLEDColor(UInt32 un_index,
                                        const CColor& c_color) {
      ARGOS_ASSERT(un_index < m_tLEDs.size(),
                   "CLEDEquippedEntity::SetLEDColor(), id=\"" <<
                   GetId() <<
                   "\": index out of bounds: un_index = " <<
                   un_index <<
                   ", m_tLEDs.size() = " <<
                   m_tLEDs.size());
      m_tLEDs[un_index]->LED.SetColor(c_color);
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::SetAllLEDsColors(const CColor& c_color) {
      for(UInt32 i = 0; i < m_tLEDs.size(); ++i) {
         m_tLEDs[i]->LED.SetColor(c_color);
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::SetAllLEDsColors(const std::vector<CColor>& vec_colors) {
      if(vec_colors.size() == m_tLEDs.size()) {
         for(UInt32 i = 0; i < vec_colors.size(); ++i) {
            m_tLEDs[i]->LED.SetColor(vec_colors[i]);
         }
      }
      else {
         THROW_ARGOSEXCEPTION(
            "CLEDEquippedEntity::SetAllLEDsColors(), id=\"" <<
            GetId() <<
            "\": number of LEDs (" <<
            m_tLEDs.size() <<
            ") is lower than the passed color vector size (" <<
            vec_colors.size() <<
            ")");
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::UpdateComponents() {
      /* LED position wrt global reference frame */
      CVector3 cLEDPosition;
      for(UInt32 i = 0; i < m_tLEDs.size(); ++i) {
         if(m_tLEDs[i]->LED.IsEnabled()) {
            cLEDPosition = m_tLEDs[i]->Offset;
            cLEDPosition.Rotate(m_tLEDs[i]->Anchor.Orientation);
            cLEDPosition += m_tLEDs[i]->Anchor.Position;
            m_tLEDs[i]->LED.SetPosition(cLEDPosition);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::AddToMedium(CLEDMedium& c_medium) {
      for(UInt32 i = 0; i < m_tLEDs.size(); ++i) {
         m_tLEDs[i]->LED.AddToMedium(c_medium);
      }
      Enable();
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::RemoveFromMedium() {
      for(UInt32 i = 0; i < m_tLEDs.size(); ++i) {
         m_tLEDs[i]->LED.RemoveFromMedium();
      }
      Disable();
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CLEDEquippedEntity);

   /****************************************/
   /****************************************/

}
