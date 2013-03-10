/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_gripper_actuator.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "ci_gripper_actuator.h"
#include <argos3/core/utility/math/range.h>

namespace argos {

   /****************************************/
   /****************************************/

   CRange<Real> UNIT(0.0f, 1.0f);

   /****************************************/
   /****************************************/

   CCI_GripperActuator::CCI_GripperActuator() :
      m_fLockState(0.0f) {}

   /****************************************/
   /****************************************/

   void CCI_GripperActuator::SetLockState(Real f_lock_state) {
      UNIT.TruncValue(f_lock_state);
      m_fLockState = f_lock_state;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_GripperActuator::CreateLuaVariables(lua_State* pt_lua_state) {
      lua_pushstring(pt_lua_state, "gripper");
      lua_newtable  (pt_lua_state);
      lua_pushstring(pt_lua_state, "lock_state");
      lua_pushnumber(pt_lua_state, m_fLockState);
      lua_settable  (pt_lua_state, -3);
      lua_settable  (pt_lua_state, -3);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_GripperActuator::LuaVariablesToSettings(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "gripper");
      lua_getfield(pt_lua_state, -1, "lock_state");
      SetLockState(lua_tonumber(pt_lua_state, -1));
      lua_pop(pt_lua_state, 2);
   }
#endif

   /****************************************/
   /****************************************/

}
