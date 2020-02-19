/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_single_body_object_model.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_SINGLE_BODY_OBJECT_MODEL_H
#define DYNAMICS3D_SINGLE_BODY_OBJECT_MODEL_H

namespace argos {
   class CDynamics3DSingleBodyObjectModel;
}

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_model.h>

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreorder"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
#endif

#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Dynamics/btRigidBody.h>

#ifdef __APPLE__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif

namespace argos {

   class CDynamics3DSingleBodyObjectModel : public CDynamics3DModel {

   public:

      class CBody : public CAbstractBody {

      public:

         CBody(CDynamics3DModel& c_model,
               SAnchor* ps_anchor,
               const std::shared_ptr<btCollisionShape>& ptr_shape,
               const SData& s_data);

         virtual ~CBody() {}

         virtual void Reset();

         virtual void AddToWorld(btMultiBodyDynamicsWorld& c_world);

         virtual void RemoveFromWorld(btMultiBodyDynamicsWorld& c_world);

         virtual void ApplyForce(const btVector3& c_force);

         virtual void ApplyForce(const btVector3& c_force,
                                 const btVector3& c_offset);

         virtual void ApplyTorque(const btVector3& c_torque);

         virtual btTransform& GetTransform();

      protected:

         btRigidBody m_cRigidBody;

      };

   public:

      CDynamics3DSingleBodyObjectModel(CDynamics3DEngine& c_engine,
                                       CComposableEntity& c_entity);

      virtual ~CDynamics3DSingleBodyObjectModel() {}

      virtual void Reset();

      virtual void MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation);

      virtual void CalculateBoundingBox();

      virtual void AddToWorld(btMultiBodyDynamicsWorld& c_world);

      virtual void RemoveFromWorld(btMultiBodyDynamicsWorld& c_world);

   };
}

#endif
