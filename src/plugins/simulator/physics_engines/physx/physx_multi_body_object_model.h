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

   /**
    * Base class for object models with multiple bodies.
    * <p>
    * This class offers all the basic functionality to manage
    * models composed of multiple bodies.
    * </p>
    * <p>
    * Multi-body objects are often active objects such as robots.
    * </p>
    * <p>
    * To use this class, simply create a class that inherits from it.
    * The default implementations for Reset(), MoveTo(), etc. should
    * be already OK for your needs.
    * </p>
    * <p>
    * In the constructor of your class, be sure to call the method AddBody()
    * when you have created the bodies and associated the shapes to them. If this
    * method is not called, the default implementations will have segfaults.
    * </p>
    * <p>
    * This class retains ownership for the bodies and shapes you created once
    * you call AddBody(). In other words, you don't need to free these objects
    * because ~CPhysXSingleBodyObjectModel() does it for you.
    * </p>
    */
   class CPhysXMultiBodyObjectModel : public CPhysXModel {

   public:

      /**
       * Class constructor.
       * @param c_engine The dynamics 2D engine that manages this model.
       * @param c_entity The composable entity associated to this model.
       * @param un_num_bodies The number of bodies that compose this model.
       * @throws CARGoSException if c_entity does not contain an embodied entity.
       */
      CPhysXMultiBodyObjectModel(CPhysXEngine& c_engine,
                                 CComposableEntity& c_entity,
                                 UInt32 un_num_bodies);

      /**
       * Class destructor.
       * Disposes of the object body and its shapes.
       */
      virtual ~CPhysXMultiBodyObjectModel();

      /**
       * Returns the associated composable entity as a non-const reference.
       * @returns The associated composable entity as a non-const reference.
       */
      inline CComposableEntity& GetComposableEntity() {
         return m_cEntity;
      }

      /**
       * Returns the associated composable entity as a const reference.
       * @returns The associated composable entity as a const reference.
       */
      inline const CComposableEntity& GetComposableEntity() const {
         return m_cEntity;
      }

      virtual void Reset();

      virtual void MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation);

      virtual void CalculateBoundingBox();

      virtual void UpdateFromEntityStatus() {}

      virtual bool IsCollidingWithSomething() const;

      /**
       * Adds a body.
       * <p>
       * You must call this method for this class' methods to work.
       * </p>
       * <p>
       * This class retains ownership of the passed body, so you don't
       * need to explicitly delete anything.
       * </p>
       * <p>
       * Internally, this method also sets <tt>pc_body->userData</tt> to
       * point to <tt>this</tt>, which is a requirement for ray
       * cast queries to work properly.
       * </p>
       * <p>
       * Finally, this method also calculates the initial bounding
       * box of the object.
       * </p>
       * <p>
       * @param c_body The object body.
       * @param c_trans The transform of the body wrt the origin anchor.
       */
      void AddBody(physx::PxRigidDynamic& c_body,
                   const physx::PxTransform& c_trans);

   private:

      struct SBody {
         physx::PxRigidDynamic& Body;
         physx::PxTransform Offset;
         SBody(physx::PxRigidDynamic& c_body,
               const physx::PxTransform& c_offset);
      };

   private:

      CComposableEntity& m_cEntity;
      physx::PxAggregate* m_pcAggregate;
      std::vector<SBody> m_vecBodies;
   };

}

#endif
