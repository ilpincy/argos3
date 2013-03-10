/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_gripper_actuator.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#ifndef CCI_GRIPPER_ACTUATOR_H
#define CCI_GRIPPER_ACTUATOR_H

namespace argos {
   class CCI_GripperActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_GripperActuator : public CCI_Actuator {

   public:

      /**
       * Class constructor.
       */
      CCI_GripperActuator();

      /**
       * Class destructor.
       */
      virtual ~CCI_GripperActuator() {}

      /**
       * Sets the gripper lock state.
       * The lock state is a value between 0 and 1, where 0 means
       * unlocked, and 1 means locked.
       * @param f_lock_state The desired lock state.
       */
      void SetLockState(Real f_lock_state);

      /**
       * Locks the gripper.
       * This corresponds to calling SetLockState(1.0f).
       * @see SetLockState()
       */
      inline void Lock() {
         SetLockState(1.0f);
      }

      /**
       * Unlocks the gripper.
       * This corresponds to calling SetLockState(0.0f).
       * @see SetLockState()
       */
      inline void Unlock() {
         SetLockState(0.0f);
      }

#ifdef ARGOS_WITH_LUA
      /**
       * Creates the Lua variables for this actuator.
       * The variables must be added to the <tt>robot</tt> table, as a nested table.
       * @param pt_lua_state The current Lua state.
       * @see LuaVariablesToSettings()
       */
      virtual void CreateLuaVariables(lua_State* pt_lua_state);

      /**
       * Reads the value of the Lua variables and sets the actuator values accordingly.
       * @param pt_lua_state The current Lua state.
       * @see CreateLuaVariables()
       */
      virtual void LuaVariablesToSettings(lua_State* pt_lua_state);
#endif

   protected:

      Real m_fLockState;

   };

}

#endif
