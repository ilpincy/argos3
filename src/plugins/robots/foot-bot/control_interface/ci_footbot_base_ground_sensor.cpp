
/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_base_ground_sensor.cpp>
 *
 * @brief This file provides the implementation of the footbot base ground sensor.
 *
 * On the robot, there are two kinds of ground sensors.
 *
 * The BASE ground sensors are located on the main PCB, intertwined with the
 * proximity sensors and evenly spaced. They are 8 and are useful for detecting
 * holes.
 *
 * The MOTOR ground sensors are built on the motor PCB and located close to the
 * motors. They are 4 and useful to detect changes in color on the ground (i.e.,
 * for line following).
 *
 * This sensor provides access to the base sensors. The readings are in the following
 * order (seeing the robot from TOP, battery socket is the BACK):
 *
 *      front
 *
 *        0      r
 * l    1   7    i
 * e  2       6  g
 * f    3   5    h
 * t      4      t
 *
 *       back
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "ci_footbot_base_ground_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   CCI_FootBotBaseGroundSensor::CCI_FootBotBaseGroundSensor() :
      m_tReadings(8) {
      // Set the values for the base ground sensor offset (taken from the CAD model, in cm)
      m_tReadings[0].Offset.Set( 8.0, 0.0);
      m_tReadings[1].Offset.Set( 4.2, 6.5);
      m_tReadings[2].Offset.Set( 0.0, 8.0);
      m_tReadings[3].Offset.Set(-4.2, 6.5);
      m_tReadings[4].Offset.Set(-8.0, 0.0);
      m_tReadings[5].Offset.Set(-4.2,-6.5);
      m_tReadings[6].Offset.Set( 0.0,-8.0);
      m_tReadings[7].Offset.Set( 4.2,-6.5);
   }

   /****************************************/
   /****************************************/

   const CCI_FootBotBaseGroundSensor::TReadings& CCI_FootBotBaseGroundSensor::GetReadings() const {
     return m_tReadings;
   }
      
   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_FootBotBaseGroundSensor::SReading& s_reading) {
      c_os << "Value=<" << s_reading.Value
           << ">, Offset=<" << s_reading.Offset << ">";
      return c_os;
   }

   /****************************************/
   /****************************************/

   std::ostream& operator<<(std::ostream& c_os,
                            const CCI_FootBotBaseGroundSensor::TReadings& t_readings) {
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

#ifdef ARGOS_WITH_LUA
   void CCI_FootBotBaseGroundSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "base_ground");
      for(size_t i = 0; i < m_tReadings.size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i+1                            );
         CLuaUtility::AddToTable(pt_lua_state, "offset", m_tReadings[i].Offset);
         CLuaUtility::AddToTable(pt_lua_state, "value",  m_tReadings[i].Value );
         CLuaUtility::EndTable  (pt_lua_state                                 );
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_FootBotBaseGroundSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "base_ground");
      for(size_t i = 0; i < m_tReadings.size(); ++i) {
         lua_pushnumber(pt_lua_state, i+1                 );
         lua_gettable  (pt_lua_state, -2                  );
         lua_pushnumber(pt_lua_state, m_tReadings[i].Value);
         lua_setfield  (pt_lua_state, -2, "value"         );
         lua_pop       (pt_lua_state, 1                   );
      }
      lua_pop(pt_lua_state, 1);
   }
#endif


   /****************************************/
   /****************************************/

}
