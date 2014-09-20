/**
 * @file <argos3/plugins/robots/mini_quadrotor/control_interface/ci_miniquadrotor_rotor_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_miniquadrotor_rotor_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

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

   CCI_MiniQuadrotorRotorActuator::SVelocities::SVelocities(Real f_north_vel,
                                                            Real f_west_vel,
                                                            Real f_south_vel,
                                                            Real f_east_vel) {
      Velocities[0] = f_north_vel;
      Velocities[1] = f_west_vel;
      Velocities[2] = f_south_vel;
      Velocities[3] = f_east_vel;
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

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have four values in this order:
    * 1. north rotor velocity (a number)
    * 2. west rotor velocity (a number)
    * 3. south rotor velocity (a number)
    * 4. east rotor velocity (a number)
    */
   int LuaSetVelocity(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 4) {
         return luaL_error(pt_lua_state, "robot.rotors.set_velocity() expects 4 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 3, LUA_TNUMBER);
      luaL_checktype(pt_lua_state, 4, LUA_TNUMBER);
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_MiniQuadrotorRotorActuator>(pt_lua_state, "rotors")->
         SetRotorVelocities(CCI_MiniQuadrotorRotorActuator::SVelocities(
                               lua_tonumber(pt_lua_state, 1),
                               lua_tonumber(pt_lua_state, 2),
                               lua_tonumber(pt_lua_state, 3),
                               lua_tonumber(pt_lua_state, 4)));
      return 0;
   }
#endif
   
   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_MiniQuadrotorRotorActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "rotors");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "set_velocity", &LuaSetVelocity);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

}
