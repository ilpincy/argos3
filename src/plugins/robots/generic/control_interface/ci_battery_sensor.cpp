/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_battery_sensor.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "ci_battery_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   const CCI_BatterySensor::SReading& CCI_BatterySensor::GetReading() const {
     return m_sReading;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BatterySensor::CreateLuaState(lua_State* pt_lua_state) {
       /** TODO */
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BatterySensor::ReadingsToLuaState(lua_State* pt_lua_state) {
       /** TODO */
   }
#endif

   /****************************************/
   /****************************************/

   void CCI_BatterySensor::SetReading(Real f_bat_level, SInt16 n_bat_avail_capacity, Real f_time_rem) {
       m_sReading.Level = f_bat_level;
       m_sReading.AvailCapacity = n_bat_avail_capacity;
       m_sReading.TimeRemaining = f_time_rem;
   }

   /****************************************/
   /****************************************/
}
