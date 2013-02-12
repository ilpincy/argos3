
/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "ci_leds_actuator.h"

namespace argos {

   /****************************************/
   /****************************************/

   void CCI_LEDsActuator::SetSingleColor(UInt32 un_led_number,
                                         const CColor& c_color) {
      m_tSettings[un_led_number] = c_color;
   }
      
   /****************************************/
   /****************************************/

   void CCI_LEDsActuator::SetAllColors(const CColor& c_color) {
      for(size_t i = 0; i < m_tSettings.size(); ++i) {
         m_tSettings[i] = c_color;
      }
   }

   /****************************************/
   /****************************************/

   void CCI_LEDsActuator::SetAllColors(const TSettings& c_colors) {
      m_tSettings = c_colors;
   }

   /****************************************/
   /****************************************/

   void CCI_LEDsActuator::SetSingleIntensity(UInt32 un_led_number,
                                             UInt8 un_intensity) {
      m_tSettings[un_led_number].SetAlpha(un_intensity);
   }

   /****************************************/
   /****************************************/

   void CCI_LEDsActuator::SetAllIntensities(UInt8 un_intensity) {
      for(size_t i = 0; i < m_tSettings.size(); ++i) {
         m_tSettings[i].SetAlpha(un_intensity);
      }
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_LEDsActuator::CreateLuaVariables(lua_State* pt_lua_state) {
      lua_pushstring(pt_lua_state, "leds");
      lua_newtable  (pt_lua_state);
      for(size_t i = 0; i < GetNumLEDs(); ++i) {
         lua_pushnumber(pt_lua_state,     i+1);
         lua_newtable  (pt_lua_state         );
         lua_pushstring(pt_lua_state,   "red");
         lua_pushnumber(pt_lua_state,       0);
         lua_settable  (pt_lua_state,      -3);
         lua_pushstring(pt_lua_state, "green");
         lua_pushnumber(pt_lua_state,       0);
         lua_settable  (pt_lua_state,      -3);
         lua_pushstring(pt_lua_state,  "blue");
         lua_pushnumber(pt_lua_state,       0);
         lua_settable  (pt_lua_state,      -3);
         lua_settable  (pt_lua_state,      -3);
      }
      lua_settable(pt_lua_state, -3);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_LEDsActuator::LuaVariablesToSettings(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "leds");
      for(size_t i = 0; i < GetNumLEDs(); ++i) {
         lua_pushnumber(pt_lua_state, i+1);
         lua_gettable  (pt_lua_state, -2);
         lua_getfield  (pt_lua_state, -1, "red");
         lua_getfield  (pt_lua_state, -2, "green");
         lua_getfield  (pt_lua_state, -3, "blue");
         m_tSettings[i].Set(lua_tonumber(pt_lua_state, -3),
                            lua_tonumber(pt_lua_state, -2),
                            lua_tonumber(pt_lua_state, -1));
         lua_pop(pt_lua_state, 4);
      }
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}
