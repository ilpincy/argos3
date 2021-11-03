/**
 * @file <argos3/plugins/robots/pi-puck/simulator/dynamics3d_pipuck_model.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_PIPUCK_MODEL_H
#define DYNAMICS3D_PIPUCK_MODEL_H

namespace argos {
   class CDynamics3DPiPuckModel;
   class CPiPuckEntity;
   class CPiPuckDifferentialDriveEntity;
}

#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Featherstone/btMultiBodyJointMotor.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_multi_body_object_model.h>

namespace argos {

   class CDynamics3DPiPuckModel : public CDynamics3DMultiBodyObjectModel {

   public:     

      CDynamics3DPiPuckModel(CDynamics3DEngine& c_engine,
                             CPiPuckEntity& c_pipuck);

      virtual ~CDynamics3DPiPuckModel() {}

      virtual void Reset();

      virtual void CalculateBoundingBox();

      virtual void UpdateEntityStatus();

      virtual void UpdateFromEntityStatus();

      virtual void AddToWorld(btMultiBodyDynamicsWorld& c_world);

      virtual void RemoveFromWorld(btMultiBodyDynamicsWorld& c_world);

   private:
      /* joint constraints */
      std::unique_ptr<btMultiBodyJointMotor> m_ptrLeftMotor;
      std::unique_ptr<btMultiBodyJointMotor> m_ptrRightMotor;
      /* links */
      std::shared_ptr<CBase> m_ptrBody;
      std::shared_ptr<CLink> m_ptrLeftWheel;
      std::shared_ptr<CLink> m_ptrRightWheel;
      /* entities */
      CPiPuckDifferentialDriveEntity& m_cDifferentialDriveEntity;
      /* lower base data */
      static const btVector3 m_cBodyHalfExtents;
      static const btScalar m_fBodyMass;
      static const btTransform m_cBodyOffset;
      static const btTransform m_cBodyGeometricOffset;
      /* wheel data */
      static const btVector3 m_cWheelHalfExtents;
      static const btScalar m_fWheelMass;
      static const btTransform m_cWheelGeometricOffset;
      static const btTransform m_cLeftWheelOffset;
      static const btTransform m_cRightWheelOffset;
      static const btVector3 m_cBodyToRightWheelJointOffset;
      static const btVector3 m_cRightWheelToBodyJointOffset;
      static const btQuaternion m_cBodyToRightWheelJointRotation;
      static const btVector3 m_cBodyToLeftWheelJointOffset;
      static const btVector3 m_cLeftWheelToBodyJointOffset;
      static const btQuaternion m_cBodyToLeftWheelJointRotation;
      static const btScalar m_fBodyFriction;
      static const btScalar m_fWheelMotorMaxImpulse;
      static const btScalar m_fWheelFriction;
      static const btScalar m_fWheelMotorClamp;
      static const btScalar m_fWheelMotorKdCoefficient;
   };
}

#endif
