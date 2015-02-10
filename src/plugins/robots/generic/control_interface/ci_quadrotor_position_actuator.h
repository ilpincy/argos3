/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_quadrotor_position_actuator.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef CI_QUADROTOR_POSITION_ACTUATOR_H
#define CI_QUADROTOR_POSITION_ACTUATOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_QuadRotorPositionActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CCI_QuadRotorPositionActuator : public CCI_Actuator {

   public:

      virtual ~CCI_QuadRotorPositionActuator() {}

      /**
       * Sets the absolute position of the robot in the environment.
       * @param c_pos The desired absolute position.
       */
      virtual void SetAbsolutePosition(const CVector3& c_pos) = 0;

      /**
       * Sets the position of the robot in the environment relative to the current position and attitude.
       * @param c_pos The desired relative position.
       */
      virtual void SetRelativePosition(const CVector3& c_pos) = 0;

      /**
       * Sets the absolute yaw of the robot in the world.
       * @param c_orient The desired absulte yaw.
       */
      virtual void SetAbsoluteYaw(const CRadians& c_yaw) = 0;

      /**
       * Sets the yaw of the robot in the environment relative to the current position and attitude.
       * @param c_orient The desired relative yaw.
       */
      virtual void SetRelativeYaw(const CRadians& c_yaw) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   };

}

#endif
