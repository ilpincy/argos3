/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_radios_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "ci_radios_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   const CCI_RadiosSensor::SInterface::TVector& CCI_RadiosSensor::GetInterfaces() const {
      return m_vecInterfaces;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_RadiosSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "radios"); // radios
      for(size_t i = 0; i < m_vecInterfaces.size(); ++i) {
         CLuaUtility::OpenRobotStateTable(pt_lua_state, m_vecInterfaces[i].Id); // interfaces
         CLuaUtility::StartTable(pt_lua_state, "rx_data");
         CLuaUtility::EndTable(pt_lua_state);
         CLuaUtility::CloseRobotStateTable(pt_lua_state); // interfaces
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state); // radios
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_RadiosSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "radios"); // radios
      for(size_t i = 0; i < m_vecInterfaces.size(); i++) {
         lua_getfield(pt_lua_state, -1, (m_vecInterfaces[i].Id).c_str()); // interface
         lua_getfield(pt_lua_state, -1, "rx_data"); // data
         size_t unLastMessageCount = lua_rawlen(pt_lua_state, -1);
         for(size_t j = 0; j < m_vecInterfaces[i].Data.size(); ++j) {
            CLuaUtility::StartTable(pt_lua_state, j + 1); // messages
            for(size_t k = 0; k < m_vecInterfaces[i].Data[j].Size(); ++k) {
               CLuaUtility::AddToTable(pt_lua_state, k + 1, m_vecInterfaces[i].Data[j][k]); // bytes
            }
            CLuaUtility::EndTable(pt_lua_state); // messages
         }
         if(m_vecInterfaces[i].Data.size() < unLastMessageCount) {
            /* Remove the extra entries from the table */
            for(size_t j = m_vecInterfaces[i].Data.size() + 1; j <= unLastMessageCount; ++j) {
               lua_pushnumber(pt_lua_state, j);
               lua_pushnil(pt_lua_state);
               lua_settable(pt_lua_state, -3);
            }
         }
         lua_pop(pt_lua_state, 1); // data
         lua_pop(pt_lua_state, 1); // interface       
      }
      lua_pop(pt_lua_state, 1); // radios
   }
#endif

   /****************************************/
   /****************************************/

}
