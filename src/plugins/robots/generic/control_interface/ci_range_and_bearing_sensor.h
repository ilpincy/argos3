/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#ifndef CI_RANGE_AND_BEARING_SENSOR_H
#define CI_RANGE_AND_BEARING_SENSOR_H

namespace argos {
   class CCI_RangeAndBearingSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   /****************************************/
   /****************************************/

   class CCI_RangeAndBearingSensor : public CCI_Sensor {

   public:

      struct SPacket {
         Real Range;
         CRadians HorizontalBearing;
         CRadians VerticalBearing;
         CByteArray Data;

         SPacket();
      };

      typedef std::vector<SPacket> TReadings;

   public:

      virtual ~CCI_RangeAndBearingSensor() {}

      inline const TReadings& GetReadings() const {
         return m_tReadings;
      }

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      TReadings m_tReadings;

   };

   /****************************************/
   /****************************************/

}

#endif
