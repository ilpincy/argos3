/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_system_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CCI_DRONE_SYSTEM_SENSOR_H
#define CCI_DRONE_SYSTEM_SENSOR_H

#include <array>

namespace argos {
   class CCI_DroneSystemSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_DroneSystemSensor : public CCI_Sensor {

   public:

      CCI_DroneSystemSensor() :
         m_fTime(0.0),
         m_arrTemperatures{0.0, 0.0, 0.0, 0.0} {}
      
      virtual ~CCI_DroneSystemSensor() {}
     
#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      Real m_fTime;
      std::array<Real, 4> m_arrTemperatures;
      
   };
}

#endif
