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
      Disable();
   }

   /****************************************/
   /****************************************/

   void CLEDEntity::SetEnabled(bool b_enabled) {
      /* Perform generic enable behavior */
      CEntity::SetEnabled(b_enabled);
      if(b_enabled) {
         /* Enable entity in medium */
         if(m_pcMedium && GetIndex() >= 0)
            m_pcMedium->AddEntity(*this);
      }
      else {
         /* Disable entity in medium */
         if(m_pcMedium)
            m_pcMedium->RemoveEntity(*this);
      }
   }

   /****************************************/
   /****************************************/

   void CLEDEntity::SetColor(const CColor& c_color) {
      m_cColor = c_color;
   }

   /****************************************/
   /****************************************/

   CLEDMedium& CLEDEntity::GetMedium() const {
      if(m_pcMedium == NULL) {
         THROW_ARGOSEXCEPTION("LED entity \"" << GetContext() << GetId() << "\" has no medium associated.");
      }
      return *m_pcMedium;
   }

   /****************************************/
   /****************************************/

   void CLEDEntity::SetMedium(CLEDMedium& c_medium) {
      if(m_pcMedium != NULL && m_pcMedium != &c_medium)
         m_pcMedium->RemoveEntity(*this);
      m_pcMedium = &c_medium;
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
      /* Discard disabled and switched off LEDs */
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

   class CSpaceOperationAddCLEDEntity : public CSpaceOperationAddEntity {
   public:
      void ApplyTo(CSpace& c_space, CLEDEntity& c_entity) {
         /* Add entity to space - this ensures that the LED entity
          * gets an id before being added to the LED medium */
         c_space.AddEntity(c_entity);
         /* Enable the LED entity, if it's enabled - this ensures that
          * the entity gets added to the LED if it's enabled */
         c_entity.SetEnabled(c_entity.IsEnabled());
      }
   };

   class CSpaceOperationRemoveCLEDEntity : public CSpaceOperationRemoveEntity {
   public:
      void ApplyTo(CSpace& c_space, CLEDEntity& c_entity) {
         /* Disable the entity - this ensures that the entity is
          * removed from the LED medium */
         c_entity.Disable();
         /* Remove the LED entity from space */
         c_space.RemoveEntity(c_entity);
      }
   };

   REGISTER_SPACE_OPERATION(CSpaceOperationAddEntity, CSpaceOperationAddCLEDEntity, CLEDEntity);
   REGISTER_SPACE_OPERATION(CSpaceOperationRemoveEntity, CSpaceOperationRemoveCLEDEntity, CLEDEntity);

   /****************************************/
   /****************************************/

}
