/**
 * @file <argos3/plugins/robots/builderbot/simulator/dynamics3d_builderbot_model.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_BUILDERBOT_MODEL_H
#define DYNAMICS3D_BUILDERBOT_MODEL_H

namespace argos {
   class CDynamics3DBuilderBotModel;
   class CBuilderBotEntity;
   class CBuilderBotDifferentialDriveEntity;
   class CBuilderBotElectromagnetSystemEntity;
   class CBuilderBotLiftSystemEntity;
}

#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Featherstone/btMultiBodyJointMotor.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_multi_body_object_model.h>

namespace argos {

   class CDynamics3DBuilderBotModel : public CDynamics3DMultiBodyObjectModel {

   public:     

      CDynamics3DBuilderBotModel(CDynamics3DEngine& c_engine,
                                 CBuilderBotEntity& c_builderbot);

      virtual ~CDynamics3DBuilderBotModel() {}

      virtual void Reset();

      virtual void CalculateBoundingBox();

      virtual void UpdateEntityStatus();

      virtual void UpdateFromEntityStatus();

      virtual void AddToWorld(btMultiBodyDynamicsWorld& c_world);

      virtual void RemoveFromWorld(btMultiBodyDynamicsWorld& c_world);

      std::shared_ptr<CLink>& GetEndEffectorLink() {
         return m_ptrEndEffector;
      }

   private:
      /* joint constraints */
      std::unique_ptr<btMultiBodyJointMotor> m_ptrLeftMotor;
      std::unique_ptr<btMultiBodyJointMotor> m_ptrRightMotor;
      std::unique_ptr<btMultiBodyJointMotor> m_ptrEndEffectorMotor;
      std::unique_ptr<btMultiBodyJointLimitConstraint> m_ptrEndEffectorLimit;

      /* links */
      std::shared_ptr<CBase> m_ptrLowerBase;
      std::shared_ptr<CLink> m_ptrLeftWheel;
      std::shared_ptr<CLink> m_ptrRightWheel;
      std::shared_ptr<CLink> m_ptrRearPivot;
      std::shared_ptr<CLink> m_ptrUpperBase;
      std::shared_ptr<CLink> m_ptrLiftColumn;
      std::shared_ptr<CLink> m_ptrEndEffector;
      std::shared_ptr<CLink> m_ptrEndEffectorSupport;

      /* entities */
      CBuilderBotDifferentialDriveEntity& m_cDifferentialDriveEntity;
      CBuilderBotElectromagnetSystemEntity& m_cElectromagnetSystemEntity;
      CBuilderBotLiftSystemEntity& m_cLiftSystemEntity;

      /* lower base data */
      static const btVector3 m_cLowerBaseHalfExtents;
      static const btScalar m_fLowerBaseMass;
      static const btTransform m_cLowerBaseOffset;
      static const btTransform m_cLowerBaseGeometricOffset;
      /* wheel data */
      static const btVector3 m_cWheelHalfExtents;
      static const btScalar m_fWheelMass;
      static const btTransform m_cWheelGeometricOffset;
      static const btTransform m_cLeftWheelOffset;
      static const btTransform m_cRightWheelOffset;
      static const btVector3 m_cLowerBaseToRightWheelJointOffset;
      static const btVector3 m_cRightWheelToLowerBaseJointOffset;
      static const btQuaternion m_cLowerBaseToRightWheelJointRotation;
      static const btVector3 m_cLowerBaseToLeftWheelJointOffset;
      static const btVector3 m_cLeftWheelToLowerBaseJointOffset;
      static const btQuaternion m_cLowerBaseToLeftWheelJointRotation;
      static const btScalar m_fWheelMotorMaxImpulse;
      /* rear pivot data */
      static const btScalar m_fRearPivotRadius;
      static const btScalar m_fRearPivotMass;
      static const btTransform m_cRearPivotGeometricOffset;
      static const btTransform m_cRearPivotOffset;
      static const btVector3 m_cLowerBaseToRearPivotJointOffset;
      static const btVector3 m_cRearPivotToLowerBaseJointOffset;
      static const btQuaternion m_cLowerBaseToRearPivotJointRotation;
      /* upper base data */
      static const std::vector<btVector3> m_vecUpperBasePoints;
      static const btScalar m_fUpperBaseMass;
      static const btTransform m_cUpperBaseOffset;
      static const btTransform m_cUpperBaseGeometricOffset;
      static const btVector3 m_cLowerBaseToUpperBaseJointOffset;
      static const btVector3 m_cUpperBaseToLowerBaseJointOffset;
      static const btQuaternion m_cLowerBaseToUpperBaseJointRotation;
      /* lift column data */
      static const btVector3 m_cLiftColumnHalfExtents;
      static const btScalar m_fLiftColumnMass;
      static const btTransform m_cLiftColumnOffset;
      static const btTransform m_cLiftColumnGeometricOffset;
      static const btVector3 m_cUpperBaseToLiftColumnJointOffset;
      static const btVector3 m_cLiftColumnToUpperBaseJointOffset;
      static const btQuaternion m_cUpperBaseToLiftColumnJointRotation;
      /* end effector data */
      static const btVector3 m_cEndEffectorHalfExtents;
      static const btScalar m_fEndEffectorMass;
      static const btTransform m_cEndEffectorOffset;
      static const btTransform m_cEndEffectorGeometricOffset;
      static const btVector3 m_cLowerBaseToEndEffectorJointOffset;
      static const btVector3 m_cEndEffectorToLowerBaseJointOffset;
      static const btQuaternion m_cLowerBaseToEndEffectorJointRotation;
      static const btScalar m_fEndEffectorTranslationLimit;
      /* end effector support data */
      static const btVector3 m_cEndEffectorSupportHalfExtents;
      static const btScalar m_fEndEffectorSupportMass;
      static const btTransform m_cEndEffectorSupportOffset;
      static const btTransform m_cEndEffectorSupportGeometricOffset;
      static const btVector3 m_cEndEffectorToEndEffectorSupportJointOffset;
      static const btVector3 m_cEndEffectorSupportToEndEffectorJointOffset;
      static const btQuaternion m_cEndEffectorToEndEffectorSupportJointRotation;
   };
}

#endif
