/**
 * @file <argos3/core/simulator/entity/led_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "led_entity.h"

namespace argos {

   /****************************************/
   /****************************************/

   CLEDEntity::CLEDEntity(CComposableEntity* pc_parent) :
      CPositionalEntity(pc_parent) {}

   /****************************************/
   /****************************************/

   CLEDEntity::CLEDEntity(CComposableEntity* pc_parent,
                          const std::string& str_id,
                          const CVector3& c_position,
                          const CColor& c_color) :
      CPositionalEntity(pc_parent, str_id, c_position, CQuaternion()),
      m_cColor(c_color),
      m_cInitColor(c_color) {}

   /****************************************/
   /****************************************/

   void CLEDEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Parse XML */
         CPositionalEntity::Init(t_tree);
         GetNodeAttribute(t_tree, "color", m_cInitColor);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error while initializing led entity", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEntity::Reset() {
      CPositionalEntity::Reset();
      m_cColor = m_cInitColor;
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

}
