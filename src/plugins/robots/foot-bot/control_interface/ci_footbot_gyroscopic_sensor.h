/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_gyroscopic_sensor.h>
 *
 * @brief This file provides the common interface definition of the footbot gyroscopes.
 *
 * The current implementation returns only the difference in rotation along the Z axis
 * with respect to the previous step.
 *
 * WARNING:
 *
 * AT THE CURRENT STATE ONLY THE ROTATION ALONG THE Z AXIS IS SUPPORTED.
 *
 * @author Giovanni Pini - <gpini@ulb.ac.be>
 */

#ifndef CCI_FOOTBOT_GYROSCOPIC_SENSOR_H
#define CCI_FOOTBOT_GYROSCOPIC_SENSOR_H

namespace argos {
   class CCI_FootBotGyroscopicSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector2.h>

namespace argos {

   class CCI_FootBotGyroscopicSensor : virtual public CCI_Sensor {

   public:

      struct SReading {
         CRadians DeltaRotationZ;
      };

   public:

      virtual ~CCI_FootBotGyroscopicSensor() {
      }

      inline const SReading& GetReading() const {
         return m_sReading;
      }

   protected:

      SReading m_sReading;

   };

}

#endif
