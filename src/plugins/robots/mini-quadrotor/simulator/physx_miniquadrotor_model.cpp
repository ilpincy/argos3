/**
 * @file <argos3/plugins/robots/mini-quadrotor/simulator/physx_miniquadrotor_model.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "physx_miniquadrotor_model.h"
#include <argos3/plugins/robots/mini-quadrotor/simulator/miniquadrotor_entity.h>
#include <argos3/plugins/simulator/entities/rotor_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/
   
   /*
    * The parameter values are taken from data provided by Kumar's group directly
    */
   static const Real ARM_LENGTH            = 0.063f;
   static const Real ARM_HALF_LENGTH       = ARM_LENGTH * 0.5f;
   static const Real PROPELLER_LENGTH      = 0.082f;
   static const Real PROPELLER_HALF_LENGTH = PROPELLER_LENGTH * 0.5f;
   static const Real BODY_HALF_WIDTH       = ARM_HALF_LENGTH + PROPELLER_HALF_LENGTH;
   static const Real BODY_HEIGHT           = 0.015f;
   static const Real BODY_HALF_HEIGHT      = BODY_HEIGHT * 0.5f;
   static const Real BODY_MASS             = 0.06736f;
   static const Real UPLIFT_COEFFICIENT    = 1.9985e-9;
   static const Real DRAG_COEFFICIENT      = 4.737e-12;

   static const physx::PxVec3 POSITION_ERROR_COEFF(6.61f, 6.61f, 72.0f); // unused
   static const physx::PxVec3 VELOCITY_ERROR_COEFF(5.14f, 5.14f, 24.0f); // unused

   static const physx::PxTransform PITCH_ARM_POSE(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f)));
   static const physx::PxVec3      INERTIA_TENSOR_DIAGONAL(2.32e-3, 2.32e-3, 4e-3);
   static const physx::PxMat33     INERTIA_TENSOR(physx::PxMat33::createDiagonal(INERTIA_TENSOR_DIAGONAL));
   static const physx::PxMat33     INERTIA_TENSOR_INVERSE(INERTIA_TENSOR.getInverse());

   /****************************************/
   /****************************************/

   CPhysXMiniQuadrotorModel::CPhysXMiniQuadrotorModel(CPhysXEngine& c_engine,
                                                      CMiniQuadrotorEntity& c_entity) :
      CPhysXSingleBodyObjectModel(c_engine, c_entity),
      m_cMiniQuadrotorEntity(c_entity) {
      /* Calculate base center */
      SetARGoSReferencePoint(physx::PxVec3(0.0f, 0.0f, -BODY_HALF_HEIGHT));
      /* Get position and orientation in this engine's representation */
      physx::PxVec3 cPos;
      CVector3ToPxVec3(GetEmbodiedEntity().GetPosition(), cPos);
      physx::PxQuat cOrient;
      CQuaternionToPxQuat(GetEmbodiedEntity().GetOrientation(), cOrient);
      /* Create the transform
       * 1. a translation up by half body height
       * 2. a rotation around the base
       * 3. a translation to the final position
       */
      physx::PxTransform cTranslation1(physx::PxVec3(0.0f, 0.0f, BODY_HALF_HEIGHT));
      physx::PxTransform cRotation(cOrient);
      physx::PxTransform cTranslation2(cPos);
      physx::PxTransform cFinalTrans = cTranslation2 * cRotation * cTranslation1;
      /* Create the capsule geometry for the arms */
      physx::PxCapsuleGeometry* pcArmGeometry =
         new physx::PxCapsuleGeometry(BODY_HALF_HEIGHT,
                                      BODY_HALF_WIDTH);
      GetGeometries().push_back(pcArmGeometry);
      /* Create the body in its initial position and orientation */
      SetBody(GetPhysXEngine().GetPhysics().createRigidDynamic(cFinalTrans));
      /* Enable CCD on the body */
      GetBody()->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
      /* Create the shape for the roll arm */
      physx::PxShape* pcRollArmShape =
         GetBody()->createShape(*pcArmGeometry,
                               GetPhysXEngine().GetDefaultMaterial());
      GetShapes().push_back(pcRollArmShape);
      /* Assign the user data pointer to this model */
      pcRollArmShape->userData = this;
      /* Create the shape for the pitch arm */
      physx::PxShape* pcPitchArmShape =
         GetBody()->createShape(*pcArmGeometry,
                               GetPhysXEngine().GetDefaultMaterial(),
                               PITCH_ARM_POSE);
      GetShapes().push_back(pcPitchArmShape);
      /* Assign the user data pointer to this model */
      pcPitchArmShape->userData = this;
      /* Set body mass and inertia tensor */
      GetBody()->setMass(BODY_MASS);
      GetBody()->setMassSpaceInertiaTensor(INERTIA_TENSOR_DIAGONAL);
      /* Add body to the scene */
      GetPhysXEngine().GetScene().addActor(*GetBody());
      /* Calculate bounding box */
      CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

   void CPhysXMiniQuadrotorModel::UpdateFromEntityStatus() {
      /* Get desired rotor velocities */
      const Real* pfRotorVel = m_cMiniQuadrotorEntity.GetRotorEquippedEntity().GetRotorVelocities();
      /* Calculate the squares */
      Real pfSquareRotorVel[4] = {
         pfRotorVel[0] * pfRotorVel[0],
         pfRotorVel[1] * pfRotorVel[1],
         pfRotorVel[2] * pfRotorVel[2],
         pfRotorVel[3] * pfRotorVel[3]
      };
      /* Calculate uplift-related control input */
      Real fUpliftInput =
         UPLIFT_COEFFICIENT *
         ((pfSquareRotorVel[0]) +
          (pfSquareRotorVel[1]) +
          (pfSquareRotorVel[2]) +
          (pfSquareRotorVel[3]));
      /* Calculate torque-related control inputs */
      physx::PxVec3 cTorqueInputs(
         UPLIFT_COEFFICIENT * ARM_HALF_LENGTH * (pfSquareRotorVel[0] - pfSquareRotorVel[2]),
         UPLIFT_COEFFICIENT * ARM_HALF_LENGTH * (pfSquareRotorVel[1] - pfSquareRotorVel[3]),
         DRAG_COEFFICIENT * (pfSquareRotorVel[0] - pfSquareRotorVel[1] + pfSquareRotorVel[2] - pfSquareRotorVel[3]));
      /* Apply uplift */
      physx::PxRigidBodyExt::addLocalForceAtLocalPos(
         *GetBody(),
         physx::PxVec3(0.0f, 0.0f, fUpliftInput),
         physx::PxVec3(0.0f));
      /* Apply rotational moment */
      physx::PxVec3 cTorque = (-GetBody()->getAngularVelocity()).cross(INERTIA_TENSOR * GetBody()->getAngularVelocity()) + cTorqueInputs;
      GetBody()->addTorque(cTorque);
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_PHYSX_OPERATIONS_ON_ENTITY(CMiniQuadrotorEntity, CPhysXMiniQuadrotorModel);

   /****************************************/
   /****************************************/

}
