/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_altitude_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CCI_ALTITUDE_SENSOR_H
#define CCI_ALTITUDE_SENSOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_AltitudeSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

   class CCI_AltitudeSensor: public CCI_Sensor {

   public:

      CCI_AltitudeSensor();

      virtual ~CCI_AltitudeSensor() {}

      /**
       * Returns the current altitude.
       */
      Real GetReading() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      Real m_fReading;

   };

}

#endif
