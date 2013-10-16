/**
 * @file <argos3/plugins/robots/mini_quadrotor/control_interface/ci_miniquadrotor_rotor_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef MINIQUADROTOR_ROTOR_ACTUATOR_H
#define MINIQUADROTOR_ROTOR_ACTUATOR_H

namespace argos {
   class CCI_MiniQuadrotorRotorActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_MiniQuadrotorRotorActuator : public CCI_Actuator {

   public:

      struct SVelocities {
         Real Velocities[4];

         SVelocities();
         SVelocities(Real f_north_vel,
                     Real f_west_vel,
                     Real f_south_vel,
                     Real f_east_vel);
         SVelocities(const SVelocities& s_velocities);
         SVelocities& operator=(const SVelocities& s_velocities);
      };

   public:

      virtual ~CCI_MiniQuadrotorRotorActuator() {}

      virtual void SetRotorVelocities(const SVelocities& s_velocities) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      SVelocities m_sCurrentVelocities;

   };

}

#endif
