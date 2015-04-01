/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "ci_range_and_bearing_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   const CCI_RangeAndBearingSensor::TReadings& CCI_RangeAndBearingSensor::GetReadings() const {
     return m_tReadings;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_RangeAndBearingSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "range_and_bearing");
      for(size_t i = 0; i < m_tReadings.size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i+1);
         CLuaUtility::AddToTable(pt_lua_state, "range", m_tReadings[i].Range);
         CLuaUtility::AddToTable(pt_lua_state, "horizontal_bearing", m_tReadings[i].HorizontalBearing);
         CLuaUtility::AddToTable(pt_lua_state, "vertical_bearing", m_tReadings[i].VerticalBearing);
         CLuaUtility::StartTable(pt_lua_state, "data");
         const CByteArray& cData = m_tReadings[i].Data;
         for(size_t j = 0; j < cData.Size(); ++j) {
            CLuaUtility::AddToTable(pt_lua_state, j+1, cData[j]);
         }
         CLuaUtility::EndTable(pt_lua_state);
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_RangeAndBearingSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "range_and_bearing");
      /* Save the number of elements in the RAB table */
      size_t unLastMsgNum = lua_rawlen(pt_lua_state, -1);
      /* Overwrite the table with the new messages */
      for(size_t i = 0; i < m_tReadings.size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i+1);
         CLuaUtility::AddToTable(pt_lua_state, "range", m_tReadings[i].Range);
         CLuaUtility::AddToTable(pt_lua_state, "horizontal_bearing", m_tReadings[i].HorizontalBearing);
         CLuaUtility::AddToTable(pt_lua_state, "vertical_bearing", m_tReadings[i].VerticalBearing);
         CLuaUtility::StartTable(pt_lua_state, "data");
         const CByteArray& cData = m_tReadings[i].Data;
         for(size_t j = 0; j < cData.Size(); ++j) {
            CLuaUtility::AddToTable(pt_lua_state, j+1, cData[j]);
         }
         CLuaUtility::EndTable(pt_lua_state);
         CLuaUtility::EndTable(pt_lua_state);
      }
      /* Are the new messages less than the old ones? */
      if(m_tReadings.size() < unLastMsgNum) {
         /* Yes, set to nil all the extra entries */
         for(size_t i = m_tReadings.size()+1; i <= unLastMsgNum; ++i) {
            lua_pushnumber(pt_lua_state,  i);
            lua_pushnil   (pt_lua_state    );
            lua_settable  (pt_lua_state, -3);
         }
      }
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

   CCI_RangeAndBearingSensor::SPacket::SPacket() :
      Range(0.0f) {}

   /****************************************/
   /****************************************/

}
