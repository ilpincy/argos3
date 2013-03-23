/**
 * @file <argos3/plugins/simulator/entities/led_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "led_equipped_entity.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   CLEDEquippedEntity::CLEDEquippedEntity(CComposableEntity* pc_parent,
                                          CPositionalEntity* pc_reference) :
      CComposableEntity(pc_parent),
      m_pcReferenceEntity(pc_reference) {
      Disable();
      SetCanBeEnabledIfDisabled(false);
   }

   /****************************************/
   /****************************************/

   CLEDEquippedEntity::CLEDEquippedEntity(CComposableEntity* pc_parent,
                                          const std::string& str_id,
                                          CPositionalEntity* pc_reference) :
      CComposableEntity(pc_parent, str_id),
      m_pcReferenceEntity(pc_reference) {
      Disable();
      SetCanBeEnabledIfDisabled(false);
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
            GetNodeAttribute(*itLED, "position", cPosition);
            GetNodeAttribute(*itLED, "color", cColor);
            m_vecLEDOffsetPositions.push_back(cPosition);
            AddLED(cPosition, cColor);
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize LED equipped entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::Reset() {
      for(CLEDEntity::TList::iterator it = m_tLEDs.begin();
          it != m_tLEDs.end();
          ++it) {
         (*it)->Reset();
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::AddLED(const CVector3& c_position,
                                   const CColor& c_color) {
      CLEDEntity* pcLED =
         new CLEDEntity(
            this,
            GetId() + ".led[" + ToString(m_tLEDs.size()) + "]",
            c_position,
            c_color);
      m_tLEDs.push_back(pcLED);
      AddComponent(*pcLED);
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::AddLEDRing(const CVector3& c_center,
                                       Real f_radius,
                                       const CRadians& c_start_angle,
                                       UInt32 un_num_leds,
                                       const CColor& c_color) {
      CRadians cLEDSpacing = CRadians::TWO_PI / un_num_leds;
      CRadians cAngle;
      CVector3 cPos;
      for(UInt32 i = 0; i < un_num_leds; ++i) {
         cAngle = c_start_angle + i * cLEDSpacing;
         cAngle.SignedNormalize();
         cPos.Set(f_radius, 0.0f, 0.0f);
         cPos.RotateZ(cAngle);
         cPos += c_center;
         AddLED(cPos, c_color);
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
      return *m_tLEDs[un_index];
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::SetLEDPosition(UInt32 un_index,
                                           const CVector3& c_position) {
      ARGOS_ASSERT(un_index < m_tLEDs.size(),
                   "CLEDEquippedEntity::SetLEDPosition(), id=\"" <<
                   GetId() <<
                   "\": index out of bounds: un_index = " <<
                   un_index <<
                   ", m_tLEDs.size() = " <<
                   m_tLEDs.size());
      m_tLEDs[un_index]->SetPosition(c_position);
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
      m_tLEDs[un_index]->SetColor(c_color);
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::SetAllLEDsColors(const CColor& c_color) {
      for(UInt32 i = 0; i < m_tLEDs.size(); ++i) {
         m_tLEDs[i]->SetColor(c_color);
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::SetAllLEDsColors(const std::vector<CColor>& vec_colors) {
      ARGOS_ASSERT(vec_colors.size() <= m_tLEDs.size(),
                   "CLEDEquippedEntity::SetAllLEDsColors(), id=\"" <<
                   GetId() <<
                   "\": number of LEDs (" <<
                   m_tLEDs.size() <<
                   ") is lower than the passed color vector size (" <<
                   vec_colors.size() <<
                   ")");
      for(UInt32 i = 0; i < vec_colors.size(); ++i) {
         m_tLEDs[i]->SetColor(vec_colors[i]);
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEquippedEntity::UpdateComponents() {
      if(HasReferenceEntity()) {
         /* Set LED position wrt reference */
         CVector3 cLEDPosition;
         for(UInt32 i = 0; i < m_tLEDs.size(); ++i) {
            cLEDPosition = m_vecLEDOffsetPositions[i];
            cLEDPosition.Rotate(m_pcReferenceEntity->GetOrientation());
            cLEDPosition += m_pcReferenceEntity->GetPosition();
            SetLEDPosition(i, cLEDPosition);
         }
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CLEDEquippedEntity);

   /****************************************/
   /****************************************/

}
