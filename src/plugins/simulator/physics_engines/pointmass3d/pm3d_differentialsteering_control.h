/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_differentialsteering_control.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PM3D_DIFFERENTIALSTEERING_CONTROL_H
#define PM3D_DIFFERENTIALSTEERING_CONTROL_H

namespace argos {
   class CPM3DEngine;
}

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_kinematicmodel.h>

namespace argos {

   class CPM3DDifferentialSteeringControl {

   public:

      /**
       * Class constructor.
       * @param c_model The instance of the model associated to this control.
       * @param f_interwheel_distance The distance between the two wheels.
       */
      CPM3DDifferentialSteeringControl(CPM3DKinematicModel& c_model,
                                       Real f_interwheel_distance);

      virtual ~CPM3DDifferentialSteeringControl() {}

      void SetWheelVelocity(Real f_left_wheel,
                            Real f_right_wheel);

      inline Real GetInterwheelDistance() const {
         return m_fInterwheelDistance;
      }

   private:

      CPM3DKinematicModel& m_cModel;
      Real m_fInterwheelDistance;

   };

}

#endif
