/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_turret_torque_sensor.h>
 *
 * @brief This file provides the common interface definition of the footbot turret torque
 * sensor. The sensor provides measures of the torque and traction applied to the turret.
 *
 * The traction can be retrieved in XY form (a vector containing X and Y components)
 * or in polar form (force and angle).
 *
 * When expressed in X-Y form the convention is the following:
 *
 *
 *        front
 *
 *          ^ X
 *          |
 *          |
 * Y <------|
 *
 *        back
 *
 * Values of the X and Y components are in [-1,1].
 *
 * When expressed in the polar form, the force has to be considered as a traction (pulling) force.
 * The point at which it is applied is determined by its angle: counter-clockwise positive when looking from
 * above, values are expressed in radians and are in the interval [-pi,pi].
 * The magnitude of the force is in [0,1]
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CCI_FOOTBOT_TURRET_TORQUE_SENSOR_H
#define CCI_FOOTBOT_TURRET_TORQUE_SENSOR_H

namespace argos {
   class CCI_FootBotTurretTorqueSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector2.h>

namespace argos {

   class CCI_FootBotTurretTorqueSensor : virtual public CCI_Sensor {

   public:

      struct SReading {
         Real EngineTorque;
         CVector2 TractionXY;

         struct STractionPolar {
            Real Force;
            CRadians Angle;

            STractionPolar() :
               Force(0.0) {
            }

            STractionPolar(Real f_force,
                           const CRadians& c_angle) :
               Force(f_force),
               Angle(c_angle) {
            }

         } TractionPolar;

         SReading() :
            EngineTorque(0.0f) {
         }

         SReading(Real f_engine_torque,
                  const CVector2& c_traction_xy,
                  Real f_force,
                  const CRadians& c_angle) :
            EngineTorque(f_engine_torque),
            TractionXY(c_traction_xy),
            TractionPolar(f_force, c_angle) {
         }
      };

   public:

      static const CRange<Real>     RAW_FORCEXY_RANGE;
      static const CRange<Real>     NORMALIZED_FORCEXY_RANGE;
      static const CRange<Real>     RAW_POLAR_RANGE;
      static const CRange<Real>     NORMALIZED_POLAR_RANGE;
      static const CRange<Real>     RAW_FORCE_TORQUE_RANGE;
      static const CRange<Real>     NORMALIZED_TORQUE_RANGE;
      static const CRange<CRadians> ANGULAR_RANGE;

   public:

      virtual ~CCI_FootBotTurretTorqueSensor() {}

      inline const SReading& GetReading() const {
         return m_sReading;
      }

   protected:

      SReading m_sReading;

   };

}

#endif
