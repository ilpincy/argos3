/**
 * @file <argos3/plugins/robots/prototype/control_interface/ci_prototype_joints_sensor.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "ci_prototype_joints_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PrototypeJointsSensor::CreateLuaState(lua_State* pt_lua_state) {
      /* CLuaUtility::OpenRobotStateTable is required since we don't know 
         whether the joint actuator has been added to the robot nor do we
         know which joints it may have added to the robot's table in Lua */
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "joints");
      for(SSensor* pc_sensor : m_vecSensors) {
         CLuaUtility::OpenRobotStateTable(pt_lua_state, pc_sensor->Id);     
         CLuaUtility::AddToTable(pt_lua_state, "encoder", pc_sensor->Value);
         CLuaUtility::CloseRobotStateTable(pt_lua_state);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PrototypeJointsSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "joints");
      for(SSensor* pc_sensor : m_vecSensors) {
         lua_getfield(pt_lua_state, -1, (pc_sensor->Id).c_str());
         CLuaUtility::AddToTable(pt_lua_state, "encoder", pc_sensor->Value);
         lua_pop(pt_lua_state, 1);
      }
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}
