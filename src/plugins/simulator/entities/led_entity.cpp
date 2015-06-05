/**
 * @file <argos3/core/simulator/entity/led_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "led_entity.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/media/led_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CLEDEntity::CLEDEntity(CComposableEntity* pc_parent) :
      CPositionalEntity(pc_parent),
      m_pcMedium(NULL) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CLEDEntity::CLEDEntity(CComposableEntity* pc_parent,
                          const std::string& str_id,
                          const CVector3& c_position,
                          const CColor& c_color) :
      CPositionalEntity(pc_parent, str_id, c_position, CQuaternion()),
      m_cColor(c_color),
      m_cInitColor(c_color),
      m_pcMedium(NULL) {
      SetColor(c_color);
   }

   /****************************************/
   /****************************************/

   void CLEDEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Parse XML */
         CPositionalEntity::Init(t_tree);
         GetNodeAttribute(t_tree, "color", m_cInitColor);
         m_cColor = m_cInitColor;
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error while initializing led entity", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEntity::Reset() {
      m_cColor = m_cInitColor;
   }

   /****************************************/
   /****************************************/

   void CLEDEntity::Destroy() {
      if(HasMedium()) {
         RemoveFromMedium();
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEntity::SetEnabled(bool b_enabled) {
      CEntity::SetEnabled(m_cColor != CColor::BLACK);
   }

   /****************************************/
   /****************************************/

   void CLEDEntity::SetColor(const CColor& c_color) {
      m_cColor = c_color;
      SetEnabled(c_color != CColor::BLACK);
   }

   /****************************************/
   /****************************************/

   void CLEDEntity::AddToMedium(CLEDMedium& c_medium) {
      if(HasMedium()) RemoveFromMedium();
      m_pcMedium = &c_medium;
      c_medium.AddEntity(*this);
      Enable();
   }

   /****************************************/
   /****************************************/

   void CLEDEntity::RemoveFromMedium() {
      try {
         GetMedium().RemoveEntity(*this);
         m_pcMedium = NULL;
         Disable();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't remove LED entity \"" << GetId() << "\" from medium.", ex);
      }
   }

   /****************************************/
   /****************************************/
      
   CLEDMedium& CLEDEntity::GetMedium() const {
      if(m_pcMedium == NULL) {
         THROW_ARGOSEXCEPTION("LED entity \"" << GetId() << "\" has no medium associated.");
      }
      return *m_pcMedium;
   }

   /****************************************/
   /****************************************/

   void CLEDEntitySpaceHashUpdater::operator()(CAbstractSpaceHash<CLEDEntity>& c_space_hash,
                                               CLEDEntity& c_element) {
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

   CLEDEntityGridUpdater::CLEDEntityGridUpdater(CGrid<CLEDEntity>& c_grid) :
      m_cGrid(c_grid) {}

   /****************************************/
   /****************************************/

   bool CLEDEntityGridUpdater::operator()(CLEDEntity& c_entity) {
      /* Discard LEDs switched off */
      if(c_entity.GetColor() != CColor::BLACK) {
         try {
            /* Calculate the position of the LED in the space hash */
            m_cGrid.PositionToCell(m_nI, m_nJ, m_nK, c_entity.GetPosition());
            /* Update the corresponding cell */
            m_cGrid.UpdateCell(m_nI, m_nJ, m_nK, c_entity);
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("While updating the LED grid for LED \"" << c_entity.GetContext() << c_entity.GetId() << "\"", ex);
         }
      }
      /* Continue with the other entities */
      return true;
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CLEDEntity);

   /****************************************/
   /****************************************/

}
