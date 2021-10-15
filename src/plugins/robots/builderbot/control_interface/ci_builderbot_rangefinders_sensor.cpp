/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_rangefinders_sensor.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_builderbot_rangefinders_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   const CCI_BuilderBotRangefindersSensor::SInterface::TVector& CCI_BuilderBotRangefindersSensor::GetInterfaces() const {
     return m_vecInterfaces;
   }

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotRangefindersSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "rangefinders");
      for(SInterface* ps_interface : m_vecInterfaces) {
         CLuaUtility::StartTable(pt_lua_state, ps_interface->Label);
         CLuaUtility::AddToTable(pt_lua_state, "proximity", ps_interface->Proximity);
         CLuaUtility::AddToTable(pt_lua_state, "illuminance", ps_interface->Illuminance);
         CLuaUtility::StartTable(pt_lua_state, "transform");
         CLuaUtility::AddToTable(pt_lua_state, "position", ps_interface->PositionOffset);
         CLuaUtility::AddToTable(pt_lua_state, "orientation", ps_interface->OrientationOffset);
         CLuaUtility::AddToTable(pt_lua_state, "anchor", ps_interface->Anchor);
         CLuaUtility::EndTable(pt_lua_state);
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_BuilderBotRangefindersSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "rangefinders");
      for(SInterface* ps_interface : m_vecInterfaces) {
         lua_getfield(pt_lua_state, -1, ps_interface->Label.c_str());
         CLuaUtility::AddToTable(pt_lua_state, "proximity", ps_interface->Proximity);
         CLuaUtility::AddToTable(pt_lua_state, "illuminance", ps_interface->Illuminance);
         lua_pop(pt_lua_state, 1);
      }
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

   const std::map<std::string, std::tuple<std::string, CVector3, CQuaternion> >
      CCI_BuilderBotRangefindersSensor::m_mapSensorConfig = {
         std::make_pair("7",          std::make_tuple("origin", CVector3( 0.0440, -0.0175, 0.0515), CQuaternion(0.5 * CRadians::PI, CVector3( 0.0000,  1.0000, 0)))),
         std::make_pair("8",          std::make_tuple("origin", CVector3( 0.0323, -0.0522, 0.0515), CQuaternion(0.5 * CRadians::PI, CVector3( 0.7071,  0.7071, 0)))),
         std::make_pair("9",          std::make_tuple("origin", CVector3(-0.0025, -0.0640, 0.0515), CQuaternion(0.5 * CRadians::PI, CVector3( 1.0000,  0.0000, 0)))),
         std::make_pair("10",          std::make_tuple("origin", CVector3(-0.0375, -0.0640, 0.0515), CQuaternion(0.5 * CRadians::PI, CVector3( 1.0000,  0.0000, 0)))),
         std::make_pair("11",          std::make_tuple("origin", CVector3(-0.0722, -0.0523, 0.0515), CQuaternion(0.5 * CRadians::PI, CVector3( 0.7071, -0.7071, 0)))),
         std::make_pair("12",          std::make_tuple("origin", CVector3(-0.0840, -0.0175, 0.0515), CQuaternion(0.5 * CRadians::PI, CVector3( 0.0000, -1.0000, 0)))),
         std::make_pair("1",          std::make_tuple("origin", CVector3(-0.0840,  0.0175, 0.0515), CQuaternion(0.5 * CRadians::PI, CVector3( 0.0000, -1.0000, 0)))),
         std::make_pair("2",          std::make_tuple("origin", CVector3(-0.0722,  0.0523, 0.0515), CQuaternion(0.5 * CRadians::PI, CVector3(-0.7071, -0.7071, 0)))),
         std::make_pair("3",          std::make_tuple("origin", CVector3(-0.0375,  0.0640, 0.0515), CQuaternion(0.5 * CRadians::PI, CVector3(-1.0000,  0.0000, 0)))),
         std::make_pair("4",         std::make_tuple("origin", CVector3(-0.0025,  0.0640, 0.0515), CQuaternion(0.5 * CRadians::PI, CVector3(-1.0000,  0.0000, 0)))),
         std::make_pair("5",         std::make_tuple("origin", CVector3( 0.0323,  0.0522, 0.0515), CQuaternion(0.5 * CRadians::PI, CVector3(-0.7071,  0.7071, 0)))),
         std::make_pair("6",         std::make_tuple("origin", CVector3( 0.0440,  0.0175, 0.0515), CQuaternion(0.5 * CRadians::PI, CVector3( 0.0000,  1.0000, 0)))),
         std::make_pair("left",       std::make_tuple("end_effector", CVector3(-0.0343,  0.016, -0.0288), CQuaternion(0.5 * CRadians::PI, CVector3( 0.0000,  1.0000, 0)))),
         std::make_pair("right",      std::make_tuple("end_effector", CVector3(-0.0343, -0.016, -0.0288), CQuaternion(0.5 * CRadians::PI, CVector3( 0.0000,  1.0000, 0)))),
         std::make_pair("underneath", std::make_tuple("end_effector", CVector3(-0.0025,  0.000,  0.0010), CQuaternion(1.0 * CRadians::PI, CVector3( 0.0000,  1.0000, 0)))),
         std::make_pair("front",      std::make_tuple("end_effector", CVector3( 0.0244,  0.000,  0.0086), CQuaternion(0.5 * CRadians::PI, CVector3( 0.0000,  1.0000, 0)))),
   };

   /****************************************/
   /****************************************/

}
