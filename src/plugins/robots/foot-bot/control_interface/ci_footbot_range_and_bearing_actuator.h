/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_range_and_bearing_actuator.h>
 *
 * @brief This file provides the definition of the actuator part of the
 *        range and bearing system.
 *
 * The range and bearing sensor/actuator pair allows robots to
 * sense other robots in their locality and exchange data with them.
 * When the presence of a robot cannot be sensed anymore (i.e., the sensing
 * robot does not receive an IR beam from the robot), the data sent
 * over the local radio is ignored. Thus, this sensor/actuator pair
 * provides localization and local communication to the robot.
 * The range of the sensor depends on the robot type.
 *
 * At each timestep, each robot emits out an IR beam, allowing other robots
 * to sense its position. At the same time, the robot sends a packet
 * over the local radio. This packet is always sent, even when not data
 * has been set by the controller. In this case, the data transmitted
 * is only zeros. The data has a size of 10 bytes.
 *
 * The controller can set the data to be sent, which is sent by the low-
 * level aseba script in the next available time slot. The data can be
 * set using the method CCI_FootBotRangeAndBearingActuator::SetData(). Thus, the
 * controller does not have direct control about the time the data is
 * set. To make sure that each packet is sent at least once, the actuator
 * does not accept new data to be sent until the previous date has
 * been transmitted at least once.
 *
 * To make this behavior transparent to the calling controller, it has
 * the the possibility to check if the currently set message has already
 * been transmitted (and that the actuator accepts new data) by calling
 * the method CCI_FootBotRangeAndBearingActuator::IsLastPacketSent().
 *
 * @author Arne Brutschy - <arne.brutschy@ulb.ac.be>
 */

#ifndef CCI_FOOTBOT_RANGE_AND_BEARING_ACTUATOR_H
#define CCI_FOOTBOT_RANGE_AND_BEARING_ACTUATOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_FootBotRangeAndBearingActuator;
}

#include <string>
#include <vector>
#include <iostream>

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_range_and_bearing_sensor.h>

namespace argos {

   class CCI_FootBotRangeAndBearingActuator : virtual public CCI_Actuator {

   public:

      CCI_FootBotRangeAndBearingActuator() :
         m_bBufferFree(true) {
      }

      virtual ~CCI_FootBotRangeAndBearingActuator() {
      }

      /* Checks if the last packet has been sent already. */
      inline bool IsLastPacketSent() const {
         return m_bBufferFree;
      }

      /* Set the data to send. This will be ignored if the last packet has not been sent yet. */
      virtual void SetData(const SRABPacket::TData& t_data) = 0;

   protected:

      bool m_bBufferFree;

   };

}

#endif
