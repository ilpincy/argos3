/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_camera_system_sensor.cpp>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#include "ci_pipuck_camera_system_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#include <argos3/core/wrappers/lua/lua_vector3.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaEnablePiPuckCameraSystemSensor(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 0) {
         return luaL_error(pt_lua_state, "robot.camera_system.enable() expects zero arguments");
      }
      /* Get the camera sensor */
      CCI_PiPuckCameraSystemSensor* pcCameraSensor = 
         CLuaUtility::GetDeviceInstance<CCI_PiPuckCameraSystemSensor>(pt_lua_state, "camera_system");
      /* Set the enable member */
      pcCameraSensor->Enable();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaDisablePiPuckCameraSystemSensor(lua_State* pt_lua_state) {
      /* Check parameters */
      if(lua_gettop(pt_lua_state) != 0) {
         return luaL_error(pt_lua_state, "robot.camera_system.disable() expects zero arguments");
      }
      /* Get the camera sensor */
      CCI_PiPuckCameraSystemSensor* pcCameraSensor = 
         CLuaUtility::GetDeviceInstance<CCI_PiPuckCameraSystemSensor>(pt_lua_state, "camera_system");
      /* Set the enable member */
      pcCameraSensor->Disable();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   /*
    * the stack must contain a single table with keys named x, y, and z
    * which represent the X, Y, and Z components of a 3D vector
    */
   int LuaPiPuckCameraSystemSensorDetectLed(lua_State* pt_lua_state) {
      /* check parameters */
      if(lua_gettop(pt_lua_state) != 1) {
         return luaL_error(pt_lua_state, "robot.camera_system.detect_led() expects a single argument");
      }
      const CVector3& cPosition = CLuaVector3::ToVector3(pt_lua_state, 1);
      /* get the camera sensor */
      CCI_PiPuckCameraSystemSensor* pcCameraSensor = 
         CLuaUtility::GetDeviceInstance<CCI_PiPuckCameraSystemSensor>(pt_lua_state, "camera_system");
      /* get the LED state */
      const CCI_PiPuckCameraSystemSensor::ELedState& eLedState =
         pcCameraSensor->DetectLed(cPosition);
      /* convert the LED state to an integer and push it onto the stack */
      lua_pushinteger(pt_lua_state, static_cast<UInt8>(eLedState));
      /* return a single result, the integer */
      return 1;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckCameraSystemSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "camera_system");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state,
                              "enable",
                              &LuaEnablePiPuckCameraSystemSensor);
      CLuaUtility::AddToTable(pt_lua_state,
                              "disable",
                              &LuaDisablePiPuckCameraSystemSensor);
      CLuaUtility::AddToTable(pt_lua_state,
                              "detect_led",
                              &LuaPiPuckCameraSystemSensorDetectLed);
      CLuaUtility::AddToTable(pt_lua_state, "timestamp", 0.0f);
      CLuaUtility::StartTable(pt_lua_state, "tags");
      for(size_t i = 0; i < m_tTags.size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i + 1);
         CLuaUtility::AddToTable(pt_lua_state, "id", m_tTags[i].Id);
         CLuaUtility::AddToTable(pt_lua_state, "position", m_tTags[i].Position);
         CLuaUtility::AddToTable(pt_lua_state, "orientation", m_tTags[i].Orientation);
         CLuaUtility::AddToTable(pt_lua_state, "center", m_tTags[i].Center);
         /* start corners */
         CLuaUtility::StartTable(pt_lua_state, "corners");
         for(size_t j = 0; j < m_tTags[i].Corners.size(); ++j) {           
            CLuaUtility::AddToTable(pt_lua_state, j + 1, m_tTags[i].Corners[j]);
         }
         CLuaUtility::EndTable(pt_lua_state);
         /* end corners */
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::EndTable(pt_lua_state);
      CLuaUtility::StartTable(pt_lua_state, "transform");
      CLuaUtility::AddToTable(pt_lua_state, "position", m_cOffsetPosition);
      CLuaUtility::AddToTable(pt_lua_state, "orientation", m_cOffsetOrientation);
      CLuaUtility::AddToTable(pt_lua_state, "anchor", m_strAnchor);
      CLuaUtility::EndTable(pt_lua_state);
      CLuaUtility::AddToTable(pt_lua_state, "resolution", m_cResolution);
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/





#ifdef ARGOS_WITH_LUA
   void CCI_PiPuckCameraSystemSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "camera_system");
      CLuaUtility::AddToTable(pt_lua_state, "timestamp", m_fTimestamp);
      lua_getfield(pt_lua_state, -1, "tags");
      /* get the tag count from last time */
      size_t unLastTagCount = lua_rawlen(pt_lua_state, -1);     
      for(size_t i = 0; i < m_tTags.size(); ++i) {
         CLuaUtility::StartTable(pt_lua_state, i + 1);
         CLuaUtility::AddToTable(pt_lua_state, "id", m_tTags[i].Id);
         CLuaUtility::AddToTable(pt_lua_state, "position", m_tTags[i].Position);
         CLuaUtility::AddToTable(pt_lua_state, "orientation", m_tTags[i].Orientation);
         CLuaUtility::AddToTable(pt_lua_state, "center", m_tTags[i].Center);
         /* start corners */
         CLuaUtility::StartTable(pt_lua_state, "corners");
         for(size_t j = 0; j < m_tTags[i].Corners.size(); ++j) {           
            CLuaUtility::AddToTable(pt_lua_state, j + 1, m_tTags[i].Corners[j]);
         }
         CLuaUtility::EndTable(pt_lua_state);
         /* end corners */
         CLuaUtility::EndTable(pt_lua_state);
      }
      if(m_tTags.size() < unLastTagCount) {
         /* Remove extra tags from the last update by setting them to nil */
         for(size_t i = m_tTags.size() + 1; i <= unLastTagCount; ++i) {
            lua_pushnumber(pt_lua_state,  i);
            lua_pushnil   (pt_lua_state    );
            lua_settable  (pt_lua_state, -3);
         }
      }
      lua_pop(pt_lua_state, 1);
      lua_pop(pt_lua_state, 1);
   }
#endif

   /****************************************/
   /****************************************/

}
