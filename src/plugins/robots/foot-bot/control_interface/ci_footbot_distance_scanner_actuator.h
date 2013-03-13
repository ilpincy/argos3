/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_distance_scanner_actuator.h>
 *
 * @brief This file provides the definition of the distance scanner sensor.
 *
 * This file provides the definition of the distance scanner actuator. This sensor
 * is located right below the iMX board. It is intended to return quite
 * precise (1cm accuracy) distance measurements of the objects around the
 * robot. As such, it can be seen as a better proximity sensor, in that
 * besides accuracy it also provides higher range.
 *
 * The distance scanner is composed by two pairs of sensors:
 * - short range sensors, that can measure distances in the range [4cm;30cm]
 *   and correspond to readings 0 and 2;
 * - long range sensors, that can measure distances in the range [20cm;150cm]
 *   and correspond to readings 1 and 3.
 *
 * Furthermore, the sensor can rotate. It can be rotated to a specific angle,
 * or it can keep rotating at a desired constant speed. One setting excludes the
 * other, so if the position is set, the sensor doesn't keep rotating; similarly,
 * if you set the speed, the sensor can't keep its position. Pretty self-evident,
 * but it's better to be really clear.
 * Finally, when the scanner is still, its accuracy is maximum; when the
 * scanner rotates, the accuracy degrades as the speed increases.
 *
 * The sensors, when the device is rotated at angle 0, are placed as follows
 * (seeing the robot from TOP, battery socket is the BACK):
 *
 *      front
 *
 *        0
 *
 * left 1   3 right
 *
 *        2
 *
 *      back
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CCI_FOOTBOT_DISTANCE_SCANNER_ACTUATOR_H
#define CCI_FOOTBOT_DISTANCE_SCANNER_ACTUATOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_FootBotDistanceScannerActuator;
};

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/math/angles.h>

namespace argos {

   class CCI_FootBotDistanceScannerActuator: virtual public CCI_Actuator {

   public:

       /**
        * Destructor.
        */
      virtual ~CCI_FootBotDistanceScannerActuator() {}

      /**
       *
       * @brief Sets the distance scanner angle
       * The distance scanner can be controller in position or in speed.
       * This method allows you to set the desired angle of the ds
       *
       * @param c_angle desired distance scanner angle - radians
       */
      virtual void SetAngle(const CRadians& c_angle) = 0;

      /**
       *
       * @brief Sets the distance scanner speed
       * The distance scanner can be controller in position or in speed.
       * This method allows you to set the desired speed of the ds
       * The speed is set in rounds per minutes, an OK value is 30rpm
       *
       * @param c_angle desired distance scanner speed - rounds per minutes
       */
      virtual void SetRPM(Real f_rpm) = 0;

      /**
       * @brief Enables the distance scanner.
       */
      virtual void Enable() = 0;

      /**
       * @brief Disables the distance scanner.
       */
      virtual void Disable() = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   };

};

#endif
