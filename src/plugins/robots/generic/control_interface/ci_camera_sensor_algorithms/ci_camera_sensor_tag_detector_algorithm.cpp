/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_camera_sensor_algorithms/ci_camera_sensor_tag_detector_algorithm.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "ci_camera_sensor_tag_detector_algorithm.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif


namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_CameraSensorTagDetectorAlgorithm::CreateLuaState(lua_State* pt_lua_state) {
      for(size_t i = 0; i < m_vecReadings.size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i + 1);
         CLuaUtility::AddToTable(pt_lua_state, "payload", m_vecReadings[i].Payload);
         CLuaUtility::AddToTable(pt_lua_state, "center", m_vecReadings[i].Center);
         CLuaUtility::StartTable(pt_lua_state, "corners");
         for(size_t j = 0; j < m_vecReadings[i].Corners.size(); ++j) {           
            CLuaUtility::AddToTable(pt_lua_state, j + 1, m_vecReadings[i].Corners[j]);
         }
         CLuaUtility::EndTable(pt_lua_state);
         CLuaUtility::EndTable(pt_lua_state);
      }
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_CameraSensorTagDetectorAlgorithm::ReadingsToLuaState(lua_State* pt_lua_state) {
      size_t unLastReadingsNum = lua_rawlen(pt_lua_state, -1);     
      for(size_t i = 0; i < m_vecReadings.size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i + 1);
         CLuaUtility::AddToTable(pt_lua_state, "payload", m_vecReadings[i].Payload);
         CLuaUtility::AddToTable(pt_lua_state, "center", m_vecReadings[i].Center);
         CLuaUtility::StartTable(pt_lua_state, "corners");
         for(size_t j = 0; j < m_vecReadings[i].Corners.size(); ++j) {           
            CLuaUtility::AddToTable(pt_lua_state, j + 1, m_vecReadings[i].Corners[j]);
         }
         CLuaUtility::EndTable(pt_lua_state);
         CLuaUtility::EndTable(pt_lua_state);
      }
      if(m_vecReadings.size() < unLastReadingsNum) {
         /* Remove extra readings from last update by setting them to nil */
         for(size_t i = m_vecReadings.size() + 1; i <= unLastReadingsNum; ++i) {
            lua_pushnumber(pt_lua_state,  i);
            lua_pushnil   (pt_lua_state    );
            lua_settable  (pt_lua_state, -3);
         }
      }
   } 
#endif

   /****************************************/
   /****************************************/
   
}
