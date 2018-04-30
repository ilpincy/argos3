/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_model.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_MODEL_H
#define DYNAMICS3D_MODEL_H

namespace argos {
   class  CDynamics3DEngine;
}

#include <argos3/core/simulator/physics_engine/physics_model.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/btBulletCollisionCommon.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h>

namespace argos {

   /****************************************/
   /****************************************/

   class CDynamics3DModel : public CPhysicsModel {

   public:

      typedef std::map<std::string, CDynamics3DModel*> TMap;

   public:

   class CAbstractBody {

   public:

         using TVector = std::vector<CAbstractBody*>;

         using TVectorIterator = std::vector<CAbstractBody*>::iterator;

         struct SData {
            /* Constructor */
            SData(const btTransform& c_start_transform,
                  const btTransform& c_center_of_mass_offset,
                  const btVector3& c_inertia,
                  btScalar f_mass,
                  btScalar f_friction);
            /* Data */
            btTransform StartTransform;
            btTransform InverseStartTransform;
            btTransform CenterOfMassOffset;
            btTransform InverseCenterOfMassOffset;
            btVector3 Inertia;
            btScalar Mass;
            btScalar Friction;
         };

      public:

         CAbstractBody(CDynamics3DModel& c_model,
                       SAnchor& s_anchor,
                       std::shared_ptr<btCollisionShape>& ptr_shape,
                       const SData& s_data);

         virtual ~CAbstractBody() {}

         CDynamics3DModel& GetModel();

         SAnchor& GetAnchor();

         btCollisionShape& GetShape();

         const SData& GetData();

         virtual void Reset() = 0;

         virtual void AddToWorld(btMultiBodyDynamicsWorld& c_world) = 0;

         virtual void RemoveFromWorld(btMultiBodyDynamicsWorld& c_world) = 0;

         virtual void ApplyForce(const btVector3& c_force) = 0;

         virtual void ApplyForce(const btVector3& c_force, const btVector3& c_offset) = 0;

         virtual void ApplyTorque(const btVector3& c_torque) = 0;

         virtual btTransform& GetTransform() = 0;

         void UpdateAnchor();

      protected:
         /* Link to the model */
         CDynamics3DModel& m_cModel;
         /* Link to the anchor */
         SAnchor& m_sAnchor;
         /* Link to the collision shape */
         std::shared_ptr<btCollisionShape> m_ptrShape;
         /* Data for setting up the body */
         SData m_sData;
      };

   public:

      CDynamics3DModel(CDynamics3DEngine& c_engine,
                       CComposableEntity& c_entity);

      virtual ~CDynamics3DModel();

      virtual void Reset() = 0;

      inline CDynamics3DEngine& GetEngine() {
         return m_cEngine;
      }

      inline const CDynamics3DEngine& GetEngine() const {
         return m_cEngine;
      }

      inline CComposableEntity& GetComposableEntity() {
         return m_cComposableEntity;
      }

      inline const CComposableEntity& GetComposableEntity() const {
         return m_cComposableEntity;
      }

      virtual bool IsCollidingWithSomething() const;

      virtual void UpdateEntityStatus();

      virtual void UpdateFromEntityStatus() {}

      std::vector<CAbstractBody*>& GetBodies() {
         return m_vecBodies;
      }

      virtual void AddToWorld(btMultiBodyDynamicsWorld& c_world) = 0;

      virtual void RemoveFromWorld(btMultiBodyDynamicsWorld& c_world) = 0;

   protected:

      std::vector<CAbstractBody*> m_vecBodies;

   private:

      CDynamics3DEngine& m_cEngine;
      CComposableEntity& m_cComposableEntity;

   };
   
   /****************************************/
   /****************************************/

}

#endif
