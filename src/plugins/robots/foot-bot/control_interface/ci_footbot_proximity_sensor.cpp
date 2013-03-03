/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_footbot_proximity_sensor.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "ci_footbot_proximity_sensor.h"
#include <argos3/core/utility/math/angles.h>

namespace argos {

   /****************************************/
   /****************************************/

   static CRadians SPACING = CRadians(ARGOS_PI / 12.0f);
   static CRadians START_ANGLE = SPACING * 0.5f;

   /****************************************/
   /****************************************/

   CCI_FootBotProximitySensor::CCI_FootBotProximitySensor() :
      m_tReadings(24) {
      for(size_t i = 0; i < 24; ++i) {
         m_tReadings[i].Angle = START_ANGLE + i * SPACING;
      }
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_FootBotProximitySensor::CreateLuaVariables(lua_State* pt_lua_state) {
      lua_pushstring(pt_lua_state, "proximity");
      lua_newtable  (pt_lua_state);
      for(size_t i = 0; i < GetReadings().size(); ++i) {
         lua_pushnumber(pt_lua_state, i+1                            );
         lua_newtable  (pt_lua_state                                 );
         lua_pushstring(pt_lua_state, "angle"                        );
         lua_pushnumber(pt_lua_state, m_tReadings[i].Angle.GetValue());
         lua_settable  (pt_lua_state, -3                             );
         lua_pushstring(pt_lua_state, "value"                        );
         lua_pushnumber(pt_lua_state, m_tReadings[i].Value           );
         lua_settable  (pt_lua_state, -3                             );
         lua_settable  (pt_lua_state, -3                             );
      }
      lua_settable(pt_lua_state, -3);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_FootBotProximitySensor::ReadingsToLuaVariables(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "proximity");
      for(size_t i = 0; i < GetReadings().size(); ++i) {
         lua_pushnumber(pt_lua_state, i+1                 );
         lua_gettable  (pt_lua_state, -2                  );
         lua_pushnumber(pt_lua_state, m_tReadings[i].Value);
         lua_setfield  (pt_lua_state, -2, "value"         );
         lua_pop(pt_lua_state, 1);
      }
      lua_pop(pt_lua_state, 1);
   }
#endif


   /****************************************/
   /****************************************/

}
