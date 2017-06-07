/**
 * @file <argos3/plugins/robots/kilobot/simulator/kilobot_communication_default_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef RANGE_AND_BEARING_DEFAULT_ACTUATOR_H
#define RANGE_AND_BEARING_DEFAULT_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CKilobotCommunicationDefaultActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/kilobot/control_interface/ci_kilobot_communication_actuator.h>
#include <argos3/plugins/robots/kilobot/simulator/kilobot_communication_entity.h>

namespace argos {

   class CKilobotCommunicationDefaultActuator : public CSimulatedActuator,
                                                public CCI_KilobotCommunicationActuator {

   public:

      CKilobotCommunicationDefaultActuator() {}
      virtual ~CKilobotCommunicationDefaultActuator() {}
      virtual void SetRobot(CComposableEntity& c_entity);
      virtual void Update();
      virtual void Reset();
      virtual void SetMessage(message_t* pt_msg);

   private:

      CKilobotCommunicationEntity* m_pcCommEntity;

   };

}

#endif
