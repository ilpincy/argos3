/**
 * @file <argos3/core/simulator/space/entities/rab_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "rab_equipped_entity.h"

namespace argos {

   /****************************************/
   /****************************************/

   std::vector<UInt16> CRABEquippedEntity::AVAILABLE_IDS(65535, 1);

   /****************************************/
   /****************************************/

   void CRABEquippedEntitySpaceHashUpdater::operator()(CAbstractSpaceHash<CRABEquippedEntity>& c_space_hash,
                                                       CRABEquippedEntity& c_element) {
      /* Calculate the position of the center of the RAB equipped entity in the space hash */
      c_space_hash.SpaceToHashTable(m_nCenterI,
                                    m_nCenterJ,
                                    m_nCenterK,
                                    c_element.GetPosition());
      /* Update the cells in a sphere around it */
      SInt32 nRangeI = c_space_hash.SpaceToHashTable(c_element.GetRange(), 0);
      SInt32 nRangeJ;
      SInt32 nRangeK;
      for(SInt32 i = 0; i <= nRangeI; ++i) {
         nRangeJ =
            c_space_hash.SpaceToHashTable(
               ::sqrt(
                  Square(c_element.GetRange()) -
                  Square(c_space_hash.HashTableToSpace(i, 0))
                  ),
               1);
         for(SInt32 j = 0; j <= nRangeJ; ++j) {
            nRangeK =
               c_space_hash.SpaceToHashTable(
                  ::sqrt(
                     Square(c_element.GetRange()) -
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

}
