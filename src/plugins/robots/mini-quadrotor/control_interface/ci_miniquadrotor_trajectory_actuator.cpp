/**
 * @file <argos3/plugins/robots/mini-quadrotor/control_interface/ci_miniquadrotor_trajectory_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_miniquadrotor_trajectory_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/

   CCI_MiniQuadrotorTrajectoryActuator::SWaypoint::SWaypoint() {}

   /****************************************/
   /****************************************/

   CCI_MiniQuadrotorTrajectoryActuator::SWaypoint::SWaypoint(const CVector3& c_position,
                                                             const CRadians& c_yaw) :
      Position(c_position),
      Yaw(c_yaw) {}

   /****************************************/
   /****************************************/

   void CCI_MiniQuadrotorTrajectoryActuator::CreateLuaState(lua_State* pt_lua_state) {
   }

   /****************************************/
   /****************************************/

}
