/**
 * @file <argos3/plugins/simulator/entities/directional_led_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "directional_led_entity.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/media/directional_led_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDirectionalLEDEntity::CDirectionalLEDEntity(CComposableEntity* pc_parent) :
      CPositionalEntity(pc_parent),
      m_cObservableAngle(CRadians::PI),
      m_cColor(CColor::BLACK),
      m_cInitColor(CColor::BLACK),
      m_pcMedium(nullptr) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CDirectionalLEDEntity::CDirectionalLEDEntity(CComposableEntity* pc_parent,
                          const std::string& str_id,
                          const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          const CRadians& c_observable_angle,
                          const CColor& c_color) :
      CPositionalEntity(pc_parent, str_id, c_position, c_orientation),
      m_cObservableAngle(c_observable_angle),
      m_cColor(c_color),
      m_cInitColor(c_color),
      m_pcMedium(nullptr) {
      SetColor(c_color);
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Parse XML */
         CPositionalEntity::Init(t_tree);
         GetNodeAttribute(t_tree, "color", m_cInitColor);
         m_cColor = m_cInitColor;
         CDegrees cObservableAngle;
         GetNodeAttribute(t_tree, "observable_angle", cObservableAngle);
         m_cObservableAngle = ToRadians(cObservableAngle);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error while initializing directional led entity", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEntity::Reset() {
      m_cColor = m_cInitColor;
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEntity::Destroy() {
      if(HasMedium()) {
         RemoveFromMedium();
      }
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEntity::SetEnabled(bool b_enabled) {
      CEntity::SetEnabled(m_cColor != CColor::BLACK);
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEntity::SetColor(const CColor& c_color) {
      m_cColor = c_color;
      SetEnabled(c_color != CColor::BLACK);
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEntity::AddToMedium(CDirectionalLEDMedium& c_medium) {
      if(HasMedium()) RemoveFromMedium();
      m_pcMedium = &c_medium;
      c_medium.AddEntity(*this);
      Enable();
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEntity::RemoveFromMedium() {
      try {
         GetMedium().RemoveEntity(*this);
         m_pcMedium = nullptr;
         Disable();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't remove directional LED entity \"" << GetContext() + GetId() << "\" from medium.", ex);
      }
   }

   /****************************************/
   /****************************************/
      
   CDirectionalLEDMedium& CDirectionalLEDEntity::GetMedium() const {
      if(m_pcMedium == nullptr) {
         THROW_ARGOSEXCEPTION("Directional LED entity \"" << GetContext() + GetId() << "\" has no medium associated.");
      }
      return *m_pcMedium;
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEntitySpaceHashUpdater::operator()(CAbstractSpaceHash<CDirectionalLEDEntity>& c_space_hash,
                                                          CDirectionalLEDEntity& c_element) {
      /* Discard LEDs switched off */
      if(c_element.GetColor() != CColor::BLACK) {
         /* Calculate the position of the LED in the space hash */
         c_space_hash.SpaceToHashTable(m_nI, m_nJ, m_nK, c_element.GetPosition());
         /* Update the corresponding cell */
         c_space_hash.UpdateCell(m_nI, m_nJ, m_nK, c_element);
      }
   }

   /****************************************/
   /****************************************/

   CDirectionalLEDEntityGridUpdater::CDirectionalLEDEntityGridUpdater(CGrid<CDirectionalLEDEntity>& c_grid) :
      m_cGrid(c_grid) {}

   /****************************************/
   /****************************************/

   bool CDirectionalLEDEntityGridUpdater::operator()(CDirectionalLEDEntity& c_entity) {
      /* Discard LEDs switched off */
      if(c_entity.GetColor() != CColor::BLACK) {
         try {
            /* Calculate the position of the LED in the space hash */
            m_cGrid.PositionToCell(m_nI, m_nJ, m_nK, c_entity.GetPosition());
            /* Update the corresponding cell */
            m_cGrid.UpdateCell(m_nI, m_nJ, m_nK, c_entity);
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("While updating the directional LED grid for LED \"" <<
                                        c_entity.GetContext() + c_entity.GetId() << "\"", ex);
         }
      }
      /* Continue with the other entities */
      return true;
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CDirectionalLEDEntity);

   /****************************************/
   /****************************************/

}
