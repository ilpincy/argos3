/**
 * @file <argos3/core/simulator/entity/led_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "led_entity.h"

namespace argos {

   /****************************************/
   /****************************************/

   void CLedEntity::Reset() {
      m_cColor = CColor::BLACK;
   }

   /****************************************/
   /****************************************/

   void CLEDEntitySpaceHashUpdater::operator()(CAbstractSpaceHash<CLedEntity>& c_space_hash,
                                               CLedEntity& c_element) {
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
