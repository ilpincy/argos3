/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_differentialsteering_control.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "pm3d_differentialsteering_control.h"

namespace argos {

   /****************************************/
   /****************************************/

   CPM3DDifferentialSteeringControl::CPM3DDifferentialSteeringControl(CPM3DKinematicModel& c_model,
                                                                      Real f_interwheel_distance) :
      m_cModel(c_model),
      m_fInterwheelDistance(f_interwheel_distance) {}

   /****************************************/
   /****************************************/

   void CPM3DDifferentialSteeringControl::SetWheelVelocity(Real f_left_wheel,
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
      CVector3 cAV = m_cModel.GetAngularVelocity();
      cAV.SetZ((f_right_wheel - f_left_wheel) / m_fInterwheelDistance);
      m_cModel.SetAngularVelocity(cAV);
      CVector3 cVelocity((f_right_wheel + f_left_wheel) * 0.5f, 0.0f, 0.0f);
      m_cModel.SetLinearVelocity(cVelocity);
   }

   /****************************************/
   /****************************************/

}
