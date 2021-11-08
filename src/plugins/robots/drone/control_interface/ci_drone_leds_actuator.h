/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_leds_actuator.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_DRONE_LEDS_ACTUATOR_H
#define CCI_DRONE_LEDS_ACTUATOR_H

namespace argos {
   class CCI_DroneLEDsActuator;
   class CColor;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_DroneLEDsActuator : public CCI_Actuator {

   public:

      virtual ~CCI_DroneLEDsActuator() {}

      virtual void SetLEDIndex(UInt32 un_index, const CColor& c_color) = 0;

      virtual void SetLEDs(const CColor& c_color) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:
      const static UInt32 NUM_LEDS = 4;

   };

}

#endif
