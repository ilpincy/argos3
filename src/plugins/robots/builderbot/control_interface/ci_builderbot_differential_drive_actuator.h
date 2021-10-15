/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_differential_drive_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_DIFFERENTIAL_DRIVE_ACTUATOR_H
#define CCI_BUILDERBOT_DIFFERENTIAL_DRIVE_ACTUATOR_H

namespace argos {
   class CCI_BuilderBotDifferentialDriveActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_BuilderBotDifferentialDriveActuator : public CCI_Actuator {

   public:

      virtual ~CCI_BuilderBotDifferentialDriveActuator() {}

      virtual void SetTargetVelocityLeft(Real f_target_velocity_left) = 0;

      virtual void SetTargetVelocityRight(Real f_target_velocity_left) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif
   };

}

#endif
