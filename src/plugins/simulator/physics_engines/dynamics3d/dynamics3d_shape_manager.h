/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_shape_manager.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Weixu Zhu- <zhuweixu_harry@126.com>
 */

#ifndef DYNAMICS3D_SHAPE_MANAGER_H
#define DYNAMICS3D_SHAPE_MANAGER_H

namespace argos {
   class CDynamics3DShapeManager;
}

#ifdef __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreorder"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
#endif

#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btBoxShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btCylinderShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btSphereShape.h>

#ifdef __APPLE__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif

#include <vector>
#include <memory>

namespace argos {

   class CDynamics3DShapeManager {
      
   public:

      static std::shared_ptr<btCollisionShape> 
         RequestBox(const btVector3& c_half_extents);

      static std::shared_ptr<btCollisionShape>
         RequestCylinder(const btVector3& c_half_extents);

      static std::shared_ptr<btCollisionShape>
         RequestSphere(btScalar f_radius);

      static std::shared_ptr<btCollisionShape>
         RequestConvexHull(const std::vector<btVector3>& vec_points);
      
   private:

      /* boxes */
      struct SBoxResource {
         SBoxResource(const btVector3& c_half_extents);
         btVector3 HalfExtents;
         std::shared_ptr<btBoxShape> Shape;
      };
      static std::vector<SBoxResource> m_vecBoxResources;

      /* cylinders */
      struct SCylinderResource {
         SCylinderResource(const btVector3& c_half_extents);
         btVector3 HalfExtents;
         std::shared_ptr<btCylinderShape> Shape;
      };
      static std::vector<SCylinderResource> m_vecCylinderResources;

      /* spheres */
      struct SSphereResource {
         SSphereResource(btScalar f_radius);
         btScalar Radius;
         std::shared_ptr<btSphereShape> Shape;
      };
      static std::vector<SSphereResource> m_vecSphereResources;

      /* convex hulls */
      struct SConvexHullResource {
         SConvexHullResource(const std::vector<btVector3>& vec_points);
         std::vector<btVector3> Points;
         std::shared_ptr<btConvexHullShape> Shape;
      };
      static std::vector<SConvexHullResource> m_vecConvexHullResources;
   };

}

#endif
