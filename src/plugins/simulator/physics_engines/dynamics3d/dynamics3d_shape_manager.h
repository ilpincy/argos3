/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_shape_manager.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_SHAPE_MANAGER_H
#define DYNAMICS3D_SHAPE_MANAGER_H

namespace argos {
   class CDynamics3DShapeManager;
}

#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btBoxShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btCylinderShape.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/bullet/BulletCollision/CollisionShapes/btSphereShape.h>
#include <vector>

namespace argos {

   class CDynamics3DShapeManager {
      
   public:
      
      static btBoxShape* RequestBox(const btVector3& c_half_extents);

      static void ReleaseBox(const btBoxShape* pc_release);

      static btCylinderShape* RequestCylinder(const btVector3& c_half_extents);

      static void ReleaseCylinder(const btCylinderShape* pc_release);

      static btSphereShape* RequestSphere(btScalar f_radius);

      static void ReleaseSphere(const btSphereShape* pc_release);
      
   private:

      /* boxes */
      struct SBoxResource {
         SBoxResource(const btVector3& c_half_extents);
         btVector3 HalfExtents;
         btBoxShape* Shape;
         size_t InUseCount;
      };
      static std::vector<SBoxResource> m_vecBoxResources;

      /* cylinders */
      struct SCylinderResource {
         SCylinderResource(const btVector3& c_half_extents);
         btVector3 HalfExtents;
         btCylinderShape* Shape;
         size_t InUseCount;
      };
      static std::vector<SCylinderResource> m_vecCylinderResources;

      /* spheres */
      struct SSphereResource {
         SSphereResource(btScalar f_radius);
         btScalar Radius;
         btSphereShape* Shape;
         size_t InUseCount;
      };
      static std::vector<SSphereResource> m_vecSphereResources;

   };

}

#endif
