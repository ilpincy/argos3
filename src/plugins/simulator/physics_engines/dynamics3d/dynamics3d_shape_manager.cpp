/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_shape_manager.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_shape_manager.h"
#include <argos3/core/utility/configuration/argos_exception.h>

namespace argos {

   /****************************************/
   /****************************************/

   std::vector<CDynamics3DShapeManager::SBoxResource> CDynamics3DShapeManager::m_vecBoxResources;

   /****************************************/
   /****************************************/
   
   btBoxShape* CDynamics3DShapeManager::RequestBox(const btVector3& c_half_extents) {
      std::vector<SBoxResource>::iterator itBoxResource;      
      for(itBoxResource = std::begin(m_vecBoxResources);
          itBoxResource != std::end(m_vecBoxResources);
          ++itBoxResource) {
         if(itBoxResource->HalfExtents == c_half_extents) break;
      }      
      /* If the resource doesn't exist, create a new one */
      if(itBoxResource == std::end(m_vecBoxResources)) {
         itBoxResource = m_vecBoxResources.emplace(itBoxResource, c_half_extents);
      }
      itBoxResource->InUseCount++;
      return (itBoxResource->Shape);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DShapeManager::ReleaseBox(const btBoxShape* pc_release) {
      std::vector<SBoxResource>::iterator itBoxResource;      
      for(itBoxResource = std::begin(m_vecBoxResources);
          itBoxResource != std::end(m_vecBoxResources);
          ++itBoxResource) {
         if(itBoxResource->Shape == pc_release) break;
      }
      /* If the resource doesn't exist, throw an exception */
      if(itBoxResource == std::end(m_vecBoxResources)) {
         THROW_ARGOSEXCEPTION("Box resource not found");
      }
      itBoxResource->InUseCount--;
      if(itBoxResource->InUseCount == 0) {
         m_vecBoxResources.erase(itBoxResource);
      }
   }

   /****************************************/
   /****************************************/

   CDynamics3DShapeManager::SBoxResource::SBoxResource(const btVector3& c_half_extents) : 
      HalfExtents(c_half_extents),
      Shape(new btBoxShape(c_half_extents)),
      InUseCount(0) {}

   /****************************************/
   /****************************************/

      std::vector<CDynamics3DShapeManager::SCylinderResource> CDynamics3DShapeManager::m_vecCylinderResources;

   /****************************************/
   /****************************************/
   
   btCylinderShape* CDynamics3DShapeManager::RequestCylinder(const btVector3& c_half_extents) {
      std::vector<SCylinderResource>::iterator itCylinderResource;      
      for(itCylinderResource = std::begin(m_vecCylinderResources);
          itCylinderResource != std::end(m_vecCylinderResources);
          ++itCylinderResource) {
         if(itCylinderResource->HalfExtents == c_half_extents) break;
      }      
      /* If the resource doesn't exist, create a new one */
      if(itCylinderResource == std::end(m_vecCylinderResources)) {
         itCylinderResource = m_vecCylinderResources.emplace(itCylinderResource, c_half_extents);
      }
      itCylinderResource->InUseCount++;
      return (itCylinderResource->Shape);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DShapeManager::ReleaseCylinder(const btCylinderShape* pc_release) {
      std::vector<SCylinderResource>::iterator itCylinderResource;      
      for(itCylinderResource = std::begin(m_vecCylinderResources);
          itCylinderResource != std::end(m_vecCylinderResources);
          ++itCylinderResource) {
         if(itCylinderResource->Shape == pc_release) break;
      }
      /* If the resource doesn't exist, throw an exception */
      if(itCylinderResource == std::end(m_vecCylinderResources)) {
         THROW_ARGOSEXCEPTION("Cylinder resource not found");
      }
      itCylinderResource->InUseCount--;
      if(itCylinderResource->InUseCount == 0) {
         m_vecCylinderResources.erase(itCylinderResource);
      }
   }

   /****************************************/
   /****************************************/

   CDynamics3DShapeManager::SCylinderResource::SCylinderResource(const btVector3& c_half_extents) : 
      HalfExtents(c_half_extents),
      Shape(new btCylinderShape(c_half_extents)),
      InUseCount(0) {}

   /****************************************/
   /****************************************/

   std::vector<CDynamics3DShapeManager::SSphereResource> CDynamics3DShapeManager::m_vecSphereResources;

   /****************************************/
   /****************************************/
   
   btSphereShape* CDynamics3DShapeManager::RequestSphere(btScalar f_radius) {
      std::vector<SSphereResource>::iterator itSphereResource;      
      for(itSphereResource = std::begin(m_vecSphereResources);
          itSphereResource != std::end(m_vecSphereResources);
          ++itSphereResource) {
         if(itSphereResource->Radius == f_radius) break;
      }      
      /* If the resource doesn't exist, create a new one */
      if(itSphereResource == std::end(m_vecSphereResources)) {
         itSphereResource = m_vecSphereResources.emplace(itSphereResource, f_radius);
      }
      itSphereResource->InUseCount++;
      return (itSphereResource->Shape);
   }

   /****************************************/
   /****************************************/

   void CDynamics3DShapeManager::ReleaseSphere(const btSphereShape* pc_release) {
      std::vector<SSphereResource>::iterator itSphereResource;      
      for(itSphereResource = std::begin(m_vecSphereResources);
          itSphereResource != std::end(m_vecSphereResources);
          ++itSphereResource) {
         if(itSphereResource->Shape == pc_release) break;
      }
      /* If the resource doesn't exist, throw an exception */
      if(itSphereResource == std::end(m_vecSphereResources)) {
         THROW_ARGOSEXCEPTION("Sphere resource not found");
      }
      itSphereResource->InUseCount--;
      if(itSphereResource->InUseCount == 0) {
         m_vecSphereResources.erase(itSphereResource);
      }
   }

   /****************************************/
   /****************************************/

   CDynamics3DShapeManager::SSphereResource::SSphereResource(btScalar f_radius) : 
      Radius(f_radius),
      Shape(new btSphereShape(f_radius)),
      InUseCount(0) {}

   /****************************************/
   /****************************************/
}

