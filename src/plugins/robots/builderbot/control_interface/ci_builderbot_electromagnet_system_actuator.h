/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_electromagnet_system_actuator.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_ELECTROMAGNET_SYSTEM_ACTUATOR_H
#define CCI_BUILDERBOT_ELECTROMAGNET_SYSTEM_ACTUATOR_H

namespace argos {
   class CCI_BuilderBotElectromagnetSystemActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_BuilderBotElectromagnetSystemActuator : public CCI_Actuator {

   public:

      enum class EDischargeMode : UInt8 {
         CONSTRUCTIVE = 0,
         DESTRUCTIVE = 1,
         DISABLED = 2,
      };

   public:

      CCI_BuilderBotElectromagnetSystemActuator() :
         m_eDischargeMode(EDischargeMode::DISABLED),
         m_bUpdateReq(false) {}

      virtual ~CCI_BuilderBotElectromagnetSystemActuator() {}

      virtual void SetDischargeMode(EDischargeMode e_discharge_mode) {
         m_eDischargeMode = e_discharge_mode;
         m_bUpdateReq = true;
      }


#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:
      EDischargeMode m_eDischargeMode;

      bool m_bUpdateReq;

   };

}

#endif
