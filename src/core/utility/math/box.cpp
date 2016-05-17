#include "box.h"
#include "ray3.h"

namespace argos {

   /****************************************/
   /****************************************/

   bool CBox::Intersects(Real& f_t_on_ray,
                         const CRay3& c_ray) {
      /*
       * This algorithm was readapted from
       * http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
       */
      /* Position of the box center */
      CVector3 cBoxCenter = m_cBasePos + m_cAxis * 0.5f * m_cSize.GetZ();
      /* Express the ray data wrt the box center */
      CVector3 cRayStart = c_ray.GetStart();
      cRayStart -= cBoxCenter;
      CVector3 cRayEnd = c_ray.GetEnd();
      cRayEnd -= cBoxCenter;
      /* Direction of the ray */
      CVector3 cRayDir;
      c_ray.GetDirection(cRayDir);
      /* Vector between the ray origin and box center */
      CVector3 cRayOrig2BoxCenter = cBoxCenter - cRayStart;
      /* Scalar distance between the ray origin and point in box
       * No intersection if the ray intersects in the opposite direction */
      if( cRayOrig2BoxCenter.DotProduct(cRayDir) < 0.0f ){
         return false;
      }
      /* Calculate inverse of ray direction */
      CVector3 cRayInvDir = cRayEnd;
      cRayInvDir -= cRayStart;
      cRayInvDir.Normalize();
      cRayInvDir.SetX(1.0f / cRayInvDir.GetX());
      cRayInvDir.SetY(1.0f / cRayInvDir.GetX());
      cRayInvDir.SetZ(1.0f / cRayInvDir.GetX());
      /* Save the signs of the ray direction */
      UInt32 unRaySigns[3] = {
         cRayInvDir.GetX() < 0,
         cRayInvDir.GetY() < 0,
         cRayInvDir.GetZ() < 0,
      };
      /* Now everything is expressed as if the box were axis aligned */
      Real fTMin, fTMax, fTYMin, fTYMax, fTZMin, fTZMax;
      fTMin  = (m_cBounds[unRaySigns[0]].GetX() - cRayStart.GetX()) * cRayInvDir.GetX();
      fTMax  = (m_cBounds[1-unRaySigns[0]].GetX() - cRayStart.GetX()) * cRayInvDir.GetX();
      fTYMin = (m_cBounds[unRaySigns[1]].GetY() - cRayStart.GetY()) * cRayInvDir.GetY();
      fTYMax = (m_cBounds[1-unRaySigns[1]].GetY() - cRayStart.GetY()) * cRayInvDir.GetY();
      if((fTMin > fTYMax) || (fTYMin > fTMax)) return false;
      if(fTYMin > fTMin) fTMin = fTYMin;
      if(fTYMax < fTMax) fTMax = fTYMax;
      fTZMin = (m_cBounds[unRaySigns[2]].GetZ() - cRayStart.GetZ()) * cRayInvDir.GetZ();
      fTZMax = (m_cBounds[1-unRaySigns[2]].GetZ() - cRayStart.GetZ()) * cRayInvDir.GetZ();
      if((fTMin > fTZMax) || (fTZMin > fTMax)) return false;
      if(fTZMin > fTMin) fTMin = fTZMin;
      f_t_on_ray = fTMin;
      return true;
   }

   /****************************************/
   /****************************************/

}
