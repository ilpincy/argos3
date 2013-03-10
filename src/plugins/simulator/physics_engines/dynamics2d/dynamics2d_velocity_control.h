/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_velocity_control.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_VELOCITY_CONTROL_H
#define DYNAMICS2D_VELOCITY_CONTROL_H

namespace argos {
   class CDynamics2DEngine;
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/chipmunk-physics/include/chipmunk.h>
#include <argos3/core/utility/math/vector2.h>

namespace argos {

   class CDynamics2DVelocityControl {

   public:
      
      CDynamics2DVelocityControl(CDynamics2DEngine& c_engine,
                             Real f_max_force,
                             Real f_max_torque);

      virtual ~CDynamics2DVelocityControl();

      void AttachTo(cpBody* pt_body);

      void Detach();

      void Reset();

      CVector2 GetLinearVelocity() const;

      void SetLinearVelocity(const CVector2& c_velocity);

      Real GetAngularVelocity() const;

      void SetAngularVelocity(Real f_velocity);

      inline Real GetMaxForce() const {
         return m_fMaxForce;
      }

      inline Real GetMaxTorque() const {
         return m_fMaxTorque;
      }

   protected:

      CDynamics2DEngine& m_cDyn2DEngine;

      cpBody* m_ptControlBody;

      cpBody* m_ptControlledBody;

      cpConstraint* m_ptLinearConstraint;

      cpConstraint* m_ptAngularConstraint;

      Real m_fMaxForce;

      Real m_fMaxTorque;
   };

}

#endif
