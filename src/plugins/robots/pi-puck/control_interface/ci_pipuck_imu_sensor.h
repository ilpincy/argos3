/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_imu_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_PIPUCK_IMU_SENSOR_H
#define CCI_PIPUCK_IMU_SENSOR_H

namespace argos {
   class CCI_PiPuckImuSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CCI_PiPuckImuSensor : public CCI_Sensor {

   public:

      CCI_PiPuckImuSensor() {}
      
      virtual ~CCI_PiPuckImuSensor() {}
     
#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

      const CVector3& GetAccelerometer() {
         return m_cAccelerometer;
      }

      const CVector3& GetGryoscope() {
         return m_cGryoscope;
      }

      const CVector3& GetMagnetometer() {
         return m_cMagnetometer;
      }

   protected:

      CVector3 m_cAccelerometer;
      CVector3 m_cGryoscope;
      CVector3 m_cMagnetometer;
      
      
   };
}

#endif
