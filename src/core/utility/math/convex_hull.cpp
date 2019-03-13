/**
 * @file <argos3/core/utility/math/convex_hull.cpp>
 *
 * @author Weixu Zhu <zhuweixu_harry@126.com>
 */

#include "convex_hull.h"

#include <algorithm>

namespace argos {

   /****************************************/
   /****************************************/

   void Insert(std::vector<std::vector<CConvexHull::TEdge> >& vec_edges, 
               const std::array<UInt32, 3>& arr_vertices) {
      for(UInt32 un_idx_i = 0; un_idx_i < arr_vertices.size(); un_idx_i++) {
         for(UInt32 un_idx_j = 0; un_idx_j < arr_vertices.size(); un_idx_j++) {
            if(un_idx_i != un_idx_j) {
               CConvexHull::TEdge& tEdge = 
                  vec_edges[arr_vertices[un_idx_i]][arr_vertices[un_idx_j]];
               for(UInt32 un_idx_k = 0; un_idx_k < arr_vertices.size(); un_idx_k++) {
                  if(un_idx_k != un_idx_i && un_idx_k != un_idx_j) {
                     tEdge.push_back(arr_vertices[un_idx_k]);
                  }
               }             
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   void Erase(std::vector<std::vector<CConvexHull::TEdge> >& vec_edges, 
              const std::array<UInt32, 3>& arr_vertices) {
      for(UInt32 un_idx_i = 0; un_idx_i < arr_vertices.size(); un_idx_i++) {
         for(UInt32 un_idx_j = 0; un_idx_j < arr_vertices.size(); un_idx_j++) {
            if(un_idx_i != un_idx_j) {
               CConvexHull::TEdge& tEdge = 
                  vec_edges[arr_vertices[un_idx_i]][arr_vertices[un_idx_j]];
               for(UInt32 un_idx_k = 0; un_idx_k < arr_vertices.size(); un_idx_k++) {
                  if(un_idx_k != un_idx_i && un_idx_k != un_idx_j) {
                     std::vector<UInt32>::iterator itErase =
                        std::find(std::begin(tEdge), std::end(tEdge), arr_vertices[un_idx_k]);
                     if(itErase != std::end(tEdge)) {
                        tEdge.erase(itErase);
                     }
                  }
               }             
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   /*
    * The algorithm used to building the convex hull is based on:
    * 1. http://instructor3.algorithmdesign.net/ppt/pdf1/IncrementalHull.pdf 
    * 2. https://gist.github.com/msg555/4963794
    */
   CConvexHull::CConvexHull(const std::vector<CVector3>& vec_points) :
      m_vecPoints(vec_points) {
      /* prepare the edge table */
      std::vector<std::vector<TEdge> > m_vecEdge;
      m_vecEdge.resize(m_vecPoints.size());
      for(std::vector<TEdge>& vec_edge : m_vecEdge) {
         vec_edge.resize(m_vecPoints.size());
      }
      /* some of the points may form a plane or a line so find four points
         that make a solid 3D entity as a starting point */
      std::array<UInt32, 4> arrInitialPoints;
      bool bFoundInitialPoints = false;
      for (UInt32 unI = 0; unI < m_vecPoints.size() && !bFoundInitialPoints; unI++) {
         for (UInt32 unJ = unI + 1; unJ < m_vecPoints.size() && !bFoundInitialPoints; unJ++) {
            for (UInt32 unK = unJ + 1; unK < m_vecPoints.size() && !bFoundInitialPoints; unK++) {
               for (UInt32 unL = unK + 1; unL < m_vecPoints.size(); unL++) {
                  /* calculate normal vector with the first 3 points */
                  CVector3 cNormal =
                     (m_vecPoints[unI] - m_vecPoints[unJ]).CrossProduct(m_vecPoints[unI] - m_vecPoints[unK]);
                  /* normal is 0 means this 3 points are in a line */
                  /* compare Real cNormal.Length() with 0 (m_fZero)
                   * */
                  if (cNormal.Length() < 0.000001)
                     continue;
                  /* whether the 4th point is in the same plane */
                  if ((cNormal.DotProduct(m_vecPoints[unL] - m_vecPoints[unI]) <  0.000001) &&
                      (cNormal.DotProduct(m_vecPoints[unL] - m_vecPoints[unI]) > -0.000001) )
                     continue;
                  arrInitialPoints = {unI, unJ, unK, unL};
                  bFoundInitialPoints = true;
                  break;
               }
            }
         }
      }
      /* create 4 faces out of these 4 points*/
      for (UInt32 un_idx_i = 0; un_idx_i < 4; un_idx_i++) {
         for (UInt32 un_idx_j = un_idx_i + 1; un_idx_j < 4; un_idx_j++) {
            for (UInt32 un_idx_k = un_idx_j + 1; un_idx_k < 4; un_idx_k++) {
               m_vecFaces.emplace_back(m_vecPoints,
                                       arrInitialPoints[un_idx_i], 
                                       arrInitialPoints[un_idx_j],
                                       arrInitialPoints[un_idx_k],
                                       arrInitialPoints[6 - un_idx_i - un_idx_j - un_idx_k]);
               const SFace& sFace = m_vecFaces.back();
               Insert(m_vecEdge, sFace.VertexIndices);
            }
         }
      }
      /* create faces with the remaining points */
      for (UInt32 un_idx_point = 0; un_idx_point < m_vecPoints.size(); un_idx_point++) {
         const CVector3& cPoint = m_vecPoints[un_idx_point];

         /* check if this point was already used */
         std::array<UInt32, 4>::iterator itPoint =
            std::find(std::begin(arrInitialPoints), std::end(arrInitialPoints), un_idx_point);
         if(itPoint != std::end(arrInitialPoints)) {
            continue;
         }
         /* find all the faces with the focal point at its outside, and remove them */
         for (const SFace& s_face : m_vecFaces) {
            if (s_face.Normal.DotProduct(cPoint) > s_face.Direction + 0.000001) {
               Erase(m_vecEdge, s_face.VertexIndices);
            }
         }       
         std::vector<SFace>::iterator itEraseFrom = 
            std::remove_if(std::begin(m_vecFaces), std::end(m_vecFaces), [cPoint] (const SFace& s_face) {
               return (s_face.Normal.DotProduct(cPoint) > s_face.Direction + 0.000001);
            });
         m_vecFaces.erase(itEraseFrom, std::end(m_vecFaces));
         /* add new faces */
         std::vector<SFace> vecFacesToAdd;
         for (const SFace& s_face : m_vecFaces) {
            for (UInt32 unIdxA = 0; unIdxA < 3; unIdxA++) {
               for (UInt32 unIdxB = unIdxA + 1; unIdxB < 3; unIdxB++) {
                  if (m_vecEdge[s_face.VertexIndices[unIdxA]][s_face.VertexIndices[unIdxB]].size() == 2)
                     continue;
                  UInt32 unIdxC = 3 - unIdxA - unIdxB;
                  vecFacesToAdd.emplace_back(m_vecPoints,
                                             s_face.VertexIndices[unIdxA],
                                             s_face.VertexIndices[unIdxB],
                                             un_idx_point,
                                             s_face.VertexIndices[unIdxC]);
                  SFace& sFace = vecFacesToAdd.back();
                  /* verify direction, the direction should be consistent with the focal face (s_face)
                   * the right direction is told by the order of unIdxA and unIdxB
                   * for example, if unIdxA and unIdxB are 0 and 1, then in the new face, the
                   * order of point s_face.VertexIndices[unIdxA] and s_face.VertexIndices[unIdxB] should be reversed
                   * */
                  if ((((unIdxB - unIdxA + 3) % 3 == 1) && (sFace.VertexIndices[2] != s_face.VertexIndices[unIdxB])) ||
                      (((unIdxA - unIdxB + 3) % 3 == 1) && (sFace.VertexIndices[1] != s_face.VertexIndices[unIdxB]))) {
                     sFace.Flip();
                  }
                  Insert(m_vecEdge, sFace.VertexIndices);
               }
            }
         }
         std::move(std::begin(vecFacesToAdd), 
                   std::end(vecFacesToAdd),
                   std::back_inserter(m_vecFaces));
         vecFacesToAdd.clear();
      }
   }

   /****************************************/
   /****************************************/

   CConvexHull::SFace::SFace(const std::vector<CVector3>& vec_points,
                             UInt32 un_A,
                             UInt32 un_B,
                             UInt32 un_C,
                             UInt32 un_inside_point) {
      VertexIndices = {un_A, un_B, un_C};
      Normal = (vec_points[un_B] - vec_points[un_A]).CrossProduct(vec_points[un_C] - vec_points[un_A]);
      Direction = Normal.DotProduct(vec_points[un_A]);
      /* flip face outwards if required */
      if (Normal.DotProduct(vec_points[un_inside_point]) > Direction) {
         Flip();
      }
   }

   /****************************************/
   /****************************************/

   void CConvexHull::SFace::Flip() {
      Normal = -Normal;
      Direction = -Direction;
      std::swap(VertexIndices[1], VertexIndices[2]);
   }

   /****************************************/
   /****************************************/

}
