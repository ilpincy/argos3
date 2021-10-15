/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_system_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_SYSTEM_SENSOR_H
#define CCI_BUILDERBOT_SYSTEM_SENSOR_H

namespace argos {
   class CCI_BuilderBotSystemSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_BuilderBotSystemSensor : public CCI_Sensor {

   public:

      CCI_BuilderBotSystemSensor() :
         m_fTime(0.0f),
         m_fTemperature(0.0f) {}
      
      virtual ~CCI_BuilderBotSystemSensor() {}
     
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
