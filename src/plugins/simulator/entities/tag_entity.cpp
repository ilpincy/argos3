/**
 * @file <argos3/plugins/simulator/entities/tag_entity.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "tag_entity.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/simulator/media/tag_medium.h>

namespace argos {

   /****************************************/
   /****************************************/

   CTagEntity::CTagEntity(CComposableEntity* pc_parent) :
      CPositionalEntity(pc_parent),
      m_cObservableAngle(CRadians::PI),
      m_fSideLength(0.0f),
      m_pcMedium(nullptr) {
      Disable();
   }

   /****************************************/
   /****************************************/

   CTagEntity::CTagEntity(CComposableEntity* pc_parent,
                          const std::string& str_id,
                          const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          const CRadians& c_observable_angle,
                          Real f_side_length,
                          const std::string& str_payload) :
      CPositionalEntity(pc_parent, str_id, c_position, c_orientation),
      m_cObservableAngle(c_observable_angle),
      m_fSideLength(f_side_length),
      m_strPayload(str_payload),
      m_strInitPayload(str_payload),
      m_pcMedium(nullptr) {
      SetPayload(str_payload);
   }

   /****************************************/
   /****************************************/

   void CTagEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Parse XML */
         CPositionalEntity::Init(t_tree);
         GetNodeAttribute(t_tree, "payload", m_strInitPayload);
         m_strPayload = m_strInitPayload;
         CDegrees cObservableAngle;
         GetNodeAttribute(t_tree, "observable_angle", cObservableAngle);
         m_cObservableAngle = ToRadians(cObservableAngle);
         GetNodeAttribute(t_tree, "side_length", m_fSideLength);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error while initializing tag entity", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CTagEntity::Reset() {
      m_strPayload = m_strInitPayload;
   }

   /****************************************/
   /****************************************/

   void CTagEntity::Destroy() {
      if(HasMedium()) {
         RemoveFromMedium();
      }
   }

   /****************************************/
   /****************************************/

   void CTagEntity::SetEnabled(bool b_enabled) {
      CEntity::SetEnabled(m_strPayload.length() > 0);
   }

   /****************************************/
   /****************************************/

   void CTagEntity::SetPayload(const std::string& str_payload) {
      m_strPayload = str_payload;
      SetEnabled(m_strPayload.length() > 0);
   }

   /****************************************/
   /****************************************/

   void CTagEntity::AddToMedium(CTagMedium& c_medium) {
      if(HasMedium()) RemoveFromMedium();
      m_pcMedium = &c_medium;
      c_medium.AddEntity(*this);
      Enable();
   }

   /****************************************/
   /****************************************/

   void CTagEntity::RemoveFromMedium() {
      try {
         GetMedium().RemoveEntity(*this);
         m_pcMedium = nullptr;
         Disable();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't remove tag entity \"" << GetContext() + GetId() << "\" from medium.", ex);
      }
   }

   /****************************************/
   /****************************************/
      
   CTagMedium& CTagEntity::GetMedium() const {
      if(m_pcMedium == nullptr) {
         THROW_ARGOSEXCEPTION("Tag entity \"" << GetContext() + GetId() << "\" has no medium associated.");
      }
      return *m_pcMedium;
   }

   /****************************************/
   /****************************************/

   void CTagEntitySpaceHashUpdater::operator()(CAbstractSpaceHash<CTagEntity>& c_space_hash,
                                               CTagEntity& c_element) {
      /* Discard tags without a payload */
      if(c_element.GetPayload().length() > 0) {
         /* Calculate the position of the LED in the space hash */
         c_space_hash.SpaceToHashTable(m_nI, m_nJ, m_nK, c_element.GetPosition());
         /* Update the corresponding cell */
         c_space_hash.UpdateCell(m_nI, m_nJ, m_nK, c_element);
      }
   }

   /****************************************/
   /****************************************/

   CTagEntityGridUpdater::CTagEntityGridUpdater(CGrid<CTagEntity>& c_grid) :
      m_cGrid(c_grid) {}

   /****************************************/
   /****************************************/

   bool CTagEntityGridUpdater::operator()(CTagEntity& c_entity) {
      /* Discard tags without a payload */
      if(c_entity.GetPayload().length() > 0) {
         try {
            /* Calculate the position of the tag in the space hash */
            m_cGrid.PositionToCell(m_nI, m_nJ, m_nK, c_entity.GetPosition());
            /* Update the corresponding cell */
            m_cGrid.UpdateCell(m_nI, m_nJ, m_nK, c_entity);
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("While updating the tag grid for tag \"" <<
                                        c_entity.GetContext() + c_entity.GetId() << "\"", ex);
         }
      }
      /* Continue with the other entities */
      return true;
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CTagEntity);

   /****************************************/
   /****************************************/

}
