#include "plane.h"
#include "ray3.h"

namespace argos {

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
