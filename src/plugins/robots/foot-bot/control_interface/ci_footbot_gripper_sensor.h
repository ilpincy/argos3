/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_gripper_sensor.h>
 *
 * @brief This file provides the common interface  definition of the gripper sensor.
 *
 * This file provides the common interface  definition of the gripper sensor.
 * The sensor measures the current gripper aperture, in the future it will also
 * integrate functions to query the fact that an object is currently gripped
 *
 * @author Giovanni Pini - <gpini@iridia.ulb.ac.be>
 */

#ifndef CCI_FOOTBOT_GRIPPER_SENSOR_H
#define CCI_FOOTBOT_GRIPPER_SENSOR_H

namespace argos {
   class CCI_FootBotGripperSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/angles.h>

namespace argos {

   class CCI_FootBotGripperSensor: virtual public CCI_Sensor {

   public:

      enum EGripPhase {
         OBJECT_NOT_GRIPPED = 0,
         CHECK_IN_PROGRESS,
         OBJECT_GRIPPED
      };

      struct SReading {
         CRadians GripperAperture;
         bool ObjectInGripper;

         SReading() :
            GripperAperture(0.0),
            ObjectInGripper(false) {}

         SReading(const CRadians& GripperAperture,
                  bool b_object_in_gripper) :
            GripperAperture(GripperAperture),
            ObjectInGripper(b_object_in_gripper) {}

      };

   public:

      CCI_FootBotGripperSensor() :
         m_eGripPhase(OBJECT_NOT_GRIPPED) {}

			virtual ~CCI_FootBotGripperSensor() {}

      inline const SReading& GetReading() const {
         return m_sReading;
      }

			inline bool IsObjectInGripper() const {
         return m_sReading.ObjectInGripper;
      }

			inline EGripPhase GetGripPhase() const {
         return m_eGripPhase;
      }

   protected:

      SReading m_sReading;
      EGripPhase m_eGripPhase;
   };

}

#endif
