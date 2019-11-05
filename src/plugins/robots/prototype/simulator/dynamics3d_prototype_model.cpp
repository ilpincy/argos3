/**
 * @file <argos3/plugins/robots/prototype/simulator/dynamics3d_prototype_model.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Weixu Zhu- <zhuweixu_harry@126.com>
 */

#include "dynamics3d_prototype_model.h"
#include <argos3/plugins/robots/prototype/simulator/prototype_entity.h>
#include <argos3/plugins/robots/prototype/simulator/prototype_joint_equipped_entity.h>
#include <argos3/plugins/robots/prototype/simulator/prototype_link_equipped_entity.h>
#include <argos3/plugins/simulator/entities/magnet_equipped_entity.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_multi_body_object_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_shape_manager.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_engine.h>

namespace argos {

   /****************************************/
   /****************************************/

   std::shared_ptr<btCollisionShape> CDynamics3DPrototypeModel::RequestShape(const CPrototypeLinkEntity& c_link_entity) {
      btVector3 cHalfExtents(c_link_entity.GetExtents().GetX() * 0.5f,
                             c_link_entity.GetExtents().GetZ() * 0.5f,
                             c_link_entity.GetExtents().GetY() * 0.5f);
      std::vector<btVector3> vecConvexHullPoints;
      std::shared_ptr<btCollisionShape> ptrShape;
      switch(c_link_entity.GetGeometry()) {
      case CPrototypeLinkEntity::EGeometry::BOX:
         ptrShape = CDynamics3DShapeManager::RequestBox(cHalfExtents);
         break;
      case CPrototypeLinkEntity::EGeometry::CYLINDER:
         ptrShape = CDynamics3DShapeManager::RequestCylinder(cHalfExtents);
         break;
      case CPrototypeLinkEntity::EGeometry::SPHERE:
         ptrShape = CDynamics3DShapeManager::RequestSphere(cHalfExtents.getZ());
         break;
      case CPrototypeLinkEntity::EGeometry::CONVEX_HULL:
         vecConvexHullPoints.reserve(c_link_entity.GetConvexHullPoints().size());
         for(const CVector3& c_point : c_link_entity.GetConvexHullPoints()) {
            vecConvexHullPoints.emplace_back(c_point.GetX(),
                                             c_point.GetZ(),
                                            -c_point.GetY());
         }
         ptrShape = CDynamics3DShapeManager::RequestConvexHull(vecConvexHullPoints);
         break;
      default:
         THROW_ARGOSEXCEPTION("Collision shape geometry not implemented");
         break;
      }
      return ptrShape;
   }

   /****************************************/
   /****************************************/

   CDynamics3DMultiBodyObjectModel::CAbstractBody::SData
      CDynamics3DPrototypeModel::CreateBodyData(const CPrototypeLinkEntity& c_link_entity) {
      /* Get friction */
      btScalar fFriction = GetEngine().GetDefaultFriction();
      /* Calculate inertia */
      btScalar fMass = c_link_entity.GetMass();
      btVector3 cInertia;
      RequestShape(c_link_entity)->calculateLocalInertia(fMass, cInertia);
      /* Calculate start transform */
      const CVector3& cPosition = c_link_entity.GetAnchor().Position;
      const CQuaternion& cOrientation = c_link_entity.GetAnchor().Orientation;
      btTransform cStartTransform(btQuaternion(cOrientation.GetX(),
                                               cOrientation.GetZ(),
                                              -cOrientation.GetY(),
                                               cOrientation.GetW()),
                                  btVector3(cPosition.GetX(),
                                            cPosition.GetZ(),
                                           -cPosition.GetY()));
      /* Calculate center of mass offset */
      btTransform cCenterOfMassOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
                                      btVector3(0.0f, -c_link_entity.GetExtents().GetZ() * 0.5f, 0.0f));
      std::vector<CAbstractBody::SData::SDipole> vecDipoles;
      /* check if this link is magnetic */
      if(m_cEntity.HasMagnetEquippedEntity()) {
         CMagnetEquippedEntity::SInstance::TVector& vecInstances =
            m_cEntity.GetMagnetEquippedEntity().GetInstances();
         for(CMagnetEquippedEntity::SInstance& s_instance : vecInstances) {
            /* check if the dipole/magnet belongs to this link by comparing the anchors */
            if(s_instance.Anchor.Index == c_link_entity.GetAnchor().Index) {
               btTransform cOffset(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
                                   btVector3(s_instance.Offset.GetX(),
                                             s_instance.Offset.GetZ(),
                                            -s_instance.Offset.GetY()));
               cOffset *= cCenterOfMassOffset;
               std::function<btVector3()> fnGetField = [&s_instance] () {
                  const CVector3& cField = s_instance.Magnet.GetField();
                  return btVector3(cField.GetX(), cField.GetZ(), -cField.GetY());
               };
               vecDipoles.emplace_back(fnGetField, cOffset);
            }
         }
      }
      /* Return link data */
      return CAbstractBody::SData(cStartTransform, cCenterOfMassOffset, cInertia, fMass, fFriction, vecDipoles);

   }

   /****************************************/
   /****************************************/

   CDynamics3DPrototypeModel::CDynamics3DPrototypeModel(CDynamics3DEngine& c_engine,
                                                        CPrototypeEntity& c_entity) :
      CDynamics3DMultiBodyObjectModel(c_engine,
                                      c_entity,
                                      c_entity.GetLinkEquippedEntity().GetLinks().size() - 1,
                                      !c_entity.GetEmbodiedEntity().IsMovable()),
      m_cEntity(c_entity),
      m_cJointEquippedEntity(c_entity.GetJointEquippedEntity()) {
      /* Use the reference link as the base of the robot */
      CPrototypeLinkEntity& cBaseLink = c_entity.GetLinkEquippedEntity().GetReferenceLink();
      /* Get the collision shape */
      std::shared_ptr<btCollisionShape> ptrBaseLinkShape = RequestShape(cBaseLink);
      /* Set up the base link body */
      std::shared_ptr<CBase> ptrBase = 
         std::make_shared<CBase>(*this, &cBaseLink.GetAnchor(), ptrBaseLinkShape, CreateBodyData(cBaseLink));
      /* Add to collection */
      m_vecBodies.push_back(ptrBase);
      /* Counter for enumerating the links inside the btMultiBody */
      UInt32 unLinkIndex = 0;
      /* Copy all joints pointers into a temporary vector */
      CPrototypeJointEntity::TVector vecJointsToAdd =
         m_cJointEquippedEntity.GetJoints();
      /* While there are joints to be added */
      while(! vecJointsToAdd.empty()) {
         size_t unRemainingJoints = vecJointsToAdd.size();
         for(CPrototypeJointEntity::TVectorIterator it_joint = std::begin(vecJointsToAdd);
             it_joint != std::end(vecJointsToAdd);
             ++it_joint) {
            /* Get a reference to the joint */
            CPrototypeJointEntity& cJoint = (**it_joint);
            /* Get a reference to the parent link entity */
            const CPrototypeLinkEntity& cParentLink = cJoint.GetParentLink();
            /* Check if the parent link has been added */
            CAbstractBody::TVectorIterator itParentLinkBody =
               std::find_if(std::begin(m_vecBodies),
                            std::end(m_vecBodies),
                            [&cParentLink] (const std::shared_ptr<CAbstractBody>& ptr_body) {
               return (cParentLink.GetAnchor().Index == ptr_body->GetAnchor().Index);
            });
            /* If the parent link hasn't been added yet, try the next joint */
            if(itParentLinkBody == std::end(m_vecBodies)) {
               continue;
            }
            /* Parent body must be a CLink, therefore no need to use dynamic_pointer_cast */
            std::shared_ptr<CLink> ptrParentLinkBody = 
               std::static_pointer_cast<CLink>(*itParentLinkBody);
            /* Get a reference to the child link */
            CPrototypeLinkEntity& cChildLink = cJoint.GetChildLink();
            /* Get the collision shape */
            std::shared_ptr<btCollisionShape> ptrChildLinkShape =
               RequestShape(cChildLink);
            /* Set up the child link body */
            std::shared_ptr<CLink> ptrChildLinkBody =
               std::make_shared<CLink>(*this,
                                       unLinkIndex++,
                                       &cChildLink.GetAnchor(),
                                       ptrChildLinkShape,
                                       CreateBodyData(cChildLink));
            /* Add to collection */
            m_vecBodies.push_back(ptrChildLinkBody);
            /* Calculate joint parameters for parent link */
            const CVector3& cParentOffsetPosition = cJoint.GetParentLinkJointPosition();
            const CQuaternion& cParentOffsetOrientation = cJoint.GetParentLinkJointOrientation();
            btTransform cParentOffsetTransform =
               btTransform(btQuaternion(cParentOffsetOrientation.GetX(),
                                        cParentOffsetOrientation.GetZ(),
                                       -cParentOffsetOrientation.GetY(),
                                        cParentOffsetOrientation.GetW()),
                           btVector3(cParentOffsetPosition.GetX(),
                                     cParentOffsetPosition.GetZ(),
                                    -cParentOffsetPosition.GetY()));
            cParentOffsetTransform = ptrParentLinkBody->GetData().CenterOfMassOffset * cParentOffsetTransform;
            /* Calculate joint parameters for child link */
            const CVector3& cChildOffsetPosition = cJoint.GetChildLinkJointPosition();
            const CQuaternion& cChildOffsetOrientation = cJoint.GetChildLinkJointOrientation();
            btTransform cChildOffsetTransform =
               btTransform(btQuaternion(cChildOffsetOrientation.GetX(),
                                        cChildOffsetOrientation.GetZ(),
                                       -cChildOffsetOrientation.GetY(),
                                        cChildOffsetOrientation.GetW()),
                           btVector3(cChildOffsetPosition.GetX(),
                                     cChildOffsetPosition.GetZ(),
                                    -cChildOffsetPosition.GetY()));
            cChildOffsetTransform = ptrChildLinkBody->GetData().CenterOfMassOffset * cChildOffsetTransform;
            /* Calculate the joint axis */
            btVector3 cJointAxis(cJoint.GetJointAxis().GetX(),
                                 cJoint.GetJointAxis().GetZ(),
                                -cJoint.GetJointAxis().GetY());
            /* Calculate the parent to child joint rotation */
            /*
            // From testing, these both the of following solutions seem correct although one may be wrong
            btQuaternion cParentToChildRotation = cParentOffsetTransform.inverse().getRotation() *
               cChildOffsetTransform.getRotation();
            */
            btQuaternion cParentToChildRotation = cChildOffsetTransform.getRotation() * 
               cParentOffsetTransform.inverse().getRotation();
            /* Store joint configuration for reset */
            m_vecJoints.emplace_back(cJoint.GetType(),
                                     ptrParentLinkBody,
                                     ptrChildLinkBody,
                                     cParentOffsetTransform.getOrigin(),
                                     -cChildOffsetTransform.getOrigin(),
                                     cParentToChildRotation,
                                     cJointAxis,
                                     cJoint.GetDisableCollision());
            /* Sensor and actuator configuration for revolute and prismatic joints */
            if(cJoint.GetType() == CPrototypeJointEntity::EType::REVOLUTE ||
               cJoint.GetType() == CPrototypeJointEntity::EType::PRISMATIC) {
               /* If the joint actuator isn't disabled */
               CPrototypeJointEntity::SActuator& sActuator = cJoint.GetActuator();
                  if(sActuator.Mode != CPrototypeJointEntity::SActuator::EMode::DISABLED) {
                  m_vecActuators.emplace_back(*this, sActuator, ptrChildLinkBody->GetIndex());
               }
               /* And if the joint sensor isn't disabled */
               CPrototypeJointEntity::SSensor& sSensor = cJoint.GetSensor();
               if(sSensor.Mode != CPrototypeJointEntity::SSensor::EMode::DISABLED) {
                  m_vecSensors.emplace_back(*this, sSensor, ptrChildLinkBody->GetIndex());
               }
            }
            /* Joint limits */
            if(cJoint.HasLimit()) {
               if(cJoint.GetType() == CPrototypeJointEntity::EType::REVOLUTE) {
                  const CRange<CRadians>& cLimit = cJoint.GetLimit().Revolute;
                  m_vecLimits.emplace_back(*this,
                                           cLimit.GetMin().GetValue(),
                                           cLimit.GetMax().GetValue(),
                                           ptrChildLinkBody->GetIndex());
               }
               else if(cJoint.GetType() == CPrototypeJointEntity::EType::PRISMATIC) {
                  const CRange<Real>& cLimit = cJoint.GetLimit().Prismatic;
                  m_vecLimits.emplace_back(*this,
                                           cLimit.GetMin(),
                                           cLimit.GetMax(),
                                           ptrChildLinkBody->GetIndex());
               }
            }
            /* Now that the joint and link has been added we remove it from the vector
               and restart the loop */
            vecJointsToAdd.erase(it_joint);
            break;
         }
         if(unRemainingJoints == vecJointsToAdd.size()) {
            CPrototypeJointEntity* pcJoint = vecJointsToAdd.front();
            THROW_ARGOSEXCEPTION("Can not add link \"" <<
                                 pcJoint->GetChildLink().GetId() <<
                                 "\" to the model before its parent link \"" <<
                                 pcJoint->GetParentLink().GetId() <<
                                 "\" has been added.");
         }
      }
      for(CPrototypeLinkEntity* pc_link : m_cEntity.GetLinkEquippedEntity().GetLinks()) {
         if(std::find_if(std::begin(m_vecBodies),
                         std::end(m_vecBodies),
                         [pc_link] (std::shared_ptr<CAbstractBody>& ptr_body) {
            return (pc_link->GetAnchor().Index == ptr_body->GetAnchor().Index);
         }) == std::end(m_vecBodies)) {
            THROW_ARGOSEXCEPTION("Link \"" << pc_link->GetId() <<
                                 "\" is not connected to the model.");

         }
      }
      /* Finalize model with a reset */
      Reset();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DPrototypeModel::AddToWorld(btMultiBodyDynamicsWorld& c_world) {
      /* Call CDynamics3DMultiBodyObjectModel::AddToWorld to add all bodies to the world */
      CDynamics3DMultiBodyObjectModel::AddToWorld(c_world);
      /* Add the actuators (btMultiBodyJointMotors) constraints to the world */
      for(SActuator& s_actuator : m_vecActuators) {
         c_world.addMultiBodyConstraint(&s_actuator.Motor);
      }
      /* Add the joint limits to the world */
      for(SLimit& s_limit : m_vecLimits) {
         c_world.addMultiBodyConstraint(&s_limit.Constraint);
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics3DPrototypeModel::RemoveFromWorld(btMultiBodyDynamicsWorld& c_world) {
      /* Remove the joint limits to the world */
      for(SLimit& s_limit : m_vecLimits) {
         c_world.removeMultiBodyConstraint(&s_limit.Constraint);
      }
      /* Remove the actuators (btMultiBodyJointMotors) constraints to the world */
      for(SActuator& s_actuator : m_vecActuators) {
         c_world.removeMultiBodyConstraint(&s_actuator.Motor);
      }
      /* Call CDynamics3DMultiBodyObjectModel::RemoveFromWorld to remove all bodies to the world */
      CDynamics3DMultiBodyObjectModel::RemoveFromWorld(c_world);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DPrototypeModel::Reset() {
      /* Reset the base class (recreates the btMultiBody and calls reset on the bodies) */
      CDynamics3DMultiBodyObjectModel::Reset();
      /* Add setup the links and joints */
      for(SJoint& s_joint : m_vecJoints) {
         switch(s_joint.Type) {
         case CPrototypeJointEntity::EType::FIXED:
            m_cMultiBody.setupFixed(s_joint.Child->GetIndex(),
                                    s_joint.Child->GetData().Mass,
                                    s_joint.Child->GetData().Inertia,
                                    s_joint.Parent->GetIndex(),
                                    s_joint.ParentToChildRotation,
                                    s_joint.ParentOffset,
                                    s_joint.ChildOffset);
            break;
         case CPrototypeJointEntity::EType::SPHERICAL:
            m_cMultiBody.setupSpherical(s_joint.Child->GetIndex(),
                                        s_joint.Child->GetData().Mass,
                                        s_joint.Child->GetData().Inertia,
                                        s_joint.Parent->GetIndex(),
                                        s_joint.ParentToChildRotation,
                                        s_joint.ParentOffset,
                                        s_joint.ChildOffset,
                                        s_joint.DisableCollision);
            break;
         case CPrototypeJointEntity::EType::REVOLUTE:
            m_cMultiBody.setupRevolute(s_joint.Child->GetIndex(),
                                       s_joint.Child->GetData().Mass,
                                       s_joint.Child->GetData().Inertia,
                                       s_joint.Parent->GetIndex(),
                                       s_joint.ParentToChildRotation,
                                       s_joint.Axis,
                                       s_joint.ParentOffset,
                                       s_joint.ChildOffset,
                                       s_joint.DisableCollision);
            break;
         case CPrototypeJointEntity::EType::PRISMATIC:
            m_cMultiBody.setupPrismatic(s_joint.Child->GetIndex(),
                                        s_joint.Child->GetData().Mass,
                                        s_joint.Child->GetData().Inertia,
                                        s_joint.Parent->GetIndex(),
                                        s_joint.ParentToChildRotation,
                                        s_joint.Axis,
                                        s_joint.ParentOffset,
                                        s_joint.ChildOffset,
                                        s_joint.DisableCollision);
            break;
         default:
            break;
         }
      }
      /* Reset the actuators */
      for(SActuator& s_actuator : m_vecActuators) {
         s_actuator.Reset();
      }
      /* Reset the joint limits */
      for(SLimit& s_limit : m_vecLimits) {
         s_limit.Reset();
      }
      /* Allocate memory and prepare the btMultiBody */
      m_cMultiBody.finalizeMultiDof();
      /* Synchronize with the entity in the space */
      UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DPrototypeModel::UpdateEntityStatus() {
      /* Write back sensor data to the joints */
      for(SSensor& s_sensor : m_vecSensors) {
         s_sensor.Update();
      }
      /* Update anchors using the base class method */
      CDynamics3DMultiBodyObjectModel::UpdateEntityStatus();
   }

   /****************************************/
   /****************************************/

   void CDynamics3DPrototypeModel::UpdateFromEntityStatus() {
      /* Read in actuator data from the joints */
      for(SActuator& s_actuator : m_vecActuators) {
         s_actuator.Update();
      }
   }

   /****************************************/
   /****************************************/

   CDynamics3DPrototypeModel::SJoint::SJoint(CPrototypeJointEntity::EType e_type,
                                             std::shared_ptr<CLink>& ptr_parent,
                                             std::shared_ptr<CLink>& ptr_child,
                                             const btVector3& c_parent_offset,
                                             const btVector3& c_child_offset,
                                             const btQuaternion& c_parent_to_child_rotation,
                                             const btVector3& c_axis,
                                             bool b_disable_collision) :
      Type(e_type),
      Parent(ptr_parent),
      Child(ptr_child),
      ParentOffset(c_parent_offset),
      ChildOffset(c_child_offset),
      ParentToChildRotation(c_parent_to_child_rotation),
      Axis(c_axis),
      DisableCollision(b_disable_collision) {}

   /****************************************/
   /****************************************/

   CDynamics3DPrototypeModel::SLimit::SLimit(CDynamics3DPrototypeModel& c_model,
                                             Real f_lower_limit,
                                             Real f_upper_limit,
                                             SInt32 n_joint_index) :
      Model(c_model),
      LowerLimit(f_lower_limit),
      UpperLimit(f_upper_limit),
      JointIndex(n_joint_index),
      Constraint(&c_model.GetMultiBody(),
                 n_joint_index,
                 f_lower_limit,
                 f_upper_limit) {}

   /****************************************/
   /****************************************/

   void CDynamics3DPrototypeModel::SLimit::Reset() {
      Constraint.~btMultiBodyJointLimitConstraint();
      new (&Constraint) btMultiBodyJointLimitConstraint(&Model.GetMultiBody(),
                                                        JointIndex,
                                                        LowerLimit,
                                                        UpperLimit);
   }

   /****************************************/
   /****************************************/

   CDynamics3DPrototypeModel::SActuator::SActuator(CDynamics3DPrototypeModel& c_model,
             CPrototypeJointEntity::SActuator& s_actuator,
             SInt32 n_joint_index) :
      Model(c_model),
      Actuator(s_actuator),
      JointIndex(n_joint_index),
      Motor(&c_model.GetMultiBody(),
            n_joint_index,
            0,
            s_actuator.Target,
            s_actuator.MaxImpulse) {}

   /****************************************/
   /****************************************/

   void CDynamics3DPrototypeModel::SActuator::Reset() {
      Motor.~btMultiBodyJointMotor();
      new (&Motor) btMultiBodyJointMotor(&Model.GetMultiBody(),
                                        JointIndex,
                                        0,
                                        Actuator.Target,
                                        Actuator.MaxImpulse);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DPrototypeModel::SActuator::Update() {
      if(Actuator.Mode == CPrototypeJointEntity::SActuator::EMode::POSITION) {
         Motor.setPositionTarget(Actuator.Target);
      }
      else if(Actuator.Mode == CPrototypeJointEntity::SActuator::EMode::VELOCITY) {
         Motor.setVelocityTarget(Actuator.Target);
      }
   }

   /****************************************/
   /****************************************/

   CDynamics3DPrototypeModel::SSensor::SSensor(CDynamics3DPrototypeModel& c_model,
           CPrototypeJointEntity::SSensor& s_sensor,
           SInt32 n_joint_index) :
      Model(c_model),
      Sensor(s_sensor),
      JointIndex(n_joint_index) {}

   /****************************************/
   /****************************************/

   void CDynamics3DPrototypeModel::SSensor::Update() {
      if(Sensor.Mode == CPrototypeJointEntity::SSensor::EMode::POSITION) {
         Sensor.Value = Model.GetMultiBody().getJointPos(JointIndex);
      }
      else if(Sensor.Mode == CPrototypeJointEntity::SSensor::EMode::VELOCITY) {
         Sensor.Value = Model.GetMultiBody().getJointVel(JointIndex);
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS3D_OPERATIONS_ON_ENTITY(CPrototypeEntity, CDynamics3DPrototypeModel);

   /****************************************/
   /****************************************/

}

