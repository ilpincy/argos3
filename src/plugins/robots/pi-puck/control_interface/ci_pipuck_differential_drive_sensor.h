/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_differential_drive_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_PIPUCK_DIFFERENTIAL_DRIVE_SENSOR_H
#define CCI_PIPUCK_DIFFERENTIAL_DRIVE_SENSOR_H

namespace argos {
   class CCI_PiPuckDifferentialDriveSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_PiPuckDifferentialDriveSensor : public CCI_Sensor {

   public:

      virtual ~CCI_PiPuckDifferentialDriveSensor() {}

      virtual Real GetLeftVelocity() = 0;

      virtual Real GetRightVelocity() = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif
   };

}

#endif
