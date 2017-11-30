/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_battery_sensor.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#ifndef CCI_BATTERY_SENSOR_H
#define CCI_BATTERY_SENSOR_H

namespace argos {
   class CCI_BatterySensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_BatterySensor : public CCI_Sensor {

   public:

      struct SReading {
         /** Available battery level is always between 0 and 1 */
         Real Level;
         /** Available capacity in mAh*/
         UInt16 AvailCapacity;
         /** Time remaining in seconds */
         UInt16 TimeRemaining;
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

      void SetReading(Real f_bat_level, SInt16 n_bat_avail_capacity, Real f_time_rem);

   };

}

#endif
