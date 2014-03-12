/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_pan_tilt_camera_actuator.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef CI_PAN_TILT_CAMERA_ACTUATOR_H
#define CI_PAN_TILT_CAMERA_ACTUATOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_PanTiltCameraActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/math/angles.h>

namespace argos {

   class CCI_PanTiltCameraActuator : public CCI_Actuator {

   public:

      virtual ~CCI_PanTiltCameraActuator() {}

      /**
       * Sets the camera pan.
       * The pan is the orientation of the camera wrt the robot z axis.
       * The value must be in [0,CRadians::TWO_PI].
       * @param c_pan The desired pan.
       */
      virtual void SetPan(const CRadians& c_pan) = 0;

      /**
       * Sets the camera tilt.
       * The tilt is the orientation of the camera wrt the camera y axis.
       * The value must be in (-CRadians::PI_OVER_TWO,CRadians::PI_OVER_TWO).
       * Zero means that the camera is directed along the z axis, looking downwards.
       * @param c_tilt The desired tilt.
       */
      virtual void SetTilt(const CRadians& c_tilt) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   };

}

#endif
