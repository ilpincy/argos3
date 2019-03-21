/**
 * @file <argos3/plugins/robots/prototype/simulator/prototype_link_entity.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Weixu Zhu - <zhuweixu_harry@126.com>
 */

#ifndef PROTOTYPE_LINK_ENTITY_H
#define PROTOTYPE_LINK_ENTITY_H

namespace argos {
   class CPrototypeLinkEntity;
}

#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/utility/math/convex_hull.h>
#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/utility/math/vector3.h>

#include <unordered_map>

namespace argos {

   class CPrototypeLinkEntity : public CEntity {

   public:

      ENABLE_VTABLE();

      using TVector = std::vector<CPrototypeLinkEntity*>;
      using TVectorIterator = std::vector<CPrototypeLinkEntity*>::iterator;
      using TVectorConstIterator = std::vector<CPrototypeLinkEntity*>::const_iterator;

   public:
      enum EGeometry {
         CYLINDER,
         BOX,
         SPHERE,
         CONVEX_HULL,
      };

   public:

      CPrototypeLinkEntity(CComposableEntity* pc_parent);

      virtual ~CPrototypeLinkEntity() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual std::string GetTypeDescription() const {
         return "link";
      }

      EGeometry GetGeometry() const {
         return m_eGeometry;
      }

      const CVector3& GetExtents() const {
         return m_cExtents;
      }

      const std::vector<CVector3>& GetConvexHullPoints() const {
         ARGOS_ASSERT(m_eGeometry == EGeometry::CONVEX_HULL,
                      "CPrototypeLinkEntity::GetConvexHullPoints(), id=\"" <<
                      GetContext() << GetId() <<
                      "\": is not a convex hull.");
         return m_vecConvexHullPoints;
      }

      const std::vector<CConvexHull::SFace>& GetConvexHullFaces() const {
         ARGOS_ASSERT(m_eGeometry == EGeometry::CONVEX_HULL,
                      "CPrototypeLinkEntity::GetConvexHullFaces(), id=\"" <<
                      GetContext() << GetId() <<
                      "\": is not a convex hull.");
         return m_vecConvexHullFaces;
      }

      Real GetMass() const {
         return m_fMass;
      }

      SAnchor& GetAnchor() {
         return *m_psAnchor;
      } 

      const SAnchor& GetAnchor() const {
         return *m_psAnchor;
      } 

   private:

      EGeometry m_eGeometry;
      CVector3 m_cExtents;
      Real m_fMass;
      SAnchor* m_psAnchor;

      std::vector<CVector3> m_vecConvexHullPoints;
      std::vector<CConvexHull::SFace> m_vecConvexHullFaces;
   };

}

#endif
