/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_gripper_actuator.h>
 *
 * @brief This file provides the definition of the gripper actuator.

 * This file provides the definition of the gripper actuator. It allows
 * to set the gripper aperture in the range [-pi/2:pi/2]. The gripper
 * can be locked in two directions: positive and negative. In the negative
 * direction the 2 side pins of the gripper move upwards and the middle pin
 * downwards.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CCI_FOOTBOT_GRIPPER_ACTUATOR_H
#define CCI_FOOTBOT_GRIPPER_ACTUATOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_FootBotGripperActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/math/angles.h>

namespace argos {

   class CCI_FootBotGripperActuator : virtual public CCI_Actuator {

   public:

      /* Useful gripper aperture values */
      static CRadians UNLOCKED;
      static CRadians LOCKED_POSITIVE;
      static CRadians LOCKED_NEGATIVE;

   public:

      /**
       * @brief Constructor.
       */
      CCI_FootBotGripperActuator() {}

      /**
       * @brief Destructor.
       */
      virtual ~CCI_FootBotGripperActuator() {}

      /**
       * @brief Sets gripper aperture in radians.
       *
       * @param c_aperture desired aperture.
       */
      virtual void SetAperture(const CRadians& c_aperture) = 0;


      /**
       *
       * @brief Disables the ASEBA routine that checks for a gripped object each time the gripper is opened
       *
       */
      virtual void DisableCheckForObjectGrippedRoutine() = 0;

      /**
       *
       * @brief Enables the ASEBA routine that checks for a gripped object each time the gripper is opened
       *
       */
      virtual void EnableCheckForObjectGrippedRoutine() = 0;

      /**
       * @brief Sets gripper aperture to lock, positive direction.
       */
      inline void LockPositive() {
         SetAperture(LOCKED_POSITIVE);
      }

      /**
       * @brief Sets gripper aperture to lock, negative direction.
       */
      inline void LockNegative() {
         SetAperture(LOCKED_NEGATIVE);
      }

      /**
       * @brief Unlock gripper: objects are released.
       */
      inline void Unlock() {
         SetAperture(UNLOCKED);
      }

   };

}

#endif
