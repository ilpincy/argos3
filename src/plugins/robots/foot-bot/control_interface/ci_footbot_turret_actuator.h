/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_turret_actuator.h>
 *
 * @brief This file provides the common interface definition of the turret actuator.

 * This file provides the definition of the turret actuator. The actuator
 * allows the user to control the turret's angular position or the turret
 * rotation speed.
 *
 * The user can dynamically decide the turret control mode.
 * Possible modes are: passive, speed control, position control, and off.
 *
 * When the turret is passive, it is loose and does not resist toi external
 * forces that make the turret turn, but the angular position of the turret can be tracked.
 *
 * When the turret is off, it is completely loose, and additionally no measures can be taken
 * concerning the turret rotation. Off mode should be used only to turn the actuator off, or
 * in cases where a loose turret is needed, but no angular measures are needed.
 *
 * When the turret is controlled in speed, the user can decide the rotation speed. Rotation
 * speed is expressed in PID pulses, positive values make the turret rotate counter clockwise,
 * when looking from above. When the turret is controlled in speed, it resists to external
 * forces that try to move the turret. Even if the target speed is set to zero, the turret will
 * try to resist to external forces that try to move it.
 *
 * When the turret is controlled in position, the user can decide the rotation angle.
 * Rotation is expressed in radians, positive values make the turret rotate counter clockwise,
 * when looking from above. When the turret is controlled in position, it resists to external
 * forces that try to move the turret.
 *
 * ACTUATED VALUES LIMITS:
 * TODO
 * The limits are enforced by the common interface.
 *
 * IMPORTANT: the robot uses the turret to determine when an object has been successfully gripped.
 * Therefore, right after a new gripper command has been issued, can be the case that the robot is using
 * the turret to assess whether gripping was successful. In the meantime EVERY command issued to
 * the turret is IGNORED. The check operation lasts less than a second.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CCI_FOOTBOT_TURRET_ACTUATOR_H
#define CCI_FOOTBOT_TURRET_ACTUATOR_H

namespace argos {
   class CCI_FootBotTurretActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/math/angles.h>

namespace argos {

   class CCI_FootBotTurretActuator : virtual public CCI_Actuator {

   public:

      /** Turret modes */
      enum ETurretModes {
         MODE_OFF,
         MODE_PASSIVE,
         MODE_SPEED_CONTROL,
         MODE_POSITION_CONTROL
      };

   public:

      static const CRange<SInt32> SPEED_RANGE;
      static const CRange<Real>   NORMALIZED_SPEED_RANGE;

   public:

      /**
       * Destructor.
       */
      virtual ~CCI_FootBotTurretActuator() {}

      /**
       * @brief Sets the turret rotation
       * Turret rotation is expressed in radians, positive values rotate the turret clockwise
       * when looking from above.
       * In the current real robot implementation, values are limited using range functions.
       * If the turret is not in position control mode, the only effect is that the target rotation is changed
       * but the turret will not turn: it will turn when position control mode is activated
       * (unless another target rotation is specified meantime).
       * @param c_angle desired turret rotation
       */
      virtual void SetRotation(const CRadians& c_angle) = 0;

      /**
       * @brief Sets the turret rotation speed
       * Turret rotation speed is expressed in motor pulses, positive values rotate the turret clockwise
       * when looking from above.
       * In the current real robot implementation, values are limited using range functions.
       * If the turret is not in speed control mode, the only effect is that the target rotation speed is changed
       * but the turret will not turn: it will turn when speed control mode is activated
       * (unless another target rotation speed is specified meantime).
       * @param n_speed_pulses desired turret rotation speed - pid pulses
       */
      virtual void SetRotationSpeed(SInt32 n_speed_pulses) = 0;

      /**
       * @brief Sets the turret control mode
       * Modes are: angular position control, rotation speed control, passive mode, off
       * @param un_mode turret control mode, possible values are defined in CCI_FootBotTurretActuator
       */
      virtual void SetMode(ETurretModes e_mode) = 0;

      /**
       * @brief Sets the turret control mode to active, and sets the target rotation to the given one
       *
       * @param c_angle desired turret rotation
       * @see SetMode
       * @see SetRotation
       *
       */
      void SetActiveWithRotation(const CRadians& c_angle);

      /**
       * @brief Sets the turret control mode to speed control
       * @see SetMode
       */
      void SetSpeedControlMode();

      /**
       * @brief Sets the turret control mode to position control
       * @see SetMode
       */
      void SetPositionControlMode();

      /**
       * @brief Sets the turret control mode to passive
       * @see SetMode
       */
      void SetPassiveMode();

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif
   };

}

#endif
