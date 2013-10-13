/**
 * @file <argos3/plugins/robots/mini_quadrotor/control_interface/ci_miniquadrotor_trajectory_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef MINIQUADROTOR_TRAJECTORY_ACTUATOR_H
#define MINIQUADROTOR_TRAJECTORY_ACTUATOR_H

namespace argos {
   class CCI_MiniQuadrotorTrajectoryActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CCI_MiniQuadrotorTrajectoryActuator : public CCI_Actuator {

   public:

      struct SWaypoint {
         CVector3 Position;
         CRadians Yaw;

         SWaypoint();
         SWaypoint(const CVector3& c_position,
                   const CRadians& c_yaw);
      };

   public:

      virtual ~CCI_MiniQuadrotorTrajectoryActuator() {}

      virtual void SetWaypoint(SWaypoint& s_waypoint) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   };

}

#endif
