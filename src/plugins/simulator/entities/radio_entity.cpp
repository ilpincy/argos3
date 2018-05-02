/**
 * @file <argos3/plugins/simulator/entities/radio_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "radio_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/simulator/media/radio_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CRadioEntity::CRadioEntity(CComposableEntity* pc_parent) :
      CPositionalEntity(pc_parent),
      m_fRange(0.0f),
      m_pcMedium(nullptr) {}

   /****************************************/
   /****************************************/

   CRadioEntity::CRadioEntity(CComposableEntity* pc_parent,
                              const std::string& str_id,
                              Real f_range) :
      CPositionalEntity(pc_parent, str_id, CVector3(), CQuaternion()),
      m_fRange(f_range),
      m_pcMedium(nullptr) {}

   /****************************************/
   /****************************************/

   void CRadioEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Parse XML */
         CPositionalEntity::Init(t_tree);
         GetNodeAttribute(t_tree, "range", m_fRange);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error while initializing radio entity", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CRadioEntity::Reset() {
      /* Erase received messages */
      m_vecData.clear();
   }

   /****************************************/
   /****************************************/

   void CRadioEntity::Destroy() {
      Disable();
   }

   /****************************************/
   /****************************************/

   void CRadioEntity::SetEnabled(bool b_enabled) {
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

   CRadioMedium& CRadioEntity::GetMedium() const {
      if(m_pcMedium == nullptr) {
         THROW_ARGOSEXCEPTION("radio entity \"" << GetContext() << GetId() <<
                              "\" has no associated medium.");
      }
      return *m_pcMedium;
   }

   /****************************************/
   /****************************************/

   void CRadioEntity::SetMedium(CRadioMedium& c_medium) {
      if(m_pcMedium != nullptr && m_pcMedium != &c_medium)
         m_pcMedium->RemoveEntity(*this);
      m_pcMedium = &c_medium;
   }

   /****************************************/
   /****************************************/

   void CRadioEntitySpaceHashUpdater::operator()(CAbstractSpaceHash<CRadioEntity>& c_space_hash,
                                                          CRadioEntity& c_element) {
      /* Calculate the position of the radop in the space hash */
      c_space_hash.SpaceToHashTable(m_nI, m_nJ, m_nK, c_element.GetPosition());
      /* Update the corresponding cell */
      c_space_hash.UpdateCell(m_nI, m_nJ, m_nK, c_element);
   }

   /****************************************/
   /****************************************/

   CRadioEntityGridUpdater::CRadioEntityGridUpdater(CGrid<CRadioEntity>& c_grid) :
      m_cGrid(c_grid) {}

   /****************************************/
   /****************************************/

   bool CRadioEntityGridUpdater::operator()(CRadioEntity& c_entity) {
      try {
         /* Calculate the position of the radio in the space hash */
         m_cGrid.PositionToCell(m_nI, m_nJ, m_nK, c_entity.GetPosition());
         /* Update the corresponding cell */
         m_cGrid.UpdateCell(m_nI, m_nJ, m_nK, c_entity);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("While updating the radio grid for radio \"" <<
                                     c_entity.GetContext() + c_entity.GetId() << "\"", ex);
      }
      /* Continue with the other entities */
      return true;
   }

   /****************************************/
   /****************************************/

   class CSpaceOperationAddCRadioEntity : public CSpaceOperationAddEntity {
   public:
      void ApplyTo(CSpace& c_space, CRadioEntity& c_entity) {
         /* Add entity to space - this ensures that the radio entity
          * gets an id before being added to the radio medium */
         c_space.AddEntity(c_entity);
         /* Enable the radio entity, if it's enabled - this ensures that
          * the entity gets added to the radio if it's enabled */
         c_entity.SetEnabled(c_entity.IsEnabled());
      }
   };

   class CSpaceOperationRemoveCRadioEntity : public CSpaceOperationRemoveEntity {
   public:
      void ApplyTo(CSpace& c_space, CRadioEntity& c_entity) {
         /* Disable the entity - this ensures that the entity is
          * removed from the radio medium */
         c_entity.Disable();
         /* Remove the radio entity from space */
         c_space.RemoveEntity(c_entity);
      }
   };

   REGISTER_SPACE_OPERATION(CSpaceOperationAddEntity,
                            CSpaceOperationAddCRadioEntity,
                            CRadioEntity);
   REGISTER_SPACE_OPERATION(CSpaceOperationRemoveEntity,
                            CSpaceOperationRemoveCRadioEntity,
                            CRadioEntity);

   /****************************************/
   /****************************************/

}
