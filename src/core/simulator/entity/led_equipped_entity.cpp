/**
 * @file <argos3/core/simulator/entity/led_equipped_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "led_equipped_entity.h"

namespace argos {

   /****************************************/
   /****************************************/

   CLedEquippedEntity::~CLedEquippedEntity() {
      while(! m_tLeds.empty()) {
         CLedEntity* pcTmp = m_tLeds.back();
         m_tLeds.pop_back();
         delete pcTmp;
      }
   }

   /****************************************/
   /****************************************/

   void CLedEquippedEntity::Init(TConfigurationNode& t_tree) {
      CEntity::Init(t_tree);
      for(UInt32 i = 0; i < m_tLeds.size(); ++i) {
         m_tLeds[i]->Init(t_tree);
         m_tLeds[i]->SetId(GetId() + "_led_" + ToString(i));
      }
   }

   /****************************************/
   /****************************************/

   void CLedEquippedEntity::Reset() {
      for(TLedEntityList::iterator it = m_tLeds.begin();
          it != m_tLeds.end();
          ++it) {
         (*it)->Reset();
      }
   }

   /****************************************/
   /****************************************/

}
