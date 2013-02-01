/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_beacon_actuator.h>
 *
 * @brief This file provides the definition of footbot beacon actuator.
 *
 * This file provides the definition of footbot beacon actuator.
 *
 * The beacon is located between the iMX board and the omnidirectional camera.
 * It is basically a very powerful LED, that can be seen from long distance and
 * can be used to recognize the center of the robot with suitable vision procedures.
 * The beacon should be rarely used at full power, and never for long periods of time.
 * In fact, if used at full power, it gets quickly to a very high temperature, which can
 * damage the connection between the iMX and the omnidirectional camera.
 *
 * The beacon intensity can be set by using the SetBeaconIntensity method. The method
 * SetColor also sets the intensity (default: full intensity).
 *
 * [TODO: CHECK IF THE FOLLOWING STATEMENTS ARE TRUE]
 * However, to prevent damage, at low level a temperature check is performed. If the
 * temperature gets too high, the beacon is automatically lit off and further commands
 * ignored.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Giovanni Pini   - <gpini@iridia.ulb.ac.be>
 */

#ifndef CCI_FOOTBOT_BEACON_ACTUATOR_H
#define CCI_FOOTBOT_BEACON_ACTUATOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_FootBotBeaconActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/datatypes/color.h>

namespace argos {

   class CCI_FootBotBeaconActuator : virtual public CCI_Actuator {

   public:

      virtual ~CCI_FootBotBeaconActuator() {}

      virtual void SetColor(const CColor& c_color) = 0;

   };

}

#endif
