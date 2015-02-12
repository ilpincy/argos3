/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_quadrotor_speed_actuator.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef CI_QUADROTOR_SPEED_ACTUATOR_H
#define CI_QUADROTOR_SPEED_ACTUATOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_QuadRotorSpeedActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CCI_QuadRotorSpeedActuator : public CCI_Actuator {

   public:

      virtual ~CCI_QuadRotorSpeedActuator() {}

      /**
       * Sets the linear velocity of the quadrotor.
       * @param c_velocity The desired linear velocity.
       */
      virtual void SetLinearVelocity(const CVector3& c_velocity) = 0;

      /**
       * Sets the rotational velocity of the quadrotor around the local Z axis (yaw).
       * @param c_velocity The desired rotational velocity.
       */
      virtual void SetRotationalSpeed(const CRadians& c_speed) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   };

}

#endif
