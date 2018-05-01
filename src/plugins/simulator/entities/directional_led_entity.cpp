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
      Disable();
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Parse XML */
         CPositionalEntity::Init(t_tree);
         GetNodeAttributeOrDefault(t_tree, "color", m_cInitColor, m_cInitColor);
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
      Disable();
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEntity::SetEnabled(bool b_enabled) {
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

   void CDirectionalLEDEntity::SetColor(const CColor& c_color) {
      m_cColor = c_color;
   }

   /****************************************/
   /****************************************/

   CDirectionalLEDMedium& CDirectionalLEDEntity::GetMedium() const {
      if(m_pcMedium == nullptr) {
         THROW_ARGOSEXCEPTION("directional LED entity \"" << GetContext() << 
                              GetId() << "\" has no associated medium.");
      }
      return *m_pcMedium;
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEntity::SetMedium(CDirectionalLEDMedium& c_medium) {
      if(m_pcMedium != nullptr && m_pcMedium != &c_medium)
         m_pcMedium->RemoveEntity(*this);
      m_pcMedium = &c_medium;
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDEntitySpaceHashUpdater::operator()(CAbstractSpaceHash<CDirectionalLEDEntity>& c_space_hash,
                                                          CDirectionalLEDEntity& c_element) {
      /* Calculate the position of the LED in the space hash */
      c_space_hash.SpaceToHashTable(m_nI, m_nJ, m_nK, c_element.GetPosition());
      /* Update the corresponding cell */
      c_space_hash.UpdateCell(m_nI, m_nJ, m_nK, c_element);
   }

   /****************************************/
   /****************************************/

   CDirectionalLEDEntityGridUpdater::CDirectionalLEDEntityGridUpdater(CGrid<CDirectionalLEDEntity>& c_grid) :
      m_cGrid(c_grid) {}

   /****************************************/
   /****************************************/

   bool CDirectionalLEDEntityGridUpdater::operator()(CDirectionalLEDEntity& c_entity) {
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
      /* Continue with the other entities */
      return true;
   }

   /****************************************/
   /****************************************/

   class CSpaceOperationAddCDirectionalLEDEntity : public CSpaceOperationAddEntity {
   public:
      void ApplyTo(CSpace& c_space, CDirectionalLEDEntity& c_entity) {
         /* Add entity to space - this ensures that the directional LED entity
          * gets an id before being added to the directional LED medium */
         c_space.AddEntity(c_entity);
         /* Enable the directional LED entity, if it's enabled - this ensures that
          * the entity gets added to the directional LED medium if it's enabled */
         c_entity.SetEnabled(c_entity.IsEnabled());
      }
   };

   class CSpaceOperationRemoveCDirectionalLEDEntity : public CSpaceOperationRemoveEntity {
   public:
      void ApplyTo(CSpace& c_space, CDirectionalLEDEntity& c_entity) {
         /* Disable the entity - this ensures that the entity is
          * removed from the directional LED medium */
         c_entity.Disable();
         /* Remove the directional LED entity from space */
         c_space.RemoveEntity(c_entity);
      }
   };

   REGISTER_SPACE_OPERATION(CSpaceOperationAddEntity, 
                            CSpaceOperationAddCDirectionalLEDEntity,
                            CDirectionalLEDEntity);
   REGISTER_SPACE_OPERATION(CSpaceOperationRemoveEntity,
                            CSpaceOperationRemoveCDirectionalLEDEntity,
                            CDirectionalLEDEntity);

   /****************************************/
   /****************************************/

}
