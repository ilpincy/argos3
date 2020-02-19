/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_multi_body_object_model.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_MULTI_BODY_OBJECT_MODEL_H
#define DYNAMICS3D_MULTI_BODY_OBJECT_MODEL_H

namespace argos {
   class CDynamics3DMultiBodyObjectModel;
}

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_model.h>

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreorder"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
#endif

#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Featherstone/btMultiBody.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Featherstone/btMultiBodyLink.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Featherstone/btMultiBodyLinkCollider.h>

#ifdef __APPLE__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif

namespace argos {

   class CDynamics3DMultiBodyObjectModel : public CDynamics3DModel {

   public:

      class CLink : public CAbstractBody {

      public:

         CLink(CDynamics3DMultiBodyObjectModel& c_model,
               UInt32 un_link_index,
               SAnchor* ps_anchor,
               const std::shared_ptr<btCollisionShape>& ptr_shape,
               const SData& s_data);

         virtual ~CLink() {}

         virtual void Reset();

         virtual void AddToWorld(btMultiBodyDynamicsWorld& c_world);

         virtual void RemoveFromWorld(btMultiBodyDynamicsWorld& c_world);

         virtual void ApplyForce(const btVector3& c_force);

         virtual void ApplyForce(const btVector3& c_force,
                                 const btVector3& c_offset);

         virtual void ApplyTorque(const btVector3& c_torque);

         virtual btTransform& GetTransform();

         UInt32 GetIndex() const;

      protected:

         CDynamics3DMultiBodyObjectModel& m_cModel;
         UInt32 m_unLinkIndex;
         btMultiBodyLinkCollider m_cMultiBodyLink;
      };

      class CBase : public CLink {

      public:

         CBase(CDynamics3DMultiBodyObjectModel& c_model,
               SAnchor* ps_anchor,
               const std::shared_ptr<btCollisionShape>& ptr_shape,
               const SData& s_data);

         virtual ~CBase() {}

         virtual void Reset();

         virtual void ApplyForce(const btVector3& c_force);

         virtual void ApplyForce(const btVector3& c_force,
                                 const btVector3& c_offset);

         virtual void ApplyTorque(const btVector3& c_torque);
      };

      CDynamics3DMultiBodyObjectModel(CDynamics3DEngine& c_engine,
                                      CComposableEntity& c_entity,
                                      UInt32 un_num_links,
                                      bool b_fixed_base);

      virtual ~CDynamics3DMultiBodyObjectModel() {}

      virtual void Reset();

      virtual void MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation);

      virtual void CalculateBoundingBox();

      virtual void AddToWorld(btMultiBodyDynamicsWorld& c_world);

      virtual void RemoveFromWorld(btMultiBodyDynamicsWorld& c_world);

      btMultiBody& GetMultiBody() {
         return m_cMultiBody;
      }

      virtual void UpdateOriginAnchor(SAnchor& s_anchor);

   protected:

      btMultiBody m_cMultiBody;
      UInt32 m_unNumLinks;
      bool m_bFixedBase;
   };

}

#endif
