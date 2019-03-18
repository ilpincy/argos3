/**
 * @file <argos3/plugins/robots/prototype/simulator/dynamics3d_prototype_model.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_PROTOTYPE_MODEL_H
#define DYNAMICS3D_PROTOTYPE_MODEL_H

namespace argos {
   class CPrototypeEntity;
   class CPrototypeJointEquippedEntity;
}

#include <argos3/plugins/robots/prototype/simulator/prototype_joint_entity.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_multi_body_object_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Featherstone/btMultiBodyJointMotor.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.h>

namespace argos {

   class CDynamics3DPrototypeModel : public CDynamics3DMultiBodyObjectModel {

   public:

      CDynamics3DPrototypeModel(CDynamics3DEngine& c_engine, CPrototypeEntity& c_entity);

      virtual ~CDynamics3DPrototypeModel() {}

      virtual void Reset();
           
      virtual void UpdateEntityStatus();

      virtual void UpdateFromEntityStatus();

      virtual void AddToWorld(btMultiBodyDynamicsWorld& c_world);

      virtual void RemoveFromWorld(btMultiBodyDynamicsWorld& c_world);

   private:

      CAbstractBody::SData CreateBodyData(const CPrototypeLinkEntity& c_link_entity);

      std::shared_ptr<btCollisionShape> RequestShape(const CPrototypeLinkEntity& c_link_entity);

   private:

      CPrototypeEntity& m_cEntity;

      CPrototypeJointEquippedEntity& m_cJointEquippedEntity;

      struct SJoint {
         SJoint(CPrototypeJointEntity::EType e_type,
                std::shared_ptr<CLink>& ptr_parent,
                std::shared_ptr<CLink>& ptr_child,
                const btVector3& c_parent_offset,
                const btVector3& c_child_offset,
                const btQuaternion& c_parent_to_child_rotation,
                const btVector3& c_axis,
                bool b_disable_collision);
         CPrototypeJointEntity::EType Type;
         std::shared_ptr<CLink> Parent;
         std::shared_ptr<CLink> Child;
         btVector3 ParentOffset;
         btVector3 ChildOffset;
         btQuaternion ParentToChildRotation;
         btVector3 Axis;
         bool DisableCollision;
      };

      struct SLimit {
         /* constructor */
         SLimit(CDynamics3DPrototypeModel& c_model,
                Real f_lower_limit,
                Real f_upper_limit,
                SInt32 n_joint_index);
         /* methods */
         void Reset();
         /* data members */
         CDynamics3DPrototypeModel& Model;
         Real LowerLimit;
         Real UpperLimit;
         SInt32 JointIndex;
         btMultiBodyJointLimitConstraint Constraint;
      };

      struct SActuator {
         /* constructor */
         SActuator(CDynamics3DPrototypeModel& c_model,
                   CPrototypeJointEntity::SActuator& s_actuator,
                   SInt32 n_joint_index);
         /* methods */
         void Reset();
         void Update();
         /* members */
         CDynamics3DPrototypeModel& Model;
         CPrototypeJointEntity::SActuator& Actuator;
         SInt32 JointIndex;
         btMultiBodyJointMotor Motor;
      };

      struct SSensor {
         /* constructor */
         SSensor(CDynamics3DPrototypeModel& c_model,
                 CPrototypeJointEntity::SSensor& s_sensor,
                 SInt32 n_joint_index);
         /* methods */
         void Update();
         /* members */
         CDynamics3DPrototypeModel& Model;
         CPrototypeJointEntity::SSensor& Sensor;
         SInt32 JointIndex;
      };

      std::vector<SJoint> m_vecJoints;
      std::vector<SLimit> m_vecLimits;
      std::vector<SActuator> m_vecActuators;
      std::vector<SSensor> m_vecSensors;

   };
}

#endif
