/**
 * @file <argos3/plugins/simulator/physics_engines/physx/physx_multi_body_object_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSX_MULTI_BODY_OBJECT_MODEL_H
#define PHYSX_MULTI_BODY_OBJECT_MODEL_H

namespace argos {
   class CPhysXBoxModel;
}

#include <argos3/plugins/simulator/physics_engines/physx/physx_model.h>

namespace argos {

   class CPhysXMultiBodyObjectModel : public CPhysXModel {

   public:

      CPhysXMultiBodyObjectModel(CPhysXEngine& c_engine,
                                  CComposableEntity& c_entity);
      virtual ~CPhysXMultiBodyObjectModel();

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual void Reset();

      virtual void CalculateBoundingBox();

      virtual void UpdateEntityStatus() {}
      virtual void UpdateFromEntityStatus() {}

      virtual bool IsCollidingWithSomething() const;

      inline CComposableEntity& GetEntity() {
         return m_cEntity;
      }

      inline const CComposableEntity& GetEntity() const {
         return m_cEntity;
      }

      inline std::vector<physx::PxRigidDynamic*>& GetBodies() {
         return m_vecBodies;
      }

      inline const std::vector<physx::PxRigidDynamic*>& GetBodies() const {
         return m_vecBodies;
      }

      inline std::vector<physx::PxGeometry*>& GetGeometries() {
         return m_vecGeometries;
      }

      inline const std::vector<physx::PxGeometry*>& GetGeometries() const {
         return m_vecGeometries;
      }

      inline std::vector<physx::PxShape*>& GetShapes() {
         return m_vecShapes;
      }

      inline const std::vector<physx::PxShape*>& GetShapes() const {
         return m_vecShapes;
      }

   private:

      CComposableEntity&                  m_cEntity;
      std::vector<physx::PxRigidDynamic*> m_vecBodies;
      std::vector<physx::PxGeometry*>     m_vecGeometries;
      std::vector<physx::PxShape*>        m_vecShapes;
   };

}

#endif
