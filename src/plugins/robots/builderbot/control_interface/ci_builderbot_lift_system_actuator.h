/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_lift_system_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_LIFT_SYSTEM_ACTUATOR_H
#define CCI_BUILDERBOT_LIFT_SYSTEM_ACTUATOR_H

namespace argos {
   class CCI_BuilderBotLiftSystemActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_BuilderBotLiftSystemActuator : public CCI_Actuator {

   public:

      CCI_BuilderBotLiftSystemActuator() :
         m_bStopReq(false),
         m_bCalibrationReq(false),
         m_bSetPositionReq(false),
         m_fTargetPosition(0.0f) {}

      virtual ~CCI_BuilderBotLiftSystemActuator() {}

      virtual void SetPosition(Real f_position) {
         m_fTargetPosition = f_position;
         m_bSetPositionReq = true;
      }

      virtual void Stop() {
         m_bStopReq = true;
      }

      virtual void Calibrate() {
         m_bCalibrationReq = true;
      }

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:
      bool m_bStopReq;
      bool m_bCalibrationReq;
      bool m_bSetPositionReq;

      Real m_fTargetPosition;
   };

}

#endif
