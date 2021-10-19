/**
 * @file <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_camera_system_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_BUILDERBOT_CAMERA_SYSTEM_SENSOR_H
#define CCI_BUILDERBOT_CAMERA_SYSTEM_SENSOR_H

namespace argos {
   class CCI_BuilderBotCameraSystemSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CCI_BuilderBotCameraSystemSensor : public CCI_Sensor {

   public:

      enum class ELedState : UInt8 {
         OFF = 0,
         Q1  = 1,
         Q2  = 2,
         Q3  = 3,
         Q4  = 4
      };

      struct STag {
         STag(UInt32 un_id,
              const CVector3& c_position,
              const CQuaternion& c_orientation,
              const CVector2& c_center,
              const std::array<CVector2, 4>& arr_corners) :
            Id(un_id),
            Position(c_position),
            Orientation(c_orientation),
            Center(c_center),
            Corners(arr_corners) {}
         /* Tag identifier */
         UInt32 Id;
         /* 3D information */
         CVector3 Position;
         CQuaternion Orientation;
         /* 2D information */
         CVector2 Center;
         std::array<CVector2, 4> Corners;
         /* Vector of tags */
         using TVector = std::vector<STag>;
      };

   public:

      CCI_BuilderBotCameraSystemSensor() :
         m_bEnabled(false) {}

      virtual ~CCI_BuilderBotCameraSystemSensor() {}

      virtual void Reset() {
         /* clear the readings */
         m_tTags.clear();
         /* zero the timestamp */
         m_fTimestamp = 0.0f;
      }

      virtual void Enable() {
         m_bEnabled = true;
      }

      virtual void Disable() {
         m_bEnabled = false;
      }

      virtual ELedState DetectLed(const CVector3& c_position) = 0;

      Real GetTimestamp() const {
         return m_fTimestamp;
      }

      const STag::TVector& GetTags() const {
         return m_tTags;
      }


#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif
   protected:

      /* whether or not the camera sensor is enabled */
      bool m_bEnabled;
      /* the detected tags in the current frame */
      STag::TVector m_tTags;
      /* the timestamp of the current frame */
      Real m_fTimestamp;
      /* the position, orientation, and anchor of the camera sensor */
      CVector2 m_cResolution;
      CVector3 m_cOffsetPosition;
      CQuaternion m_cOffsetOrientation;
      std::string m_strAnchor;

   };

}

#endif
