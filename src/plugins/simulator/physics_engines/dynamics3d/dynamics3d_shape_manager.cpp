/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_shape_manager.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Weixu Zhu- <zhuweixu_harry@126.com>
 */

#include "dynamics3d_shape_manager.h"
#include <argos3/core/utility/configuration/argos_exception.h>

namespace argos {

   /****************************************/
   /****************************************/

   std::vector<CDynamics3DShapeManager::SBoxResource>
      CDynamics3DShapeManager::m_vecBoxResources;

   /****************************************/
   /****************************************/

   std::shared_ptr<btCollisionShape> CDynamics3DShapeManager::RequestBox(const btVector3& c_half_extents) {
      std::vector<SBoxResource>::iterator itBoxResource;
      for(itBoxResource = std::begin(m_vecBoxResources);
          itBoxResource != std::end(m_vecBoxResources);
          ++itBoxResource) {
         if(itBoxResource->HalfExtents == c_half_extents) break;
      }
      /* If the resource doesn't exist, create a new one */
      if(itBoxResource == std::end(m_vecBoxResources)) {
         itBoxResource =
            m_vecBoxResources.emplace(itBoxResource, c_half_extents);
      }
      return std::static_pointer_cast<btCollisionShape>(itBoxResource->Shape);
   }

   /****************************************/
   /****************************************/

   CDynamics3DShapeManager::SBoxResource::SBoxResource(const btVector3& c_half_extents) :
      HalfExtents(c_half_extents),
      Shape(new btBoxShape(c_half_extents)) {}

   /****************************************/
   /****************************************/

   std::vector<CDynamics3DShapeManager::SCylinderResource>
      CDynamics3DShapeManager::m_vecCylinderResources;

   /****************************************/
   /****************************************/

   std::shared_ptr<btCollisionShape> CDynamics3DShapeManager::RequestCylinder(const btVector3& c_half_extents) {
      std::vector<SCylinderResource>::iterator itCylinderResource;
      for(itCylinderResource = std::begin(m_vecCylinderResources);
          itCylinderResource != std::end(m_vecCylinderResources);
          ++itCylinderResource) {
         if(itCylinderResource->HalfExtents == c_half_extents) break;
      }
      /* If the resource doesn't exist, create a new one */
      if(itCylinderResource == std::end(m_vecCylinderResources)) {
         itCylinderResource =
            m_vecCylinderResources.emplace(itCylinderResource, c_half_extents);
      }
      return std::static_pointer_cast<btCollisionShape>(itCylinderResource->Shape);
   }

   /****************************************/
   /****************************************/

   CDynamics3DShapeManager::SCylinderResource::SCylinderResource(const btVector3& c_half_extents) :
      HalfExtents(c_half_extents),
      Shape(new btCylinderShape(c_half_extents)) {}

   /****************************************/
   /****************************************/

   std::vector<CDynamics3DShapeManager::SSphereResource>
      CDynamics3DShapeManager::m_vecSphereResources;

   /****************************************/
   /****************************************/

   std::shared_ptr<btCollisionShape> CDynamics3DShapeManager::RequestSphere(btScalar f_radius) {
      std::vector<SSphereResource>::iterator itSphereResource;
      for(itSphereResource = std::begin(m_vecSphereResources);
          itSphereResource != std::end(m_vecSphereResources);
          ++itSphereResource) {
         if(itSphereResource->Radius == f_radius) break;
      }
      /* If the resource doesn't exist, create a new one */
      if(itSphereResource == std::end(m_vecSphereResources)) {
         itSphereResource =
            m_vecSphereResources.emplace(itSphereResource, f_radius);
      }
      return std::static_pointer_cast<btCollisionShape>(itSphereResource->Shape);
   }

   /****************************************/
   /****************************************/

   CDynamics3DShapeManager::SSphereResource::SSphereResource(btScalar f_radius) :
      Radius(f_radius),
      Shape(new btSphereShape(f_radius)) {}

   /****************************************/
   /****************************************/

   std::vector<CDynamics3DShapeManager::SConvexHullResource>
      CDynamics3DShapeManager::m_vecConvexHullResources;

   /****************************************/
   /****************************************/

   std::shared_ptr<btCollisionShape> CDynamics3DShapeManager::RequestConvexHull(const std::vector<btVector3>& vec_points) {
      std::vector<SConvexHullResource>::iterator itConvexHullResource;
      for(itConvexHullResource = std::begin(m_vecConvexHullResources);
          itConvexHullResource != std::end(m_vecConvexHullResources);
          ++itConvexHullResource) {
         if(itConvexHullResource->Points == vec_points) break;
      }
      /* If the resource doesn't exist, create a new one */
      if(itConvexHullResource == std::end(m_vecConvexHullResources)) {
         itConvexHullResource =
            m_vecConvexHullResources.emplace(itConvexHullResource, vec_points);
      }
      return std::static_pointer_cast<btCollisionShape>(itConvexHullResource->Shape);
   }

   /****************************************/
   /****************************************/

   CDynamics3DShapeManager::SConvexHullResource::SConvexHullResource(const std::vector<btVector3>& vec_points) :
      Points(vec_points),
      Shape(new btConvexHullShape) {
      Shape->setMargin(0);
      for(const btVector3& c_point : vec_points) {
         Shape->addPoint(c_point);
      }
   }

   /****************************************/
   /****************************************/
}
