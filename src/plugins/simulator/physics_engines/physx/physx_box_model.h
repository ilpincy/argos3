/**
 * @file <argos3/plugins/simulator/physics_engines/physx/physx_box_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSX_BOX_MODEL_H
#define PHYSX_BOX_MODEL_H

namespace argos {
   class CPhysXBoxModel;
}

#include <argos3/plugins/simulator/physics_engines/physx/physx_model.h>
#include <argos3/plugins/simulator/entities/box_entity.h>

namespace argos {

   class CPhysXBoxModel : public CPhysXModel {

   public:

      CPhysXBoxModel(CPhysXEngine& c_engine,
                     CBoxEntity& c_entity);
      virtual ~CPhysXBoxModel();

      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const;

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual void Reset();

      virtual void CalculateBoundingBox();

      virtual void UpdateEntityStatus();
      virtual void UpdateFromEntityStatus() {}

      virtual bool IsCollidingWithSomething() const;

   private:

      union {
         physx::PxRigidDynamic* m_pcDynamicBody;
         physx::PxRigidStatic* m_pcStaticBody;
      };

      CBoxEntity&   m_cBoxEntity;
      physx::PxVec3 m_cBaseCenterLocal;
      Real          m_fMass;
   };

}

#endif
