/**
 * @file <argos3/plugins/robots/mini_quadrotor/control_interface/ci_miniquadrotor_rotor_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_miniquadrotor_rotor_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/

   CCI_MiniQuadrotorRotorActuator::SVelocities::SVelocities() {
      Velocities[0] = 0.0f;
      Velocities[1] = 0.0f;
      Velocities[2] = 0.0f;
      Velocities[3] = 0.0f;
   }

   /****************************************/
   /****************************************/

   void CCI_MiniQuadrotorRotorActuator::CreateLuaState(lua_State* pt_lua_state) {
      // TODO
   }

   /****************************************/
   /****************************************/

}
