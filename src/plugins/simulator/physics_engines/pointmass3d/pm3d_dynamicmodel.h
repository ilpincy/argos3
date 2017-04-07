/**
 * @file <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PM3D_DYNAMICMODEL_H
#define PM3D_DYNAMICMODEL_H

#include <argos3/plugins/simulator/physics_engines/pointmass3d/pm3d_kinematicmodel.h>

namespace argos {

   class CPM3DDynamicModel : public CPM3DKinematicModel {

   public:

      CPM3DDynamicModel(CPM3DEngine& c_engine,
                               CEmbodiedEntity& c_entity);

      virtual ~CPM3DDynamicModel() {}

      virtual void Step();
      
      virtual void UpdateFromEntityStatus() = 0;

      /**
       * Returns the acceleration of this model expressed in the world coordinate frame.
       */
      inline const CVector3& GetAcceleration() const {
         return m_cAcceleration;
      }

      /**
       * Returns the torque of this model expressed in the world coordinate frame.
       */
      inline const CVector3& GetTorque() const {
         return m_cTorque;
      }

      /**
       * Sets the acceleration of this model expressed in the world coordinate frame.
       */
      inline void SetAcceleration(const CVector3& c_acc) {
         m_cAcceleration = c_acc;
      }

      /**
       * Sets the torque of this model expressed in the world coordinate frame.
       */
      inline void SetTorque(const CVector3& c_tor) {
         m_cTorque = c_tor;
      }

   protected:

      /** The acceleration of this model in the engine. */
      CVector3 m_cAcceleration;

      /** The torque of this model in the engine. */
      CVector3 m_cTorque;

   };

}

#endif
