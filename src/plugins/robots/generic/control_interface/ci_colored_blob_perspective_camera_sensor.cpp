#include "ci_colored_blob_perspective_camera_sensor.h"

#ifdef ARGOS_WITH_LUA
#include <argos3/core/wrappers/lua/lua_utility.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaEnablePerspectiveCamera(lua_State* pt_lua_state) {
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_ColoredBlobPerspectiveCameraSensor>(pt_lua_state, "colored_blob_perspective_camera")->Enable();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   int LuaDisablePerspectiveCamera(lua_State* pt_lua_state) {
      /* Perform action */
      CLuaUtility::GetDeviceInstance<CCI_ColoredBlobPerspectiveCameraSensor>(pt_lua_state, "colored_blob_perspective_camera")->Disable();
      return 0;
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_ColoredBlobPerspectiveCameraSensor::CreateLuaState(lua_State* pt_lua_state) {
      CLuaUtility::OpenRobotStateTable(pt_lua_state, "colored_blob_perspective_camera");
      CLuaUtility::AddToTable(pt_lua_state, "_instance", this);
      CLuaUtility::AddToTable(pt_lua_state, "enable", &LuaEnablePerspectiveCamera);
      CLuaUtility::AddToTable(pt_lua_state, "disable", &LuaDisablePerspectiveCamera);
      for(size_t i = 0; i < m_sReadings.BlobList.size(); ++i) {
         SBlob& sBlob = *(m_sReadings.BlobList[i]);
         CLuaUtility::StartTable(pt_lua_state, i+1);
         CLuaUtility::AddToTable(pt_lua_state, "color", sBlob.Color);
         CLuaUtility::AddToTable(pt_lua_state, "x", sBlob.X);
         CLuaUtility::AddToTable(pt_lua_state, "y", sBlob.Y);
         CLuaUtility::EndTable(pt_lua_state);
      }
      CLuaUtility::CloseRobotStateTable(pt_lua_state);
   }
#endif

   /****************************************/
   /****************************************/

#ifdef ARGOS_WITH_LUA
   void CCI_ColoredBlobPerspectiveCameraSensor::ReadingsToLuaState(lua_State* pt_lua_state) {
      lua_getfield(pt_lua_state, -1, "colored_blob_perspective_camera");
      /* Save the number of elements in the blob list */
      size_t unLastBlobNum = lua_rawlen(pt_lua_state, -1);
      /* Overwrite the table with the new messages */
      for(size_t i = 0; i < m_sReadings.BlobList.size(); ++i) {
         SBlob& sBlob = *(m_sReadings.BlobList[i]);
         CLuaUtility::StartTable(pt_lua_state, i+1);
         CLuaUtility::AddToTable(pt_lua_state, "color", sBlob.Color);
         CLuaUtility::AddToTable(pt_lua_state, "x", sBlob.X);
         CLuaUtility::AddToTable(pt_lua_state, "y", sBlob.Y);
         CLuaUtility::EndTable(pt_lua_state);
      }
      /* Are the new blobs less than the old ones? */
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

   const CCI_ColoredBlobPerspectiveCameraSensor::SReadings& CCI_ColoredBlobPerspectiveCameraSensor::GetReadings() const {
      return m_sReadings;
   }

   /****************************************/
   /****************************************/

}
