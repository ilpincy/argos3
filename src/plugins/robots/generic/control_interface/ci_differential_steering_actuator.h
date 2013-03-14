
/**
 * @file <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef CCI_DIFFERENTIAL_STEERING_ACTUATOR_H
#define CCI_DIFFERENTIAL_STEERING_ACTUATOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_DifferentialSteeringActuator;
}

#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {

   class CCI_DifferentialSteeringActuator : public CCI_Actuator {

   public:

      virtual ~CCI_DifferentialSteeringActuator() {}

      virtual void SetLinearVelocity(Real f_left_velocity,
                                     Real f_right_velocity) = 0;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);
#endif

   protected:

      Real m_fCurrentVelocity[2];

   };

}

#endif
