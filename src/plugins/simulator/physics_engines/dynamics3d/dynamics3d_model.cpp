/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_model.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_model.h"

#include <argos3/core/simulator/physics_engine/physics_model.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_engine.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics3DModel::CAbstractBody::SData::SData(const btTransform& c_start_transform,
                                                 const btTransform& c_center_of_mass_offset,
                                                 const btVector3& c_inertia,
                                                 btScalar f_mass,
                                                 btScalar f_friction,
                                                 const std::vector<SDipole>& vec_dipoles) :
      StartTransform(c_start_transform),
      InverseStartTransform(c_start_transform.inverse()),
      CenterOfMassOffset(c_center_of_mass_offset),
      InverseCenterOfMassOffset(c_center_of_mass_offset.inverse()),
      Inertia(c_inertia),
      Mass(f_mass),
      Friction(f_friction),
      Dipoles(vec_dipoles) {}

   /****************************************/
   /****************************************/

   CDynamics3DModel::CAbstractBody::CAbstractBody(CDynamics3DModel& c_model,
                                                  SAnchor* ps_anchor,
                                                  const std::shared_ptr<btCollisionShape>& ptr_shape,
                                                  const SData& s_data) :
      m_cModel(c_model),
      m_psAnchor(ps_anchor),
      m_ptrShape(ptr_shape),
      m_sData(s_data) {}

   /****************************************/
   /****************************************/

   CDynamics3DModel& CDynamics3DModel::CAbstractBody::GetModel() {
      return m_cModel;
   }

   /****************************************/
   /****************************************/

   SAnchor& CDynamics3DModel::CAbstractBody::GetAnchor() {
      return *m_psAnchor;
   }

   /****************************************/
   /****************************************/

   btCollisionShape& CDynamics3DModel::CAbstractBody::GetShape() {
      return *(m_ptrShape.get());
   }

   /****************************************/
   /****************************************/

   const CDynamics3DModel::CAbstractBody::SData& CDynamics3DModel::CAbstractBody::GetData() const {
      return m_sData;
   }

   /****************************************/
   /****************************************/

   void CDynamics3DModel::CAbstractBody::UpdateAnchor() {
      if(m_psAnchor != nullptr) {
         /* Offset the world transform to respect ARGoS conventions */
         btTransform cTransform = 
            GetTransform() * m_sData.CenterOfMassOffset;
         /* Retrieve the position and orientation */
         const btVector3& cPosition = cTransform.getOrigin();
         const btQuaternion& cOrientation = cTransform.getRotation();
         /* Swap coordinate system and set anchor position */
         m_psAnchor->Position.Set(cPosition.getX(), -cPosition.getZ(), cPosition.getY());
         /* Swap coordinate system and set anchor orientation */
         m_psAnchor->Orientation.Set(cOrientation.getW(),
                                     cOrientation.getX(),
                                    -cOrientation.getZ(),
                                     cOrientation.getY());
      }
   }

   /****************************************/
   /****************************************/

   CDynamics3DModel::CDynamics3DModel(CDynamics3DEngine& c_engine,
                                      CComposableEntity& c_entity) :
      CPhysicsModel(c_engine, c_entity.GetComponent<CEmbodiedEntity>("body")),
      m_cEngine(c_engine),
      m_cComposableEntity(c_entity) {}

   /****************************************/
   /****************************************/

   bool CDynamics3DModel::IsCollidingWithSomething() const {
      /* Rerun collision detection */
      m_cEngine.GetWorld().performDiscreteCollisionDetection();
      /* Get the collision dispatcher */
      const btCollisionDispatcher& cDispatcher =
         m_cEngine.GetDispatcher();
      /* For each manifold from the collision dispatcher */
      for(SInt32 i = 0; i < cDispatcher.getNumManifolds(); i++) {
         const btPersistentManifold* pcContactManifold =
            cDispatcher.getManifoldByIndexInternal(i);
         const CDynamics3DModel* pcModelA =
            static_cast<const CDynamics3DModel*>(pcContactManifold->getBody0()->getUserPointer());
         const CDynamics3DModel* pcModelB =
            static_cast<const CDynamics3DModel*>(pcContactManifold->getBody1()->getUserPointer());
         /* Ignore self-collisions */
         if(pcModelA == pcModelB) {
            continue;
         }
         /* Ignore collisions that don't belong to a model (e.g. a collision with the ground) */
         if((pcModelA == nullptr) || (pcModelB == nullptr)) {
            continue;
         }
         /* Only check collisions that involve this model */
         if((pcModelA == this) || (pcModelB == this)) {
            /* One of the two bodies involved in the contact manifold belongs to this model,
               check for contact points with negative distance to indicate a collision */
            for(SInt32 j = 0; j < pcContactManifold->getNumContacts(); j++) {
               const btManifoldPoint& cManifoldPoint = pcContactManifold->getContactPoint(j);
               if (cManifoldPoint.getDistance() < 0.0f) {
                  return true;
               }
            }
         }
      }
      return false;
   }

   /****************************************/
   /****************************************/

   void CDynamics3DModel::UpdateEntityStatus() {
      /* Update the anchor associated with each body */
      for(const std::shared_ptr<CAbstractBody>& ptr_body : m_vecBodies) {
         ptr_body->UpdateAnchor();
      }
      /* Call CPhysicsModel::UpdateEntityStatus which updates the AABB and origin anchor */
      CPhysicsModel::UpdateEntityStatus();
   }
  
   /****************************************/
   /****************************************/

}

