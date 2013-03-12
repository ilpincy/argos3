/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_gripper_actuator.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "ci_gripper_actuator.h"
#include <argos3/core/utility/math/range.h>
#include <argos3/core/wrappers/lua/lua_utility.h>

namespace argos {

   /****************************************/
   /****************************************/

   CRange<Real> UNIT(0.0f, 1.0f);

   /****************************************/
   /****************************************/

   /*
    * The stack must have no values
    */
   int LuaGripperLock(lua_State* pt_lua_state) {
      /* Get wheel speed from stack */
      if(lua_gettop(pt_lua_state) != 0) {
         return luaL_error(pt_lua_state, "robot.gripper.lock() expects no arguments");
      }
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_GripperActuator>(pt_lua_state, "gripper")->Lock();
      return 0;
   }

   /*
    * The stack must have no values
    */
   int LuaGripperUnlock(lua_State* pt_lua_state) {
      /* Get wheel speed from stack */
      if(lua_gettop(pt_lua_state) != 0) {
         return luaL_error(pt_lua_state, "robot.gripper.unlock() expects no arguments");
      }
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_GripperActuator>(pt_lua_state, "gripper")->Unlock();
      return 0;
   }

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
   void CCI_GripperActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "gripper");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "lock", &LuaGripperLock);
      CLuaUtility::AddToTable(pt_lua_state, "unlock", &LuaGripperUnlock);
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

}
