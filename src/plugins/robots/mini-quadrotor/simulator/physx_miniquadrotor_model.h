/**
 * @file <argos3/plugins/robots/mini-quadrotor/simulator/physx_miniquadrotor_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSX_MINIQUADROTOR_MODEL_H
#define PHYSX_MINIQUADROTOR_MODEL_H

namespace argos {
   class CPhysXEngine;
   class CPhysXMiniQuadrotorModel;
   class CMiniQuadrotorEntity;
}

#include <argos3/plugins/simulator/physics_engines/physx/physx_single_body_object_model.h>

namespace argos {

   class CPhysXMiniQuadrotorModel : public CPhysXSingleBodyObjectModel {

   public:

      CPhysXMiniQuadrotorModel(CPhysXEngine& c_engine,
                               CMiniQuadrotorEntity& c_entity);

      virtual void UpdateFromEntityStatus();

   private:

      CMiniQuadrotorEntity& m_cMiniQuadrotorEntity;
      physx::PxRigidDynamic* m_pcBody;
   };

}

#endif
