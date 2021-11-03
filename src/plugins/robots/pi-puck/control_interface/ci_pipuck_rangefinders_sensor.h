/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_rangefinders_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_PIPUCK_RANGEFINDERS_SENSOR_H
#define CCI_PIPUCK_RANGEFINDERS_SENSOR_H

namespace argos {
   class CCI_PiPuckRangefindersSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

#include <map>
#include <tuple>
#include <functional>

namespace argos {

   class CCI_PiPuckRangefindersSensor : public CCI_Sensor {

   public:

      using TConfiguration = std::tuple<const char*, CVector3, CQuaternion, Real>;

      struct SInterface {
         SInterface(const UInt8& un_label) :
            Label(un_label),
            Configuration(MAP_SENSOR_CONFIG.at(un_label)),
            Proximity(0.0),
            Illuminance(0.0) {}
         const UInt8& Label;
         const TConfiguration& Configuration;
         Real Proximity;
         Real Illuminance;
         
      };

      virtual ~CCI_PiPuckRangefindersSensor() {}

      virtual void Visit(std::function<void(const SInterface&)>) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      static const std::map<UInt8, TConfiguration> MAP_SENSOR_CONFIG;

   };

}

#endif
