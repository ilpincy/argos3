#ifndef POSITIONAL_INDEX_H
#define POSITIONAL_INDEX_H

namespace argos {
   class CVector2;
   class CVector3;
   class CRay3;
}

#include <argos3/core/utility/configuration/base_configurable_resource.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/datatypes/set.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/simulator/entity/entity.h>

#include <string>

namespace argos {

   /**
    * A data structure that contains positional entities.
    * This interface defines the basic operations a data structure
    * can do on positional entities. It is meant to provide methods
    * to perform operations on entities within given ranges.
    * @see CPositionalEntity
    * @see CEmbodiedEntity
    */
   template<class ENTITY>
   class CPositionalIndex : CBaseConfigurableResource {

   public:

      /**
       * The operation to perform on each entity found in range.
       * The operation gets as input a reference to an entity. The return
       * value is either <tt>true</tt> or <tt>false</tt>. The former means
       * that the processing should continue with the next entity. The
       * latter means that processing is finished. For example, this code
       * prints the id of all the embodied entities in a sphere centered
       * in zero and with 1-meter radius:
       * <code>
       *    class CMyOperation : public CPositionalIndex<CEmbodiedEntity>::COperation {
       *    public:
       *       virtual bool operator()(CEmbodiedEntity& e) {
       *          LOG << e.GetId() << std::endl;
       *          return true;
       *       }
       *    };
       *    ...
       *    CPositionalIndex<CEmbodiedEntity>& i = ...
       *    i.ForEntitiesInSphereRange(CVector3(), 1, CMyOperation);
       * </code>
       * @see ForEntitiesInSphereRange()
       * @see ForEntitiesInBoxRange()
       * @see ForEntitiesInCircleRange()
       * @see ForEntitiesInRectangleRange()
       * @see ForEntitiesAlongRay()
       */
      class COperation {
      public:
         virtual ~COperation() {}
         virtual bool operator()(ENTITY&) = 0;
      };

   public:

      CPositionalIndex() {}
      virtual ~CPositionalIndex() {}

      virtual void Init(TConfigurationNode& t_tree) = 0;
      virtual void Reset() = 0;
      virtual void Destroy() = 0;

      /**
       * Adds an entity to this index.
       * @param c_entity The entity to add.
       */
      virtual void AddEntity(ENTITY& c_entity) = 0;

      /**
       * Removes an entity from this index.
       * @param c_entity The entity to remove.
       */
      virtual void RemoveEntity(ENTITY& c_entity) = 0;

      /**
       * Updates this positional index.
       */
      virtual void Update() = 0;

      /**
       * Puts the entities located at the given point in the passed buffer.
       * @param c_entities The entity set to use as buffer.
       * @param c_position The wanted point in the space.
       */
      virtual void GetEntitiesAt(CSet<ENTITY*,SEntityComparator>& c_entities,
                                 const CVector3& c_position) const = 0;

      /**
       * Executes an operation on all the indexed entities.
       * @param c_operation The operation to perform.
       * @see COperation
       */
      virtual void ForAllEntities(COperation& c_operation) = 0;
      
      /**
       * Executes an operation on all entities within the specified sphere range.
       * @param c_center The sphere center.
       * @param f_radius The sphere radius.
       * @param c_operation The operation to perform.
       * @see COperation
       */
      virtual void ForEntitiesInSphereRange(const CVector3& c_center,
                                            Real f_radius,
                                            COperation& c_operation) = 0;
      
      /**
       * Executes an operation on all entities within the specified box range.
       * The box is axis-aligned.
       * @param c_center The box center.
       * @param c_half_size The box half-size.
       * @param c_operation The operation to perform.
       * @see COperation
       */
      virtual void ForEntitiesInBoxRange(const CVector3& c_center,
                                         const CVector3& c_half_size,
                                         COperation& c_operation) = 0;
      
      /**
       * Executes an operation on all entities within the specified circle range.
       * The circle is parallel to the XY plane.
       * @param c_center The circle center.
       * @param f_radius The circle radius.
       * @param c_operation The operation to perform.
       * @see COperation
       */
      virtual void ForEntitiesInCircleRange(const CVector3& c_center,
                                            Real f_radius,
                                            COperation& c_operation) = 0;
      
      /**
       * Executes an operation on all entities within the specified rectangle range.
       * The rectangle is axis-aligned and parallel to the XY plane.
       * @param c_center The rectangle center.
       * @param c_half_size The rectangle half-size.
       * @param c_operation The operation to perform.
       * @see COperation
       */
      virtual void ForEntitiesInRectangleRange(const CVector3& c_center,
                                               const CVector2& c_half_size,
                                               COperation& c_operation) = 0;

      /**
       * Executes an operation on all entities that intersect the given ray.
       * When the parameter <tt>b_stop_at_closest_matches</tt> is set to <tt>true</tt>, this
       * method stops at the closest match. Depending on the actual index implementation, there
       * may be multiple closest matches. In this case, the operation is executed on all of them.
       * @param c_ray The ray.
       * @param c_operation The operation to perform.
       * @param b_stop_at_closest_matches Stop the computation as soon as the first match is found.
       * @see CRay
       * @see COperation
       */
      virtual void ForEntitiesAlongRay(const CRay3& c_ray,
                                       COperation& c_operation,
                                       bool b_stop_at_closest_match = false) = 0;

   };

}

#endif
