/**
 * @file <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_front_camera_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_PIPUCK_FRONT_CAMERA_SENSOR_H
#define CCI_PIPUCK_FRONT_CAMERA_SENSOR_H

namespace argos {
   class CCI_PiPuckFrontCameraSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CCI_PiPuckFrontCameraSensor : public CCI_Sensor {

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

      using TConfiguration = std::tuple<const char*, CVector3, CQuaternion>;

   public:

      CCI_PiPuckFrontCameraSensor() {}

      virtual ~CCI_PiPuckFrontCameraSensor() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual ELedState DetectLed(const CVector3& c_position) const = 0;

      virtual Real GetTimestamp() const = 0;

      virtual const STag::TVector& GetTags() const = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif
   protected:

      /* the position, orientation, and anchor of the camera sensor */
      CQuaternion m_cOrientationOffset;
      CVector2 m_cResolution;
      CVector2 m_cPrincipalPoint;
      CVector2 m_cFocalLength;
      /* constants and defaults */
      static const CVector3 POSITION_OFFSET;
      static const Real DEFAULT_ROTATION;
      static const CVector2 DEFAULT_RESOLUTION;
      static const CVector2 DEFAULT_FOCAL_LENGTH;
   };

}

#endif
