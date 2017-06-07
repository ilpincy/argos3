/**
 * @file <argos3/plugins/robots/kilobot/control_interface/ci_kilobot_communication_actuator.h>
 *
 * @brief This file provides the definition of the kilobot communication actuator.
 *
 * This file provides the definition of the kilobot communication actuator.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CCI_KILOBOT_COMMUNICATION_ACTUATOR_H
#define CCI_KILOBOT_COMMUNICATION_ACTUATOR_H

namespace argos {
   class CCI_KilobotCommunicationActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/plugins/robots/kilobot/control_interface/kilolib.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <vector>

namespace argos {

   class CCI_KilobotCommunicationActuator : public CCI_Actuator {

   public:

      CCI_KilobotCommunicationActuator();
      virtual ~CCI_KilobotCommunicationActuator() {}

      virtual void SetMessage(message_t* pt_msg);

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      message_t* m_ptMessage;

   };

}

#endif
