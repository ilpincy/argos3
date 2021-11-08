/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_flight_system_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Sinan Oguz - <soguz.ankara@gmail.com>
 */

#ifndef CCI_DRONE_FLIGHT_SYSTEM_SENSOR_H
#define CCI_DRONE_FLIGHT_SYSTEM_SENSOR_H

namespace argos {
   class CCI_DroneFlightSystemSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CCI_DroneFlightSystemSensor : public CCI_Sensor {

   public:

      CCI_DroneFlightSystemSensor() :
         m_cPosition(CVector3::ZERO),
         m_cTargetPosition(CVector3::ZERO),
         m_cOrientation(CVector3::ZERO),
         m_cTargetOrientation(CVector3::ZERO),
         m_cVelocity(CVector3::ZERO),
         m_cAngularVelocity(CVector3::ZERO),
         m_cAccelerometer(CVector3::ZERO),
         m_cMagnetometer(CVector3::ZERO),
         m_cGyroscope(CVector3::ZERO),
         m_fBatteryVoltage(0.0),
         m_fHeight(0.0),
         m_fTemperature(0.0) {}
      
      virtual ~CCI_DroneFlightSystemSensor() {}
      
      virtual void Reset() {
         m_cPosition = CVector3::ZERO;
         m_cTargetPosition = CVector3::ZERO;
         m_cOrientation = CVector3::ZERO;
         m_cTargetOrientation = CVector3::ZERO;
         m_cVelocity = CVector3::ZERO;
         m_cAngularVelocity = CVector3::ZERO;
         m_cAccelerometer = CVector3::ZERO;
         m_cMagnetometer = CVector3::ZERO;
         m_cGyroscope = CVector3::ZERO;
         m_fBatteryVoltage = 0.0;
         m_fHeight = 0.0;
         m_fTemperature = 0.0;
      }

      const CVector3& GetPosition() const {
         return m_cPosition;
      }

      const CVector3& GetTargetPosition() const {
         return m_cTargetPosition;
      }

      const CVector3& GetOrientation() const {
         return m_cOrientation;
      }

      const CVector3& GetTargetOrientation() const {
         return m_cTargetOrientation;
      }

      const CVector3& GetVelocity() const {
         return m_cVelocity;
      }

      const CVector3& GetAngularVelocity() const {
         return m_cAngularVelocity;
      }

      const Real& GetBatteryVoltage() const {
         return m_fBatteryVoltage;
      }

      const Real& GetHeight() const {
         return m_fHeight;
      }

      const Real& GetTemperature() const {
         return m_fTemperature;
      }

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:
      CVector3 m_cPosition;
      CVector3 m_cTargetPosition;
      CVector3 m_cOrientation;
      CVector3 m_cTargetOrientation;
      CVector3 m_cVelocity;
      CVector3 m_cAngularVelocity; 
      CVector3 m_cAccelerometer;
      CVector3 m_cMagnetometer;
      CVector3 m_cGyroscope;
      Real m_fBatteryVoltage;
      Real m_fHeight;
      Real m_fTemperature;
   };
}

#endif
