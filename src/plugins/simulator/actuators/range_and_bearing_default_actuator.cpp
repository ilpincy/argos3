/**
 * @file <argos3/plugins/simulator/actuators/range_and_bearing_default_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "range_and_bearing_default_actuator.h"
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CRangeAndBearingDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcRangeAndBearingEquippedEntity = &c_entity.GetComponent<CRABEquippedEntity>("rab");
      m_pcRangeAndBearingEquippedEntity->SetCanBeEnabledIfDisabled(true);
      m_pcRangeAndBearingEquippedEntity->Enable();
      m_cData.Resize(m_pcRangeAndBearingEquippedEntity->GetMsgSize());
   }

   /****************************************/
   /****************************************/

   void CRangeAndBearingDefaultActuator::Update() {
      m_pcRangeAndBearingEquippedEntity->SetData(m_cData);
   }

   /****************************************/
   /****************************************/

   void CRangeAndBearingDefaultActuator::Reset() {
      m_cData.Zero();
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CRangeAndBearingDefaultActuator,
                     "range_and_bearing", "default",
                     "Carlo Pinciroli [ilpincy@gmail.com]",
                     "1.0",
                     "The range and bearing actuator",
                     "TODO",
                     "Usable");

}
