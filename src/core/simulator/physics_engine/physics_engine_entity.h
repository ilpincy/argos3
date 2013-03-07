/**
 * @file <argos3/core/simulator/physics_engine/physics_engine_entity.h>
 *
 * @brief This file provides the definition of the physics engine entity interface.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSICS_ENGINE_ENTITY_H
#define PHYSICS_ENGINE_ENTITY_H

namespace argos {
   class CPhysicsEngineEntity;
   class CPhysicsEngine;
   class CEmbodiedEntity;
   class CRay3;
   class CQuaternion;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/math/vector3.h>
#include <map>
#include <vector>
#include <string>

namespace argos {

   struct SBoundingBox {
      CVector3 MinCorner;
      CVector3 MaxCorner;

      inline bool Intersects(const SBoundingBox& s_bb) const {
         return
            (MinCorner.GetX() < s_bb.MaxCorner.GetX()) && (MaxCorner.GetX() > s_bb.MinCorner.GetX()) &&
            (MinCorner.GetY() < s_bb.MaxCorner.GetY()) && (MaxCorner.GetY() > s_bb.MinCorner.GetY()) &&
            (MinCorner.GetZ() < s_bb.MaxCorner.GetZ()) && (MaxCorner.GetZ() > s_bb.MinCorner.GetZ());
      }
   };

   class CPhysicsEngineEntity {

   public:

      typedef std::map<std::string, CPhysicsEngineEntity*> TMap;
      typedef std::vector<CPhysicsEngineEntity*> TVector;

   public:

      CPhysicsEngineEntity(CPhysicsEngine& c_engine,
                           CEmbodiedEntity& c_entity) :
         m_cEngine(c_engine),
         m_cEmbodiedEntity(c_entity),
         m_sBoundingBox() {}

      virtual ~CPhysicsEngineEntity() {}

      /**
       * Returns the physics engine associated to this physics engine entity.
       * @return The physics engine associated to this physics engine entity.
       */
      inline CPhysicsEngine& GetEngine() {
         return m_cEngine;
      }

      /**
       * Returns the embodied entity associated to this physics engine entity.
       * @return The embodied entity associated to this physics engine entity.
       */
      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return m_cEmbodiedEntity;
      }

      /**
       * Returns the embodied entity associated to this physics engine entity.
       * @return The embodied entity associated to this physics engine entity.
       */
      inline const CEmbodiedEntity& GetEmbodiedEntity() const {
         return m_cEmbodiedEntity;
      }

      /**
       * Check whether an object in this engine intersects with the given ray.
       * @param f_t_on_ray In the parametric definition of <em>ray</em> = <em>starting_point</em> + <em>t</em> * <em>direction</em>, this parameter is <em>t</em>. A value between 0 and 1 means that the intersection point lies within the given ray; values outside this range mean that the intersection point is outside the ray extrema.
       * @param c_ray The test ray.
       * @returns <tt>true</tt> if an intersection was found, <tt>false</tt> otherwise.
       */
      virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                            const CRay3& c_ray) const = 0;

      /**
       * Moves the entity to the wanted position and orientation.
       * The movement is allowed only if the object does not collide
       * with anything once in the new position.
       * @param c_position The wanted position.
       * @param c_orientation The wanted orientation.
       * @param b_check_only If <tt>false</tt>, the movement is executed; otherwise, the object is not actually moved.
       * @return <tt>true</tt> if no collisions were detected, <tt>false</tt> otherwise.
       */
      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false) = 0;

      /**
       * Returns an axis-aligned box that contains the physics engine entity.
       * The bounding box is often called AABB.
       * @returns The bounding box as a const struct.
       */
      inline const SBoundingBox& GetBoundingBox() const {
         return m_sBoundingBox;
      }

      /**
       * Returns an axis-aligned box that contains the physics engine entity.
       * The bounding box is often called AABB.
       */
      virtual void CalculateBoundingBox() = 0;

   protected:

      /**
       * Returns an axis-aligned box that contains the physics engine entity.
       * The bounding box is often called AABB.
       * @returns The bounding box as a modifiable struct.
       */
      inline SBoundingBox& GetBoundingBox() {
         return m_sBoundingBox;
      }

      friend class CEmbodiedEntity;

   protected:

      CPhysicsEngine& m_cEngine;
      CEmbodiedEntity& m_cEmbodiedEntity;
      SBoundingBox m_sBoundingBox;

   };

}

#endif
