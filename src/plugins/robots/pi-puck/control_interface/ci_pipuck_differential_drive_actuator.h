/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_differential_drive_actuator.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_PIPUCK_DIFFERENTIAL_DRIVE_ACTUATOR_H
#define CCI_PIPUCK_DIFFERENTIAL_DRIVE_ACTUATOR_H

namespace argos {
   class CCI_PiPuckDifferentialDriveActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_PiPuckDifferentialDriveActuator : public CCI_Actuator {

   public:

      virtual ~CCI_PiPuckDifferentialDriveActuator() {}

      virtual void SetTargetVelocityLeft(Real f_target_velocity_left) = 0;

      virtual void SetTargetVelocityRight(Real f_target_velocity_left) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif
   };

}

#endif
