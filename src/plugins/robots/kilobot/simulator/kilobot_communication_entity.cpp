/**
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "kilobot_communication_entity.h"
#include <argos3/core/utility/string_utilities.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CKilobotCommunicationEntity::CKilobotCommunicationEntity(CComposableEntity* pc_parent,
                                                            const std::string& str_id,
                                                            size_t un_msg_size,
                                                            Real f_range,
                                                            SAnchor& s_anchor,
                                                            CEmbodiedEntity& c_entity_body) :
      CPositionalEntity(pc_parent,
                        str_id),
      m_psAnchor(&s_anchor),
      m_fTxRange(f_range),
      m_pcEntityBody(&c_entity_body),
      m_eTxStatus(TX_NONE) {
      Disable();
      SetInitPosition(s_anchor.Position);
      SetPosition(GetInitPosition());
      SetInitOrientation(s_anchor.Orientation);
      SetOrientation(GetInitOrientation());
   }

   /****************************************/
   /****************************************/

   void CKilobotCommunicationEntity::Reset() {
      m_eTxStatus = TX_NONE;
   }

   /****************************************/
   /****************************************/

   void CKilobotCommunicationEntity::Update() {
      if(m_eTxStatus == TX_SUCCESS) m_eTxStatus = TX_NONE;
      SetPosition(m_psAnchor->Position);
      SetOrientation(m_psAnchor->Orientation);
   }

   /****************************************/
   /****************************************/

   void CKilobotCommunicationEntity::SetEnabled(bool b_enabled) {
      CEntity::SetEnabled(b_enabled);
      if(b_enabled) m_psAnchor->Enable();
      else m_psAnchor->Disable();
   }

   /****************************************/
   /****************************************/

   void CKilobotCommunicationEntitySpaceHashUpdater::operator()(CAbstractSpaceHash<CKilobotCommunicationEntity>& c_space_hash,
                                                       CKilobotCommunicationEntity& c_element) {
      /* Calculate the position of the center of the kilobot communication entity in the space hash */
      c_space_hash.SpaceToHashTable(m_nCenterI,
                                    m_nCenterJ,
                                    m_nCenterK,
                                    c_element.GetPosition());
      /* Update the cells in a sphere around it */
      SInt32 nRangeI = c_space_hash.SpaceToHashTable(c_element.GetTxRange(), 0);
      SInt32 nRangeJ;
      SInt32 nRangeK;
      for(SInt32 i = 0; i <= nRangeI; ++i) {
         nRangeJ =
            c_space_hash.SpaceToHashTable(
               ::sqrt(
                  Square(c_element.GetTxRange()) -
                  Square(c_space_hash.HashTableToSpace(i, 0))
                  ),
               1);
         for(SInt32 j = 0; j <= nRangeJ; ++j) {
            nRangeK =
               c_space_hash.SpaceToHashTable(
                  ::sqrt(
                     Square(c_element.GetTxRange()) -
                     Square(c_space_hash.HashTableToSpace(j, 1))
                     ),
                  2);
            for(SInt32 k = 0; k <= nRangeK; ++k) {
               if(i > 0) {
                  /*
                   * i > 0
                   */
                  if(j > 0) {
                     /*
                      * i > 0
                      * j > 0
                      */
                     if(k > 0) {
                        /*
                         * i > 0
                         * j > 0
                         * k > 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ + j, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ + j, m_nCenterK - k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ - j, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ - j, m_nCenterK - k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ + j, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ + j, m_nCenterK - k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ - j, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ - j, m_nCenterK - k, c_element);
                     }
                     else {
                        /*
                         * i > 0
                         * j > 0
                         * k == 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ + j, m_nCenterK, c_element);
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ - j, m_nCenterK, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ + j, m_nCenterK, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ - j, m_nCenterK, c_element);
                     }
                  }
                  else {
                     /*
                      * i > 0
                      * j == 0
                      */
                     if(k > 0) {
                        /*
                         * i > 0
                         * j == 0
                         * k > 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ, m_nCenterK - k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ, m_nCenterK - k, c_element);
                     }
                     else {
                        /*
                         * i > 0
                         * j == 0
                         * k == 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI + i, m_nCenterJ, m_nCenterK, c_element);
                        c_space_hash.UpdateCell(m_nCenterI - i, m_nCenterJ, m_nCenterK, c_element);
                     }
                  }
               }
               else {
                  /*
                   * i == 0
                   */
                  if(j > 0) {
                     /*
                      * i == 0
                      * j > 0
                      */
                     if(k > 0) {
                        /*
                         * i == 0
                         * j > 0
                         * k > 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ + j, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ + j, m_nCenterK - k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ - j, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ - j, m_nCenterK - k, c_element);
                     }
                     else {
                        /*
                         * i == 0
                         * j > 0
                         * k == 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ + j, m_nCenterK, c_element);
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ - j, m_nCenterK, c_element);
                     }
                  }
                  else {                     
                     /*
                      * i == 0
                      * j == 0
                      */
                     if(k > 0) {
                        /*
                         * i == 0
                         * j == 0
                         * k > 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ, m_nCenterK + k, c_element);
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ, m_nCenterK - k, c_element);
                     }
                     else {
                        /*
                         * i == 0
                         * j == 0
                         * k == 0
                         */
                        c_space_hash.UpdateCell(m_nCenterI, m_nCenterJ, m_nCenterK, c_element);
                     }
                  }
               }
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_ENTITY(CKilobotCommunicationEntity);

   /****************************************/
   /****************************************/

   CKilobotCommunicationEntityGridCellUpdater::CKilobotCommunicationEntityGridCellUpdater(CGrid<CKilobotCommunicationEntity>& c_grid) :
      m_cGrid(c_grid) {}
   
   bool CKilobotCommunicationEntityGridCellUpdater::operator()(SInt32 n_i,
                                                      SInt32 n_j,
                                                      SInt32 n_k,
                                                      CGrid<CKilobotCommunicationEntity>::SCell& s_cell) {
      /* Update cell */
      m_cGrid.UpdateCell(n_i, n_j, n_k, *m_pcEntity);
      /* Continue with other cells */
      return true;
   }
   
   void CKilobotCommunicationEntityGridCellUpdater::SetEntity(CKilobotCommunicationEntity& c_entity) {
      m_pcEntity = &c_entity;
   }

   CKilobotCommunicationEntityGridEntityUpdater::CKilobotCommunicationEntityGridEntityUpdater(CGrid<CKilobotCommunicationEntity>& c_grid) :
      m_cGrid(c_grid),
      m_cCellUpdater(c_grid) {}

   bool CKilobotCommunicationEntityGridEntityUpdater::operator()(CKilobotCommunicationEntity& c_entity) {
      try {
         m_cCellUpdater.SetEntity(c_entity);
         m_cGrid.ForCellsInBoxRange(c_entity.GetPosition(),
                                    CVector3(c_entity.GetTxRange(),
                                             c_entity.GetTxRange(),
                                             c_entity.GetTxRange()),
                                    m_cCellUpdater);
         /* Continue with the other entities */
         return true;
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("While updating the kilobot entity grid for kilobot entity \"" << c_entity.GetContext() << c_entity.GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

}
