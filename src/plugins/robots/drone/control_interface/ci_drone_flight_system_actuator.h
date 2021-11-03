/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_flight_system_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Sinan Oguz - <soguz.ankara@gmail.com>
 */

#ifndef CCI_DRONE_FLIGHT_SYSTEM_ACTUATOR_H
#define CCI_DRONE_FLIGHT_SYSTEM_ACTUATOR_H

namespace argos {
   class CCI_DroneFlightSystemActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CCI_DroneFlightSystemActuator : public CCI_Actuator {

   public:

      CCI_DroneFlightSystemActuator() :
         m_cTargetPosition(CVector3::ZERO),
         m_cTargetYawAngle(0.0) {} 

      virtual ~CCI_DroneFlightSystemActuator() {}

      virtual void Reset() {
         m_cTargetPosition = CVector3::ZERO;
         m_cTargetYawAngle.SetValue(0.0);
      }

      void SetTargetPosition(const CVector3& c_position) {
          m_cTargetPosition = c_position;
      }

      void SetTargetYawAngle(Real f_yaw_angle) {
         m_cTargetYawAngle.SetValue(f_yaw_angle);
         m_cTargetYawAngle.SignedNormalize();
      }

      virtual bool Ready() = 0;

      virtual void Arm(bool b_arm, bool b_bypass_safety_checks) = 0;

      virtual void SetOffboardMode(bool b_offboard_mode) = 0;
      
#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:
      CVector3 m_cTargetPosition;
      CRadians m_cTargetYawAngle;
   };
 
}

#endif
