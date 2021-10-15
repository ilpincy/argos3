/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_leds_actuator.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_LEDS_ACTUATOR_H
#define CCI_BUILDERBOT_LEDS_ACTUATOR_H

namespace argos {
   class CCI_BuilderBotLEDsActuator;
   class CColor;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_BuilderBotLEDsActuator : public CCI_Actuator {

   public:

      virtual ~CCI_BuilderBotLEDsActuator() {}

      virtual void SetLEDIndex(UInt32 un_index, const CColor& c_color) = 0;

      virtual void SetLEDs(const CColor& c_color) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:
      const static UInt32 NUM_LEDS = 12;

   };

}

#endif
