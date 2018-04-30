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

   };

}

#endif
