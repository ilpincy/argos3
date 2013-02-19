
/**
 * @file <argos3/core/utility/math/ray3.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "ray3.h"
#include "plane.h"

namespace argos {

   /****************************************/
   /****************************************/

   bool CRay3::Intersects(const CPlane& c_plane,
                          CVector3& c_point) const {
      CVector3 cRayVec;
      Real fDen = c_plane.GetNormal().DotProduct(ToVector(cRayVec));
      if(Abs(fDen) > 0.0f) {
         /* There could be intersection */
         Real fTOnRay = -c_plane.GetNormal().DotProduct(m_cStart - c_plane.GetPosition()) / fDen;
         if(fTOnRay < 0.0f || fTOnRay > 1.0f) {
            /* Intersection point is beyond ray extrema */
            return false;
         }
         else {
            /* There is an intersection */
            GetPoint(c_point, fTOnRay);
            return true;
         }
      }
      else {
         /* No intersection */
         return false;
      }
   }
   
   /****************************************/
   /****************************************/

}
