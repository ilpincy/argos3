/**
 * @file <argos3/core/utility/math/convex_hull.h>
 *
 * @author Weixu Zhu <zhuweixu_harry@126.com>
 */

#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

namespace argos {
   class CConvexHull;
}

#include <argos3/core/utility/math/vector3.h>
#include <array>
#include <algorithm> // REMOVE

namespace argos {

   class CConvexHull {

   public:
      typedef std::vector<UInt32> TEdge;

      /* each face has three points, specified in counter-clockwise 
         order looking from the outside */
      struct SFace {
         SFace(const std::vector<CVector3>& vec_points,
               UInt32 un_A, 
               UInt32 un_B, 
               UInt32 un_C, 
               UInt32 un_inside_point);

         void Flip();

         CVector3 Normal;
         Real Direction;
         std::array<UInt32, 3> VertexIndices;
      };

   public:
      CConvexHull(const std::vector<CVector3>& vec_points);

      const std::vector<SFace>& GetFaces() const {
         return m_vecFaces;
      }

   private:
      std::vector<SFace> m_vecFaces;
      std::vector<CVector3> m_vecPoints;
   };
}

#endif
