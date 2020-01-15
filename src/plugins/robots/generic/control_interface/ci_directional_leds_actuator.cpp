/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_directional_leds_actuator.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_directional_leds_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * This function expects the stack to have either two or four arguments.
    * The first argument must always be the index of the LED to set.
    * Then, in case two arguments are passed, the second argument can be the string
    * definition of a color. In case of four arguments, the RGB values are expected.
    */
   int LuaDirectionalLEDSetSingleColor(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 2 && lua_gettop(pt_lua_state) != 4) {
         return luaL_error(pt_lua_state, "robot.directional_leds.set_single_color() expects 2 or 4 arguments");
      }
      luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
      size_t unIdx = lua_tonumber(pt_lua_state, 1);
      /* Get reference to actuator */
      CCI_DirectionalLEDsActuator* pcAct = 
         CLuaUtility::GetDeviceInstance<CCI_DirectionalLEDsActuator>(pt_lua_state, "directional_leds");
      if(unIdx < 1 || unIdx > pcAct->GetNumLEDs()) {
         return luaL_error(pt_lua_state, "passed index %d out of bounds [1,%d]", unIdx, pcAct->GetNumLEDs());
      }
      /* Create color buffer */
      CColor cColor;
      if(lua_gettop(pt_lua_state) == 2) {
         luaL_checktype(pt_lua_state, 2, LUA_TSTRING);
         try {
            cColor.Set(lua_tostring(pt_lua_state, 2));
         }
         catch(CARGoSException& ex) {
            return luaL_error(pt_lua_state, ex.what());
         }
      }
      else {
         luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 3, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 4, LUA_TNUMBER);
         cColor.Set(lua_tonumber(pt_lua_state, 2),
                    lua_tonumber(pt_lua_state, 3),
                    lua_tonumber(pt_lua_state, 4));
      }
      /* Perform action */
      pcAct->SetSingleColor(unIdx - 1, cColor);
      return 0;
   }

   /*
    * This function expects the stack to have either one or three arguments.
    * In case one argument is passed, it must be the string definition of a color.
    * In case of three arguments, the RGB values are expected.
    */
   int LuaDirectionalLEDSetAllColors(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 1 && lua_gettop(pt_lua_state) != 3) {
         return luaL_error(pt_lua_state, "robot.directional_leds.set_all_colors() expects 1 or 3 arguments");
      }
      /* Create color buffer */
      CColor cColor;
      if(lua_gettop(pt_lua_state) == 1) {
         luaL_checktype(pt_lua_state, 1, LUA_TSTRING);
         try {
            cColor.Set(lua_tostring(pt_lua_state, 1));
         }
         catch(CARGoSException& ex) {
            return luaL_error(pt_lua_state, ex.what());
         }
      }
      else {
         luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 3, LUA_TNUMBER);
         cColor.Set(lua_tonumber(pt_lua_state, 1),
                    lua_tonumber(pt_lua_state, 2),
                    lua_tonumber(pt_lua_state, 3));
      }
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_DirectionalLEDsActuator>(pt_lua_state, "directional_leds")->
         SetAllColors(cColor);
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

   size_t CCI_DirectionalLEDsActuator::GetNumLEDs() const {
     return m_tSettings.size();
   }

   /****************************************/
   /****************************************/

   void CCI_DirectionalLEDsActuator::SetSingleColor(UInt32 un_led_number,
                                                    const CColor& c_color) {
      m_tSettings[un_led_number] = c_color;
   }
      
   /****************************************/
   /****************************************/

   void CCI_DirectionalLEDsActuator::SetAllColors(const CColor& c_color) {
      for(size_t i = 0; i < m_tSettings.size(); ++i) {
         m_tSettings[i] = c_color;
      }
   }

   /****************************************/
   /****************************************/

   void CCI_DirectionalLEDsActuator::SetAllColors(const std::vector<CColor>& c_colors) {
      m_tSettings = c_colors;
   }

   /****************************************/
   /****************************************/

   void CCI_DirectionalLEDsActuator::SetSingleIntensity(UInt32 un_led_number,
                                                        UInt8 un_intensity) {
      m_tSettings[un_led_number].SetAlpha(un_intensity);
   }

   /****************************************/
   /****************************************/

   void CCI_DirectionalLEDsActuator::SetAllIntensities(UInt8 un_intensity) {
      for(size_t i = 0; i < m_tSettings.size(); ++i) {
         m_tSettings[i].SetAlpha(un_intensity);
      }
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_DirectionalLEDsActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "directional_leds");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "set_single_color", &LuaDirectionalLEDSetSingleColor);
      CLuaUtility::AddToTable(pt_lua_state, "set_all_colors", &LuaDirectionalLEDSetAllColors);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

}
