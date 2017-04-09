#include "box.h"
#include "ray3.h"

namespace argos {

   /****************************************/
   /****************************************/

   bool CBox::Intersects(Real& f_t_on_ray,
                         const CRay3& c_ray) {
      /* Transform the ray so the origin is the axis-aligned box base */
      CVector3 cRayStart = c_ray.GetStart();
      CVector3 cInvRayDir;
      c_ray.GetDirection(cInvRayDir);
      cRayStart -= m_cBasePos;
      cRayStart.Rotate(m_cOrientation.Inverse());
      cInvRayDir.Rotate(m_cOrientation.Inverse());
      /* Calculate the inverse direction */
      cInvRayDir.Set(1.0 / cInvRayDir.GetX(),
                     1.0 / cInvRayDir.GetY(),
                     1.0 / cInvRayDir.GetZ());
      /* X plane */
      Real fT1 = (m_cXBounds.GetMin() - cRayStart.GetX()) * cInvRayDir.GetX();
      Real fT2 = (m_cXBounds.GetMax() - cRayStart.GetX()) * cInvRayDir.GetX();
      Real fTmin = Min(fT1, fT2);
      Real fTmax = Max(fT1, fT2);
      /* Y plane */
      fT1 = (m_cYBounds.GetMin() - cRayStart.GetY()) * cInvRayDir.GetY();
      fT2 = (m_cYBounds.GetMax() - cRayStart.GetY()) * cInvRayDir.GetY();
      fTmin = Max(fTmin, Min(fT1, fT2));
      fTmax = Min(fTmax, Max(fT1, fT2));
      if(fTmin > fTmax) return false;
      /* Z plane */
      fT1 = (m_cZBounds.GetMin() - cRayStart.GetZ()) * cInvRayDir.GetZ();
      fT2 = (m_cZBounds.GetMax() - cRayStart.GetZ()) * cInvRayDir.GetZ();
      fTmin = Max(fTmin, Min(fT1, fT2));
      fTmax = Min(fTmax, Max(fT1, fT2));
      if(fTmin > fTmax) return false;
      /* The t we search for is the smallest non-negative */
      if(fTmin >= 0) f_t_on_ray = fTmin / c_ray.GetLength();
      else if(fTmax >= 0) f_t_on_ray = fTmax / c_ray.GetLength();
      else return false;
      return true;
   }

   /****************************************/
   /****************************************/

}
