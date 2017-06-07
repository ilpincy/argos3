#include "kilobot_communication_default_actuator.h"
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/simulator.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CKilobotCommunicationDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      m_pcCommEntity = &c_entity.GetComponent<CKilobotCommunicationEntity>("kilocomm");
      m_pcCommEntity->Enable();
   }

   /****************************************/
   /****************************************/

   void CKilobotCommunicationDefaultActuator::Update() {
      m_pcCommEntity->SetTxMessage(m_ptMessage);
   }

   /****************************************/
   /****************************************/

   void CKilobotCommunicationDefaultActuator::Reset() {
   }

   /****************************************/
   /****************************************/

   void CKilobotCommunicationDefaultActuator::SetMessage(message_t* pt_msg) {
      CCI_KilobotCommunicationActuator::SetMessage(pt_msg);
      m_pcCommEntity->SetTxStatus(CKilobotCommunicationEntity::TX_ATTEMPT);
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CKilobotCommunicationDefaultActuator,
                     "kilobot_communication", "default",
                     "Carlo Pinciroli [ilpincy@gmail.com]",
                     "1.0",
                     "The Kilobot communication actuator.",
                     "This actuator allows Kilobots to perform situated communication, i.e., a form\n"
                     "of wireless communication whereby the receiver also knows the distance of the\n"
                     "sender.\n"
                     "To use this actuator, in controllers you must include the\n"
                     "ci_kilobot_communication_actuator.h header.\n"
                     "This actuator only allows a Kilobot to send messages. To receive messages, you\n"
                     "need the range-and-bearing sensor.\n\n"
                     "REQUIRED XML CONFIGURATION\n\n"
                     "  <controllers>\n"
                     "    ...\n"
                     "    <my_controller ...>\n"
                     "      ...\n"
                     "      <actuators>\n"
                     "        ...\n"
                     "        <kilobot_communication implementation=\"default\" />\n"
                     "        ...\n"
                     "      </actuators>\n"
                     "      ...\n"
                     "    </my_controller>\n"
                     "    ...\n"
                     "  </controllers>\n\n"
                     "OPTIONAL XML CONFIGURATION\n\n"
                     "None for the time being.\n",
                     "Usable");
   
}
