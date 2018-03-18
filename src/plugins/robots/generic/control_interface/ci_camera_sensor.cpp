/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_camera_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include <algorithm>

#include "ci_camera_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   const CCI_CameraSensor::SInterface::TVector& CCI_CameraSensor::GetInterfaces() const {
      return m_vecInterfaces;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_CameraSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "cameras");
      for(SInterface& s_interface : m_vecInterfaces) {
         CLuaUtility::StartTable(pt_lua_state, s_interface.Id);
         for(CCI_CameraSensorAlgorithm* pc_algorithm : s_interface.Algorithms) {
            CLuaUtility::StartTable(pt_lua_state, pc_algorithm->GetId());     
            pc_algorithm->CreateLuaState(pt_lua_state);
            CLuaUtility::EndTable(pt_lua_state);
         }
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_CameraSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "cameras");
      for(SInterface& s_interface : m_vecInterfaces) {
         CLuaUtility::StartTable(pt_lua_state, s_interface.Id);     
         for(CCI_CameraSensorAlgorithm* pc_algorithm : s_interface.Algorithms) {
            CLuaUtility::StartTable(pt_lua_state, pc_algorithm->GetId());
            pc_algorithm->ReadingsToLuaState(pt_lua_state);
            CLuaUtility::EndTable(pt_lua_state);
         }
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

}
