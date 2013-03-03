/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_light_sensor.cpp>
 *
 * @brief This file provides the implementation of the footbot light sensor.
 *
 * This file provides the definition of the footbot light sensor.
 * The sensors are evenly spaced on a ring around the base of the robot.
 * Therefore, they do not turn with the turret. The readings are in the
 * following order (seeing the robot from TOP, battery socket is the BACK):
 *
 *              front
 *
 *              0 23
 *            1     22
 *          2         21
 *        3             20      r
 * l    4                 19    i
 * e  5                     18  g
 * f  6                     17  h
 * t    7                 16    t
 *        8             15
 *          9         14
 *            10     13
 *              11 12
 *
 *              back
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_footbot_light_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   static CRadians SPACING = CRadians(ARGOS_PI / 12.0f);
   static CRadians START_ANGLE = SPACING * 0.5f;

   /****************************************/
   /****************************************/

   CCI_FootBotLightSensor::CCI_FootBotLightSensor() :
      m_tReadings(24) {
      for(size_t i = 0; i < 24; ++i) {
         m_tReadings[i].Angle = START_ANGLE + i * SPACING;
         m_tReadings[i].Angle.SignedNormalize();
      }
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_FootBotLightSensor::CreateLuaVariables(lua_State* pt_lua_state) {
      lua_pushstring(pt_lua_state, "light");
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
   void CCI_FootBotLightSensor::ReadingsToLuaVariables(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "light");
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

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_FootBotLightSensor::SReading& s_reading) {
      c_os << "Value=<" << s_reading.Value
           << ">, Angle=<" << s_reading.Angle << ">";
      return c_os;
   }

   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_FootBotLightSensor::TReadings& t_readings) {
      if(! t_readings.empty()) {
         c_os << "{ " << t_readings[0].Value << " }";
         for(UInt32 i = 1; i < t_readings.size(); ++i) {
            c_os << " { " << t_readings[0].Value << " }";
         }
         c_os << std::endl;
      }
      return c_os;
   }

   /****************************************/
   /****************************************/

}
