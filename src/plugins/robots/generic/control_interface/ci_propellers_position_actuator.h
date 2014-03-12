/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_propellers_position_actuator.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef CI_PROPELLERS_POSITION_ACTUATOR_H
#define CI_PROPELLERS_POSITION_ACTUATOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_PropellersPositionActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CCI_PropellersPositionActuator : public CCI_Actuator {

   public:

      virtual ~CCI_PropellersPositionActuator() {}

      /**
       * Sets the position of the robot.
       * @param c_pos The desired position.
       */
      virtual void SetPosition(const CVector3& c_pos) = 0;

      /**
       * Sets the orientation of the robot.
       * @param c_orient The desired orientation.
       */
      virtual void SetOrientation(const CQuaternion& c_orient) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   };

}

#endif
