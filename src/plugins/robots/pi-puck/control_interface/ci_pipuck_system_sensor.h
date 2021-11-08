/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_system_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_PIPUCK_SYSTEM_SENSOR_H
#define CCI_PIPUCK_SYSTEM_SENSOR_H

namespace argos {
   class CCI_PiPuckSystemSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_PiPuckSystemSensor : public CCI_Sensor {

   public:

      CCI_PiPuckSystemSensor() :
         m_fTime(0.0f),
         m_fTemperature(0.0f) {}
      
      virtual ~CCI_PiPuckSystemSensor() {}
     
#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      Real m_fTime;
      Real m_fTemperature;
      
   };
}

#endif
