/**
 * @file <argos3/plugins/simulator/entities/simple_radio_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "simple_radio_entity.h"
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/simulator/media/simple_radio_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CSimpleRadioEntity::CSimpleRadioEntity(CComposableEntity* pc_parent) :
      CPositionalEntity(pc_parent),
      m_pcMedium(nullptr),
      m_fRange(0.0f) {}

   /****************************************/
   /****************************************/

   CSimpleRadioEntity::CSimpleRadioEntity(CComposableEntity* pc_parent,
                                          const std::string& str_id,
                                          CSimpleRadioMedium& c_medium,
                                          Real f_range) :
      CPositionalEntity(pc_parent, str_id, CVector3(), CQuaternion()),
      m_pcMedium(&c_medium),
      m_fRange(f_range) {}

   /****************************************/
   /****************************************/

   void CSimpleRadioEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Parse XML */
         CPositionalEntity::Init(t_tree);
         GetNodeAttribute(t_tree, "range", m_fRange);
         std::string strMedium;
         GetNodeAttribute(t_tree, "medium", strMedium);
         m_pcMedium = &CSimulator::GetInstance().GetMedium<CSimpleRadioMedium>(strMedium);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error while initializing simple radio entity", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioEntity::Reset() {
      /* Erase received messages */
      m_vecMessages.clear();
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioEntity::Destroy() {
      Disable();
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioEntity::SetEnabled(bool b_enabled) {
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

   CSimpleRadioMedium& CSimpleRadioEntity::GetMedium() const {
      if(m_pcMedium == nullptr) {
         THROW_ARGOSEXCEPTION("radio entity \"" << GetContext() << GetId() <<
                              "\" has no associated medium.");
      }
      return *m_pcMedium;
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioEntity::SetMedium(CSimpleRadioMedium& c_medium) {
      if(m_pcMedium != nullptr && m_pcMedium != &c_medium)
         m_pcMedium->RemoveEntity(*this);
      m_pcMedium = &c_medium;
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioEntitySpaceHashUpdater::operator()(CAbstractSpaceHash<CSimpleRadioEntity>& c_space_hash,
                                                       CSimpleRadioEntity& c_element) {
      /* Calculate the position of the radop in the space hash */
      c_space_hash.SpaceToHashTable(m_nI, m_nJ, m_nK, c_element.GetPosition());
      /* Update the corresponding cell */
      c_space_hash.UpdateCell(m_nI, m_nJ, m_nK, c_element);
   }

   /****************************************/
   /****************************************/

   CSimpleRadioEntityGridUpdater::CSimpleRadioEntityGridUpdater(CGrid<CSimpleRadioEntity>& c_grid) :
      m_cGrid(c_grid) {}

   /****************************************/
   /****************************************/

   bool CSimpleRadioEntityGridUpdater::operator()(CSimpleRadioEntity& c_entity) {
      try {
         /* Calculate the position of the radio in the space hash */
         m_cGrid.PositionToCell(m_nI, m_nJ, m_nK, c_entity.GetPosition());
         /* Update the corresponding cell */
         m_cGrid.UpdateCell(m_nI, m_nJ, m_nK, c_entity);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("While updating the simple radio grid for simple radio \"" <<
                                     c_entity.GetContext() + c_entity.GetId() << "\"", ex);
      }
      /* Continue with the other entities */
      return true;
   }

   /****************************************/
   /****************************************/

   class CSpaceOperationAddCSimpleRadioEntity : public CSpaceOperationAddEntity {
   public:
      void ApplyTo(CSpace& c_space, CSimpleRadioEntity& c_entity) {
         /* Add entity to space - this ensures that the radio entity
          * gets an id before being added to the radio medium */
         c_space.AddEntity(c_entity);
         /* Enable the radio entity, if it's enabled - this ensures that
          * the entity gets added to the radio if it's enabled */
         c_entity.SetEnabled(c_entity.IsEnabled());
      }
   };

   class CSpaceOperationRemoveCSimpleRadioEntity : public CSpaceOperationRemoveEntity {
   public:
      void ApplyTo(CSpace& c_space, CSimpleRadioEntity& c_entity) {
         /* Disable the entity - this ensures that the entity is
          * removed from the radio medium */
         c_entity.Disable();
         /* Remove the radio entity from space */
         c_space.RemoveEntity(c_entity);
      }
   };

   REGISTER_SPACE_OPERATION(CSpaceOperationAddEntity,
                            CSpaceOperationAddCSimpleRadioEntity,
                            CSimpleRadioEntity);
   REGISTER_SPACE_OPERATION(CSpaceOperationRemoveEntity,
                            CSpaceOperationRemoveCSimpleRadioEntity,
                            CSimpleRadioEntity);

   /****************************************/
   /****************************************/

}
