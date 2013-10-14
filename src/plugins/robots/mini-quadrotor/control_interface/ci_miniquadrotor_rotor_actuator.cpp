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

   CCI_MiniQuadrotorRotorActuator::SVelocities::SVelocities(const SVelocities& s_velocities) {
      Velocities[0] = s_velocities.Velocities[0];
      Velocities[1] = s_velocities.Velocities[1];
      Velocities[2] = s_velocities.Velocities[2];
      Velocities[3] = s_velocities.Velocities[3];
   }

   /****************************************/
   /****************************************/

   CCI_MiniQuadrotorRotorActuator::SVelocities& CCI_MiniQuadrotorRotorActuator::SVelocities::operator=(const CCI_MiniQuadrotorRotorActuator::SVelocities& s_velocities) {
      if(&s_velocities != this) {
         Velocities[0] = s_velocities.Velocities[0];
         Velocities[1] = s_velocities.Velocities[1];
         Velocities[2] = s_velocities.Velocities[2];
         Velocities[3] = s_velocities.Velocities[3];
      }
      return *this;
   }

   /****************************************/
   /****************************************/

   void CCI_MiniQuadrotorRotorActuator::CreateLuaState(lua_State* pt_lua_state) {
      // TODO
   }

   /****************************************/
   /****************************************/

}
