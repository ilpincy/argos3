/**
 * @file <argos3/plugins/simulator/actuators/range_and_bearing_default_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "range_and_bearing_default_actuator.h"
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/simulator.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CRangeAndBearingDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcRangeAndBearingEquippedEntity = &c_entity.GetComponent<CRABEquippedEntity>("rab");
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
                     "The range and bearing actuator.",

                     "This actuator allows robots to perform situated communication, i.e., a form of\n"
                     "wireless communication whereby the receiver also knows the location of the\n"
                     "sender with respect to its own frame of reference.\n"
                     "This actuator allows a robot to send messages. To receive messages, you need\n"
                     "the range-and-bearing sensor.\n"
                     "To use this actuator, in controllers you must include the\n"
                     "ci_range_and_bearing_actuator.h header.\n\n"

                     "REQUIRED XML CONFIGURATION\n\n"

                     "  <controllers>\n"
                     "    ...\n"
                     "    <my_controller ...>\n"
                     "      ...\n"
                     "      <actuators>\n"
                     "        ...\n"
                     "        <range_and_bearing implementation=\"default\" />\n"
                     "        ...\n"
                     "      </actuators>\n"
                     "      ...\n"
                     "    </my_controller>\n"
                     "    ...\n"
                     "  </controllers>\n\n"

                     "OPTIONAL XML CONFIGURATION\n\n"

                     "None.\n",

                     "Usable");
   
}
