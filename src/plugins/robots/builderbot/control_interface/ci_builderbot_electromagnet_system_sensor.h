/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_electromagnet_system_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_ELECTROMAGNET_SYSTEM_SENSOR_H
#define CCI_BUILDERBOT_ELECTROMAGNET_SYSTEM_SENSOR_H

namespace argos {
   class CCI_BuilderBotElectromagnetSystemSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_BuilderBotElectromagnetSystemSensor : public CCI_Sensor {

   public:

      CCI_BuilderBotElectromagnetSystemSensor() :
         m_fVoltage(0.0f) {}

      virtual ~CCI_BuilderBotElectromagnetSystemSensor() {}

      Real GetVoltage() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      Real m_fVoltage;

   };

}

#endif
