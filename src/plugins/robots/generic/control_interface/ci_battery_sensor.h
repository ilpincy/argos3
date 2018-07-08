/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_battery_sensor.h>
 *
 * @author Adhavan Jayabalan <jadhu94@gmail.com>
 */

#ifndef CCI_BATTERY_SENSOR_H
#define CCI_BATTERY_SENSOR_H

namespace argos {
   class CCI_BatterySensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <limits>

namespace argos {

   class CCI_BatterySensor : public CCI_Sensor {

   public:

      struct SReading {
         /** Available charge, between 0 and 1 */
         Real AvailableCharge;
         /** Time left in seconds */
         Real TimeLeft;

         SReading() :
            AvailableCharge(1.0),
            TimeLeft(std::numeric_limits<Real>::infinity()) {}
      };

   public:

      virtual ~CCI_BatterySensor() {}

      const SReading& GetReading() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      SReading m_sReading;
   };

}

#endif
