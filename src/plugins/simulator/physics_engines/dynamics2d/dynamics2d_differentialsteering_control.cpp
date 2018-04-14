/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_differentialsteering_control.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "dynamics2d_differentialsteering_control.h"

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics2DDifferentialSteeringControl::CDynamics2DDifferentialSteeringControl(CDynamics2DEngine& c_engine,
                                                                                  Real f_max_force,
                                                                                  Real f_max_torque,
                                                                                  Real f_interwheel_distance,
                                                                                  TConfigurationNode* t_node) :
      CDynamics2DVelocityControl(c_engine, f_max_force, f_max_torque, t_node), 
      m_fInterwheelDistance(f_interwheel_distance) {}

   /****************************************/
   /****************************************/

   void CDynamics2DDifferentialSteeringControl::SetWheelVelocity(Real f_left_wheel,
                                                                 Real f_right_wheel) {
         /*
          * THE DIFFERENTIAL STEERING SYSTEM
          *
          * check http://rossum.sourceforge.net/papers/DiffSteer/
          * for details
          *
          * Equations:
          *
          * w = (Vr - Vl) / b
          * v = [ ((Vr + Vl) / 2) cos(a),
          *       ((Vr + Vl) / 2) sin(a) ]
          *
          * where:
          *        a  = body orientation
          *        w  = body angular velocity
          *        v  = body center linear velocity
          *        Vr = right wheel velocity
          *        Vl = left wheel velocity
          *        b  = length of wheel axis
          */
      SetAngularVelocity((f_right_wheel - f_left_wheel) / m_fInterwheelDistance);
      Real fVelocity = (f_right_wheel + f_left_wheel) * 0.5f;
      CVector2 cVelocity(fVelocity * ::cos(m_ptControlledBody->a),
                         fVelocity * ::sin(m_ptControlledBody->a));
      SetLinearVelocity(cVelocity);
   }

   /****************************************/
   /****************************************/

}
