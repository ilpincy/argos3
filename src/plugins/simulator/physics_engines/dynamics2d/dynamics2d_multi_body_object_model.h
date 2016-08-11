/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_multi_body_object_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_MULTI_BODY_OBJECT_MODEL_H
#define DYNAMICS2D_MULTI_BODY_OBJECT_MODEL_H

namespace argos {
   class CDynamics2DMultiBodyObjectModel;
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/chipmunk-physics/include/chipmunk.h>

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
    * because ~CDynamics2DMultiBodyObjectModel() does it for you.
    * </p>
    */
   class CDynamics2DMultiBodyObjectModel : public CDynamics2DModel {

   public:

      struct SBody {
         cpBody* Body;
         cpVect  OffsetPos;
         cpFloat OffsetOrient;
         Real    Height;
         SBody(cpBody* pt_body,
               const cpVect& t_offset_pos,
               cpFloat t_offset_orient,
               Real f_height);
      };

   public:

      /**
       * Class constructor.
       * @param c_engine The dynamics 2D engine that manages this model.
       * @param c_entity The composable entity associated to this model.
       * @throws CARGoSException if c_entity does not contain an embodied entity.
       */
      CDynamics2DMultiBodyObjectModel(CDynamics2DEngine& c_engine,
                                      CComposableEntity& c_entity);

      /**
       * Class destructor.
       * Disposes of the object body and its shapes.
       */
      virtual ~CDynamics2DMultiBodyObjectModel();

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

      /**
       * Returns the i-th body associated to the model.
       * @return The i-th body associated to the model.
       */
      inline SBody& GetBody(size_t i) {
         return m_vecBodies[i];
      }
      
      /**
       * Returns the i-th body associated to the model.
       * @return The i-th body associated to the model.
       */
      inline const SBody& GetBody(size_t i) const {
         return m_vecBodies[i];
      }
      
      virtual void Reset();

      virtual void MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation);

      virtual void CalculateBoundingBox();

      virtual void UpdateFromEntityStatus()  = 0;

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
       * @param pt_body The object body.
       * @param t_offset_pos The position of the body wrt the origin anchor.
       * @param t_offset_orient The orientation of the body wrt the origin anchor.
       * @param f_height The object height.
       */
      virtual void AddBody(cpBody* pt_body,
                           const cpVect& t_offset_pos,
                           cpFloat t_offset_orient,
                           Real f_height);

   private:

      CComposableEntity& m_cEntity;
      std::vector<SBody> m_vecBodies;
   };

}

#endif
