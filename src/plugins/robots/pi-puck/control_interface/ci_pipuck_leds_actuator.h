/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_leds_actuator.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_PIPUCK_LEDS_ACTUATOR_H
#define CCI_PIPUCK_LEDS_ACTUATOR_H

namespace argos {
   class CCI_PiPuckLEDsActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_PiPuckLEDsActuator : public CCI_Actuator {

   public:

      virtual ~CCI_PiPuckLEDsActuator() {}

      virtual void SetRingLEDIndex(UInt32 un_index, bool b_on) = 0;

      virtual void SetRingLEDs(bool b_on) = 0;

      virtual void SetFrontLED(bool b_on) = 0;

      virtual void SetBodyLED(bool b_on) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:
      const static UInt32 NUM_LEDS = 10;
      const static UInt32 NUM_RING_LEDS = 8;
      const static UInt32 INDEX_FRONT_LED = 8;
      const static UInt32 INDEX_BODY_LED = 9;

   };

}

#endif
