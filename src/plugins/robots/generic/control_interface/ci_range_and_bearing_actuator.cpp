/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_actuator.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "ci_range_and_bearing_actuator.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * The stack can have one or two values
    * - The case of one value is when you set an entire array
    * - In the case of two values, you must have:
    *   1. the idx of the data item to set
    *   2. the value of the data item in the range [0,255]
    */
   int LuaRABSetData(lua_State* pt_lua_state) {
      if(lua_gettop(pt_lua_state) == 1) {
         /* Check parameters */
         luaL_checktype(pt_lua_state, 1, LUA_TTABLE);
         /* Get reference to actuator */
         CCI_RangeAndBearingActuator* pcAct = CLuaUtility::GetDeviceInstance<CCI_RangeAndBearingActuator>(pt_lua_state, "range_and_bearing");
         /* Check whether sizes match */
         if(pcAct->GetSize() != lua_rawlen(pt_lua_state, -1)) {
            return luaL_error(pt_lua_state, "robot.range_and_bearing.set_data(array) expects an array of %d numbers", pcAct->GetSize());
         }
         /* Fill up a byte array, checking that all elements are numbers */
         CByteArray cBuf(pcAct->GetSize());
         for(size_t i = 0; i < pcAct->GetSize(); ++i) {
            lua_pushnumber(pt_lua_state, i+1);
            lua_gettable(pt_lua_state, -2);
            if(lua_type(pt_lua_state, -1) == LUA_TNUMBER) {
               cBuf[i] = static_cast<UInt8>(lua_tonumber(pt_lua_state, -1));
               lua_pop(pt_lua_state, 1);
            }
            else {
               return luaL_error(pt_lua_state, "element #%d of the array is not a number", i+1);
            }
         }
         /* Perform action */
         pcAct->SetData(cBuf);
         return 0;
      }
      else if(lua_gettop(pt_lua_state) == 2) {
         /* Check parameters */
         luaL_checktype(pt_lua_state, 1, LUA_TNUMBER);
         luaL_checktype(pt_lua_state, 2, LUA_TNUMBER);
         /* Get reference to actuator */
         CCI_RangeAndBearingActuator* pcAct = CLuaUtility::GetDeviceInstance<CCI_RangeAndBearingActuator>(pt_lua_state, "range_and_bearing");
         /* Check parameter values */
         size_t unIdx = lua_tonumber(pt_lua_state, 1);
         UInt8 unData = lua_tonumber(pt_lua_state, 2);
         if(unIdx < 1 || unIdx > pcAct->GetSize()) {
            return luaL_error(pt_lua_state, "passed index %d out of bounds [1,%d]", unIdx, pcAct->GetSize());
         }
         /* Perform action */
         pcAct->SetData(unIdx-1, unData);
         return 0;
      }
      else {
         return luaL_error(pt_lua_state, "robot.range_and_bearing.set_data() expects either one or two arguments");
      }
   }
#endif

#ifdef ARGOS_WITH_LUA
   /*
    * The stack must have no values
    */
   int LuaRABClearData(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 0) {
         return luaL_error(pt_lua_state, "robot.range_and_bearing.clear_data() expects no arguments");
      }
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_RangeAndBearingActuator>(pt_lua_state, "range_and_bearing")->ClearData();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

   size_t CCI_RangeAndBearingActuator::GetSize() const {
     return m_cData.Size();
   }
  
   /****************************************/
   /****************************************/

   void CCI_RangeAndBearingActuator::SetData(size_t un_idx,
					    UInt8 un_value) {
     m_cData[un_idx] = un_value;
   }
  
   /****************************************/
   /****************************************/

   void CCI_RangeAndBearingActuator::ClearData() {
     m_cData.Zero();
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_RangeAndBearingActuator::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::StartTable(pt_lua_state, "range_and_bearing");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "set_data", &LuaRABSetData);
      CLuaUtility::AddToTable(pt_lua_state, "clear_data", &LuaRABClearData);
      CLuaUtility::EndTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

   void CCI_RangeAndBearingActuator::SetData(const CByteArray& c_data) {
      if(m_cData.Size() == c_data.Size()) {
         m_cData = c_data;
      }
      else {
         THROW_ARGOSEXCEPTION("CCI_RangeAndBearingActuator::SetData() : data size does not match, expected " << m_cData.Size() << ", got " << c_data.Size());
      }
   }

   /****************************************/
   /****************************************/

}
