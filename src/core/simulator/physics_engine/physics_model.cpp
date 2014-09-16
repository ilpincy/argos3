#include "physics_model.h"
#include <argos3/core/simulator/entity/embodied_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CPhysicsModel::CalculateAnchors() {
      std::vector<SAnchor*>& vecAnchors = m_cEmbodiedEntity.GetEnabledAnchors();
      for(size_t i = 0; i < vecAnchors.size(); ++i) {
         Call(*vecAnchors[i]);
      }
   }

   /****************************************/
   /****************************************/

   void CPhysicsModel::Call(SAnchor& s_anchor) {
      if(m_mapThunks.count(&s_anchor) > 0) {
         TThunk tThunk = m_mapThunks[&s_anchor];
         (this->*tThunk)(s_anchor);
      }
   }

   /****************************************/
   /****************************************/

}
