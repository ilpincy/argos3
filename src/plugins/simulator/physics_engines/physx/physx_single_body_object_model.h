/**
 * @file <argos3/plugins/simulator/physics_engines/physx/physx_singlebody_object_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSX_SINGLE_BODY_OBJECT_MODEL_H
#define PHYSX_SINGLE_BODY_OBJECT_MODEL_H

namespace argos {
   class CPhysXBoxModel;
}

#include <argos3/plugins/simulator/physics_engines/physx/physx_model.h>

namespace argos {

   class CPhysXSingleBodyObjectModel : public CPhysXModel {

   public:

      CPhysXSingleBodyObjectModel(CPhysXEngine& c_engine,
                                  CComposableEntity& c_entity);
      virtual ~CPhysXSingleBodyObjectModel();

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual void Reset();

      virtual void CalculateBoundingBox();

      virtual void UpdateEntityStatus();
      virtual void UpdateFromEntityStatus() {}

      virtual bool IsCollidingWithSomething() const;

      inline CComposableEntity& GetEntity() {
         return m_cEntity;
      }

      inline const CComposableEntity& GetEntity() const {
         return m_cEntity;
      }

      inline physx::PxRigidDynamic* GetBody() {
         return m_pcBody;
      }

      inline const physx::PxRigidDynamic* GetBody() const {
         return m_pcBody;
      }

      inline void SetBody(physx::PxRigidDynamic* pc_body) {
         m_pcBody = pc_body;
      }

      inline physx::PxVec3& GetARGoSReferencePoint() {
         return m_cARGoSReferencePoint;
      }

      inline const physx::PxVec3& GetARGoSReferencePoint() const {
         return m_cARGoSReferencePoint;
      }

      inline void SetARGoSReferencePoint(const physx::PxVec3& c_argos_reference_point) {
         m_cARGoSReferencePoint = c_argos_reference_point;
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

      CComposableEntity&              m_cEntity;
      physx::PxRigidDynamic*          m_pcBody;
      physx::PxVec3                   m_cARGoSReferencePoint;
      std::vector<physx::PxGeometry*> m_vecGeometries;
      std::vector<physx::PxShape*>    m_vecShapes;
   };

}

#endif
