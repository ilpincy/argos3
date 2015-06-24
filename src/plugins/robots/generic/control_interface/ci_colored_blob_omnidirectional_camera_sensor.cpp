#include "ci_colored_blob_omnidirectional_camera_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaEnableOmnidirectionalCamera(lua_State* pt_lua_state) {
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_ColoredBlobOmnidirectionalCameraSensor>(pt_lua_state, "colored_blob_omnidirectional_camera")->Enable();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaDisableOmnidirectionalCamera(lua_State* pt_lua_state) {
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_ColoredBlobOmnidirectionalCameraSensor>(pt_lua_state, "colored_blob_omnidirectional_camera")->Disable();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_ColoredBlobOmnidirectionalCameraSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "colored_blob_omnidirectional_camera");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "enable", &LuaEnableOmnidirectionalCamera);
      CLuaUtility::AddToTable(pt_lua_state, "disable", &LuaDisableOmnidirectionalCamera);
      for(size_t i = 0; i < m_sReadings.BlobList.size(); ++i) {
         SBlob& sBlob = *(m_sReadings.BlobList[i]);
         CLuaUtility::StartTable(pt_lua_state, i+1);
         CLuaUtility::AddToTable(pt_lua_state, "distance", sBlob.Distance);
         CLuaUtility::AddToTable(pt_lua_state, "angle", sBlob.Angle);
         CLuaUtility::AddToTable(pt_lua_state, "color", sBlob.Color);
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_ColoredBlobOmnidirectionalCameraSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "colored_blob_omnidirectional_camera");
      /* Save the number of elements in the blob list */
      size_t unLastBlobNum = lua_rawlen(pt_lua_state, -1);
      /* Overwrite the table with the new messages */
      for(size_t i = 0; i < m_sReadings.BlobList.size(); ++i) {
         SBlob& sBlob = *(m_sReadings.BlobList[i]);
         CLuaUtility::StartTable(pt_lua_state, i+1);
         CLuaUtility::AddToTable(pt_lua_state, "distance", sBlob.Distance);
         CLuaUtility::AddToTable(pt_lua_state, "angle", sBlob.Angle);
         CLuaUtility::AddToTable(pt_lua_state, "color", sBlob.Color);
         CLuaUtility::EndTable(pt_lua_state);
      }
      /* Are the new messages less than the old ones? */
      if(m_sReadings.BlobList.size() < unLastBlobNum) {
         /* Yes, set to nil all the extra entries */
         for(size_t i = m_sReadings.BlobList.size()+1; i <= unLastBlobNum; ++i) {
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

   const CCI_ColoredBlobOmnidirectionalCameraSensor::SReadings& CCI_ColoredBlobOmnidirectionalCameraSensor::GetReadings() const {
      return m_sReadings;
   }

}
