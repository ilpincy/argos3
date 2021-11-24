/**
 * @file <argos3/plugins/robots/drone/control_interface/ci_drone_camera_system_sensor.h>
 *
 * @author Michael Allwright <allsey87@gmail.com>
 */

#ifndef CCI_DRONE_CAMERAS_SYSTEM_SENSOR_H
#define CCI_DRONE_CAMERAS_SYSTEM_SENSOR_H

namespace argos {
   class CCI_DroneCameraSystemSensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

#include <functional>

namespace argos {

   class CCI_DroneCamerasSystemSensor : public CCI_Sensor {

   public:

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

      struct SInterface {
         /* constructor */
         SInterface(const std::string& str_label):
            Label(str_label),
            Enabled(false) {}
         /* methods */
         virtual void Enable();
         virtual void Disable();
         virtual const TConfiguration& GetConfiguration() const = 0;
         /*  data */
         std::string Label;
         bool Enabled;
         STag::TVector Tags;
         Real Timestamp;
      };

   public:

      CCI_DroneCamerasSystemSensor() {}

      virtual ~CCI_DroneCamerasSystemSensor() {}

      virtual void Visit(std::function<void(SInterface&)>) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      static const UInt32 DEFAULT_CAMERA_RESOLUTION_X;
      static const UInt32 DEFAULT_CAMERA_RESOLUTION_Y;
      static const Real DEFAULT_CAMERA_FOCAL_LENGTH_X;
      static const Real DEFAULT_CAMERA_FOCAL_LENGTH_Y;
      static const Real DEFAULT_CAMERA_PRINCIPAL_POINT_X;
      static const Real DEFAULT_CAMERA_PRINCIPAL_POINT_Y;
      static const Real DEFAULT_CAMERA_XY_OFFSET;
      static const Real DEFAULT_CAMERA_Z_OFFSET;
      static const CDegrees DEFAULT_CAMERA_ANGLE;
      static const std::map<std::string, TConfiguration> DEFAULT_SENSOR_CONFIGURATION;

   };

}

#endif
