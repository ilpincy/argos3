/**
 * @file <argos3/core/simulator/entity/led_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "led_entity.h"
#include <argos3/core/simulator/space/space.h>

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
         m_cColor = m_cInitColor;
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

   void CLEDEntity::SetEnabled(bool b_enabled) {
      CEntity::SetEnabled(b_enabled);
      if(IsEnabled()) {
         m_cColor = CColor::BLACK;
      }
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
         /* Calculate the position of the LED in the space hash */
         m_cGrid.PositionToCell(m_nI, m_nJ, m_nK, c_entity.GetPosition());
         /* Update the corresponding cell */
         m_cGrid.UpdateCell(m_nI, m_nJ, m_nK, c_entity);
      }
      /* Continue with the other entities */
      return true;
   }

   /****************************************/
   /****************************************/

   class CSpaceOperationAddLEDEntity : public CSpaceOperationAddEntity {
   public:
      void ApplyTo(CSpace& c_space, CLEDEntity& c_entity) {
         c_space.AddEntity(c_entity);
      }
   };
   REGISTER_SPACE_OPERATION(CSpaceOperationAddEntity, CSpaceOperationAddLEDEntity, CLEDEntity);
   
   class CSpaceOperationRemoveLEDEntity : public CSpaceOperationRemoveEntity {
   public:
      void ApplyTo(CSpace& c_space, CLEDEntity& c_entity) {
         c_space.RemoveEntity(c_entity);
      }
   };
   REGISTER_SPACE_OPERATION(CSpaceOperationRemoveEntity, CSpaceOperationRemoveLEDEntity, CLEDEntity);

   /****************************************/
   /****************************************/

}
