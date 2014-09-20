/**
 * @file <argos3/plugins/simulator/physics_engines/physx/physx_single_body_object_model.h>
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

      virtual void Reset();

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual void CalculateBoundingBox();

      virtual void UpdateEntityStatus();
      virtual void UpdateFromEntityStatus() {}

      virtual bool IsCollidingWithSomething() const;

      /**
       * Sets up the internal information for the object model.
       * Call this method once you're done adding shapes to your
       * object model.
       * If you forget to call this method, some of the methods defined in this class
       * won't work (segmentation fault due to NULL pointers).
       * If you call this method before adding all the shapes to the body,
       * some shapes won't be taken into account.
       * @param pc_body The body of the object model.
       */
      void SetupBody(physx::PxRigidActor* pc_body);

      inline CComposableEntity& GetEntity() {
         return m_cEntity;
      }

      inline const CComposableEntity& GetEntity() const {
         return m_cEntity;
      }

      inline physx::PxRigidActor* GetBody() {
         return m_pcGenericBody;
      }

      inline const physx::PxRigidActor* GetBody() const {
         return m_pcGenericBody;
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

      inline std::vector<physx::PxShape*>& GetShapes() {
         return m_vecShapes;
      }

      inline const std::vector<physx::PxShape*>& GetShapes() const {
         return m_vecShapes;
      }

   private:

      CComposableEntity&              m_cEntity;
      union {
         physx::PxRigidActor* m_pcGenericBody;
         physx::PxRigidStatic* m_pcStaticBody;
         physx::PxRigidDynamic* m_pcDynamicBody;
      };
      bool m_bIsDynamic;
      physx::PxVec3                   m_cARGoSReferencePoint;
      std::vector<physx::PxShape*>    m_vecShapes;
   };

}

#endif
