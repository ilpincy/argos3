/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_ceiling_camera_sensor.h>
 *
 * @brief This file provides the definition of the FootBot ceiling camera sensor.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CI_FOOTBOT_CEILING_CAMERA_SENSOR_H
#define CI_FOOTBOT_CEILING_CAMERA_SENSOR_H

namespace argos {
   class CCI_FootBotCeilingCameraSensor;
}

#include <argos3/core/control_interface/ci_camera_sensor.h>

namespace argos {

   /**
    * This class provides the main interface to the ceiling camera on the FootBot.
    *
    * @see CCI_CameraSensor
    */
   class CCI_FootBotCeilingCameraSensor: virtual public CCI_CameraSensor {

   public:

      CCI_FootBotCeilingCameraSensor() {}

      virtual ~CCI_FootBotCeilingCameraSensor() {}

   };

}

#endif
