/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_ground_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_PIPUCK_GROUND_SENSOR_H
#define CCI_PIPUCK_GROUND_SENSOR_H

namespace argos {
   class CCI_PiPuckGroundSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

#include <map>
#include <tuple>
#include <functional>

namespace argos {

   class CCI_PiPuckGroundSensor : public CCI_Sensor {

   public:

      using TConfiguration = std::tuple<std::string, CVector3, CQuaternion, Real>;

      struct SInterface {
         /* constructor */
         SInterface(UInt8 un_label) :
            Label(un_label),
            Configuration(MAP_SENSOR_CONFIG.at(un_label)),
            Reflected(0.0),
            Background(0.0) {}
         /* members */
         const UInt8 Label;
         const TConfiguration& Configuration;
         Real Reflected;
         Real Background;
      };

   public:

      virtual ~CCI_PiPuckGroundSensor() {}

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
