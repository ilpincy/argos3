/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_ground_sensor.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_pipuck_ground_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckGroundSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "ground_sensors");
      Visit([pt_lua_state] (const SInterface& s_interface) {
         CLuaUtility::StartTable(pt_lua_state, s_interface.Label + 1);
         CLuaUtility::AddToTable(pt_lua_state, "reflected", s_interface.Reflected);
         CLuaUtility::AddToTable(pt_lua_state, "background", s_interface.Background);
         CLuaUtility::StartTable(pt_lua_state, "transform");
         CLuaUtility::AddToTable(pt_lua_state, "position", std::get<CVector3>(s_interface.Configuration));
         CLuaUtility::AddToTable(pt_lua_state, "orientation", std::get<CQuaternion>(s_interface.Configuration));
         CLuaUtility::AddToTable(pt_lua_state, "anchor", std::get<std::string>(s_interface.Configuration));
         CLuaUtility::EndTable(pt_lua_state);
         CLuaUtility::EndTable(pt_lua_state);
      });
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckGroundSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "ground_sensors");
      Visit([pt_lua_state] (const SInterface& s_interface) {
         lua_pushnumber(pt_lua_state, s_interface.Label + 1);
         lua_gettable(pt_lua_state, -2);
         lua_pushstring(pt_lua_state, "reflected");
         lua_pushnumber(pt_lua_state, s_interface.Reflected);
         lua_settable(pt_lua_state, -3);
         lua_pushstring(pt_lua_state, "background");
         lua_pushnumber(pt_lua_state, s_interface.Background);
         lua_settable(pt_lua_state, -3);
         lua_pop(pt_lua_state, 1);
      });
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

   const std::map<UInt8, CCI_PiPuckGroundSensor::TConfiguration> CCI_PiPuckGroundSensor::MAP_SENSOR_CONFIG = {
      std::make_pair(0, std::make_tuple("origin", CVector3(0.03,  0.01, 0.002), CQuaternion(CRadians::PI, CVector3::Y), 0.005)),
      std::make_pair(1, std::make_tuple("origin", CVector3(0.03,  0.0,  0.002), CQuaternion(CRadians::PI, CVector3::Y), 0.005)),
      std::make_pair(2, std::make_tuple("origin", CVector3(0.03, -0.01, 0.002), CQuaternion(CRadians::PI, CVector3::Y), 0.005)),
   };

   /****************************************/
   /****************************************/


}
