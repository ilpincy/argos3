#include "plane.h"
#include "ray3.h"

namespace argos {

   /****************************************/
   /****************************************/

   void CPlane::SetFromThreePoints(const CVector3& c_point_1,
                                   const CVector3& c_point_2,
                                   const CVector3& c_point_3) {
      m_cNormal = CVector3(c_point_3 - c_point_2);
      m_cNormal.CrossProduct(c_point_1 - c_point_2);
      m_cNormal.Normalize();
      m_cPosition = c_point_2;
   }

   /****************************************/
   /****************************************/

   bool CPlane::Intersects(Real& f_t_on_ray,
                           const CRay3& c_ray) {
      /* Ray direction */
      CVector3 cRayDir;
      c_ray.GetDirection(cRayDir);
      /* Calculate f_t_on_ray */
      Real fNumerator = (m_cPosition-c_ray.GetStart()).DotProduct(m_cNormal);
      Real fDenominator = cRayDir.DotProduct(m_cNormal);
      /* Is ray parallel to plane? */
      if(Abs(fDenominator) > 1e-6) {
         /* No, it's not */
         f_t_on_ray = fNumerator / fDenominator / c_ray.GetLength();
         return (f_t_on_ray < 1.0f);
      }
      else {
         /* Yes, it is */
         /* Is ray coincident with the plane? */
         if(Abs(fNumerator) > 1e-6) {
            /* No, the ray is parallel to and far from the plane */
            /* No intersection possible */
            return false;
         }
         else {
            /* Yes, the ray coincides with the plane */
            f_t_on_ray = 0.0f;
            return true;
         }
      }
   }

   /****************************************/
   /****************************************/

}
