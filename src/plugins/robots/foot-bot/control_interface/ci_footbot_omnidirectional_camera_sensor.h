/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_omnidirectional_camera_sensor.h>
 *
 * @brief This file provides the definition of the foot-bot omnidirectional camera sensor.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Alessandro Stranieri - <alessandro.stranieri@ulb.ac.be>
 */

#ifndef CI_FOOTBOT_OMNIDIRECTIONAL_CAMERA_SENSOR_H
#define CI_FOOTBOT_OMNIDIRECTIONAL_CAMERA_SENSOR_H

namespace argos {
	class CCI_FootBotOmnidirectionalCameraSensor;
}

#include <argos3/core/control_interface/ci_camera_sensor.h>

namespace argos {

	/**
	 * This class provides the main interface to the omnidirectional camera on the FootBot.
	 *
	 * @see CCI_CameraSensor
	 */
	class CCI_FootBotOmnidirectionalCameraSensor: virtual public CCI_CameraSensor {

		public:

			CCI_FootBotOmnidirectionalCameraSensor() {}

			virtual ~CCI_FootBotOmnidirectionalCameraSensor() {}
	};

}

#endif
