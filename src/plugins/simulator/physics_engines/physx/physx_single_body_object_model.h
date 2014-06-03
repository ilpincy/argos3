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

   protected:

      CComposableEntity&              m_cEntity;
      physx::PxRigidDynamic*          m_pcBody;
      physx::PxVec3                   m_cBaseCenterLocal;
      std::vector<physx::PxGeometry*> m_vecGeometries;
      std::vector<physx::PxShape*>    m_vecShapes;
   };

}

#endif
