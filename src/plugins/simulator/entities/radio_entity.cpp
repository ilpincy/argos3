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
      CPositionalEntity(pc_parent) {}

   /****************************************/
   /****************************************/

   CRadioEntity::CRadioEntity(CComposableEntity* pc_parent,
                              const std::string& str_id,
                              Real f_range) :
      CPositionalEntity(pc_parent, str_id, CVector3(), CQuaternion()),
      m_fRange(f_range) {}

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

   void CRadioEntity::AddToMedium(CRadioMedium& c_medium) {
      if(HasMedium()) RemoveFromMedium();
      m_pcMedium = &c_medium;
      c_medium.AddEntity(*this);
      Enable();
   }

   /****************************************/
   /****************************************/

   void CRadioEntity::RemoveFromMedium() {
      try {
         GetMedium().RemoveEntity(*this);
         m_pcMedium = nullptr;
         Disable();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't remove radio entity \"" << GetContext() + GetId() << "\" from medium.", ex);
      }
   }

   /****************************************/
   /****************************************/
      
   CRadioMedium& CRadioEntity::GetMedium() const {
      if(m_pcMedium == nullptr) {
         THROW_ARGOSEXCEPTION("Radio entity \"" << GetContext() + GetId() << "\" has no medium associated.");
      }
      return *m_pcMedium;
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

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CRadioEntity);

   /****************************************/
   /****************************************/

}
