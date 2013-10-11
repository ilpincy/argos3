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

#include <argos3/plugins/simulator/physics_engines/physx/physx_model.h>

namespace argos {

   class CPhysXMiniQuadrotorModel : public CPhysXModel {

   public:

      typedef std::map<std::string, CPhysXModel*> TMap;

   public:

      CPhysXMiniQuadrotorModel(CPhysXEngine& c_engine,
                               CMiniQuadrotorEntity& c_entity);

      virtual ~CPhysXMiniQuadrotorModel();

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual void Reset();

      virtual void CalculateBoundingBox();

      virtual void UpdateEntityStatus();
      virtual void UpdateFromEntityStatus();

      virtual bool IsCollidingWithSomething() const;

   private:

      physx::PxRigidDynamic*    m_pcBody;
      physx::PxVec3             m_cBaseCenterLocal;
      physx::PxShape*           m_pcRollArmShape;
      physx::PxShape*           m_pcPitchArmShape;
      physx::PxCapsuleGeometry* m_pcArmGeometry;
      CMiniQuadrotorEntity&     m_cMiniQuadrotorEntity;

   };

}

#endif
