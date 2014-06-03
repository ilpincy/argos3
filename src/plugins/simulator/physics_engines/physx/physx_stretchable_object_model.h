/**
 * @file <argos3/plugins/simulator/physics_engines/physx/physx_stretchable_object_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSX_STRETCHABLE_OBJECT_MODEL_H
#define PHYSX_STRETCHABLE_OBJECT_MODEL_H

namespace argos {
   class CPhysXBoxModel;
}

#include <argos3/plugins/simulator/physics_engines/physx/physx_single_body_object_model.h>

namespace argos {

   /**
    * A stretchable object model for PhysX.
    * This template class is used by CPhysXBoxModel and CPhysXCylinderModel
    * to share common code.
    * If you want to add an additional stretchable object, you just need to
    * subclass this template and implement the constructor similarly to
    * what is done in CPhysXBoxModel and CPhysXCylinderModel.
    * @see CPhysXBoxModel
    * @see CPhysXCylinderModel
    */
   template<class ENTITY>
   class CPhysXStretchableObjectModel : public CPhysXSingleBodyObjectModel {

   public:

      CPhysXStretchableObjectModel(CPhysXEngine& c_engine,
                                   ENTITY& c_entity);
      virtual ~CPhysXStretchableObjectModel();

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual void Reset();

      virtual void CalculateBoundingBox();

      virtual void UpdateEntityStatus();

      virtual bool IsCollidingWithSomething() const;

   protected:

      union {
         physx::PxRigidActor*   m_pcGenericBody;
         physx::PxRigidDynamic* m_pcDynamicBody;
         physx::PxRigidStatic*  m_pcStaticBody;
      };
      Real m_fMass;
   };

}

/*
 * Include template implementation
 */
#include <argos3/plugins/simulator/physics_engines/physx/physx_stretchable_object_model_impl.h>

#endif
