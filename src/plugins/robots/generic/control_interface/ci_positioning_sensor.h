/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef CCI_POSITIONING_SENSOR_H
#define CCI_POSITIONING_SENSOR_H

namespace argos {
   class CCI_PositioningSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CCI_PositioningSensor : public CCI_Sensor {

   public:

      struct SReading {
         CVector3 Position;
         CQuaternion Orientation;
      };

   public:

      virtual ~CCI_PositioningSensor() {}

      const SReading& GetReading() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      SReading m_sReading;
      CRadians m_cAngle;
      CVector3 m_cAxis;

   };

}

#endif
