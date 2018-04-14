/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_differentialsteering_control.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_DIFFERENTIALSTEERING_CONTROL_H
#define DYNAMICS2D_DIFFERENTIALSTEERING_CONTROL_H

namespace argos {
   class CDynamics2DEngine;
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_velocity_control.h>
#include <argos3/core/utility/math/vector2.h>

namespace argos {

   class CDynamics2DDifferentialSteeringControl : public CDynamics2DVelocityControl {

   public:
      
      CDynamics2DDifferentialSteeringControl(CDynamics2DEngine& c_engine,
                                             Real f_max_force,
                                             Real f_max_torque,
                                             Real f_interwheel_distance,
                                             TConfigurationNode* t_node = NULL);

      virtual ~CDynamics2DDifferentialSteeringControl() {}

      void SetWheelVelocity(Real f_left_wheel,
                            Real f_right_wheel);

      inline Real GetInterwheelDistance() const {
         return m_fInterwheelDistance;
      }

   private:

      Real m_fInterwheelDistance;

   };

}

#endif
