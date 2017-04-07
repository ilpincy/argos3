/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_kinematicmodel.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PM3D_KINEMATICMODEL_H
#define PM3D_KINEMATICMODEL_H

namespace argos {
   class CPM3DEngine;
}

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_model.h>

namespace argos {

   class CPM3DKinematicModel : public CPM3DModel {

   public:

      CPM3DKinematicModel(CPM3DEngine& c_engine,
                          CEmbodiedEntity& c_entity);

      virtual ~CPM3DKinematicModel() {}

      virtual void Step();
      
      virtual void UpdateFromEntityStatus() = 0;

      /**
       * Returns the linear velocity of this model expressed in the world coordinate frame.
       */
      inline const CVector3& GetLinearVelocity() const {
         return m_cLinearVelocity;
      }

      /**
       * Returns the linear velocity of this model expressed in the world coordinate frame.
       */
      inline const CVector3& GetAngularVelocity() const {
         return m_cAngularVelocity;
      }

      /**
       * Sets the linear velocity of this model expressed in the world coordinate frame.
       */
      inline void SetLinearVelocity(const CVector3& c_vel) {
         m_cLinearVelocity = c_vel;
      }

      /**
       * Sets the angular velocity of this model expressed in the world coordinate frame.
       */
      inline void SetAngularVelocity(const CVector3& c_vel) {
         m_cAngularVelocity = c_vel;
      }

   protected:

      /**
       * The linear velocity of this model in the engine.
       */
      CVector3 m_cLinearVelocity;

      /**
       * The angular velocity of this model in the engine.
       * The length of this vector is the magnitude of the rotation;
       * its direction designates the rotation axis.
       */
      CVector3 m_cAngularVelocity;

   };

}

#endif
