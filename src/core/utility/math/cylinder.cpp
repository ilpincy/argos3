#include "cylinder.h"
#include "ray3.h"

namespace argos {

   /****************************************/
   /****************************************/

   bool CCylinder::Intersects(Real& f_t_on_ray,
                              const CRay3& c_ray) {
      /*
       * This algorithm was adapted from
       * http://www.realtimerendering.com/resources/GraphicsGems/gemsiv/ray_cyl.c
       */
      /* Vector from cylinder base to ray start */
      CVector3 cCylBase2RayStart(c_ray.GetStart());
      cCylBase2RayStart -= m_cBasePos;
      /* Ray direction and length */
      CVector3 cRayDir;
      c_ray.GetDirection(cRayDir);
      Real fRayLen = c_ray.GetLength();
      /* Vector normal to cylinder axis and ray direction */
      CVector3 cNormal(cRayDir);
      cNormal.CrossProduct(m_cAxis);
      Real fNormalLen = cNormal.Length();
      /* Are cylinder axis and ray parallel? */
      if(fNormalLen > 0) {
         /* No, they aren't parallel */
         /* Make normal have length 1 */
         cNormal /= fNormalLen;
         /* Calculate shortest distance between axis and ray
          * by projecting cCylBase2RayStart onto cNormal */
         Real fDist = Abs(cCylBase2RayStart.DotProduct(cNormal));
         /* Is fDist smaller than the cylinder radius? */
         if(fDist > m_fRadius) {
            /* No, it's not, so there can't be any intersection */
            return false;
         }
         /* If we get here, it's because the ray intersects the infinite cylinder */
         /* Create a buffer for the 4 potential intersection points
            (two on the sides, two on the bases) */
         Real fPotentialT[4];
         /* First, calculate the intersection points with the sides */
         /* Calculate the midpoint between the two intersection points */
         CVector3 cVec(cCylBase2RayStart);
         cVec.CrossProduct(m_cAxis);
         Real fMidPointDist = -cVec.DotProduct(cNormal) / fNormalLen;
         /* Calculate the distance between the midpoint and the potential t's */
         cVec = cNormal;
         cVec.CrossProduct(m_cAxis);
         cVec.Normalize();
         Real fDeltaToMidPoint = Abs(Sqrt(Square(m_fRadius) - Square(fDist)) / cRayDir.DotProduct(cVec));
         /* Calculate the potential t's on the infinite surface */
         fPotentialT[0] = (fMidPointDist - fDeltaToMidPoint) / fRayLen;
         fPotentialT[1] = (fMidPointDist + fDeltaToMidPoint) / fRayLen;
         /* Make sure these t's correspond to points within the cylinder bases */
         CVector3 cPoint;
         c_ray.GetPoint(cPoint, fPotentialT[0]);
         if((cPoint - m_cBasePos).DotProduct(m_cAxis) < 0 ||
            (cPoint - (m_cBasePos + m_fHeight * m_cAxis)).DotProduct(m_cAxis) > 0) {
            fPotentialT[0] = -1;
         }
         c_ray.GetPoint(cPoint, fPotentialT[1]);
         if((cPoint - m_cBasePos).DotProduct(m_cAxis) < 0 ||
            (cPoint - (m_cBasePos + m_fHeight * m_cAxis)).DotProduct(m_cAxis) > 0) {
            fPotentialT[1] = -1;
         }
         /* Check whether the ray is contained within the cylinder bases */
         Real fDenominator = cRayDir.DotProduct(m_cAxis);
         /* Is ray parallel to plane? */
         if(Abs(fDenominator) > 1e-6) {
            /* No, it's not parallel */
            fDenominator *= fRayLen;
            /* Bottom base */
            fPotentialT[2] =
               (m_cBasePos - c_ray.GetStart()).DotProduct(m_cAxis) / fDenominator;
            /* Top base */
            fPotentialT[3] =
               (m_cBasePos + m_fHeight * m_cAxis - c_ray.GetStart()).DotProduct(m_cAxis) / fDenominator;
            /* Make sure these t's are within the cylinder surface */
            c_ray.GetPoint(cPoint, fPotentialT[2]);
            CVector3 cDiff = cPoint - m_cBasePos;
            if((cDiff - cDiff.DotProduct(m_cAxis) * m_cAxis).SquareLength() > Square(m_fRadius))
               fPotentialT[2] = -1;
            c_ray.GetPoint(cPoint, fPotentialT[3]);
            cDiff = cPoint - m_cBasePos;
            if((cDiff - cDiff.DotProduct(m_cAxis) * m_cAxis).SquareLength() > Square(m_fRadius))
               fPotentialT[3] = -1;
         }
         else {
            /* Yes, it's parallel - discard the intersections */
            fPotentialT[2] = -1.0;
            fPotentialT[3] = -1.0;
         }
         /* Go through all the potential t's and get the best */
         f_t_on_ray = 2.0;
         for(UInt32 i = 0; i < 4; ++i) {
            if(fPotentialT[i] > 0.0f) {
               f_t_on_ray = Min(f_t_on_ray, fPotentialT[i]);
            }
         }
         /* Return true only if the intersection point is within the ray limits */
         return (f_t_on_ray < 1.0f);
      }
      else {
         /* Yes, ray and axis are parallel */
         /* Projection of cCylBase2RayStart onto the axis */
         Real fProj = cCylBase2RayStart.DotProduct(m_cAxis);
         /* Radial vector */
         CVector3 cRadial(cCylBase2RayStart);
         cRadial -= fProj * m_cAxis;
         Real fDist = cRadial.Length();
         /* Is ray within the cylinder radius? */
         if(fDist > m_fRadius) {
            /* No, it's not */
            return false;
         }
         /* If we get here, it's because the ray might intersect the cylinder bases */
         Real fDenominator = cRayDir.DotProduct(m_cAxis) * fRayLen;
         /* Create a buffer for the 2 potential intersection points */
         Real fPotentialT[2];
         /* Bottom base */
         fPotentialT[0] =
            (m_cBasePos-c_ray.GetStart()).DotProduct(m_cAxis) / fDenominator;
         /* Top base */
         fPotentialT[1] =
            (m_cBasePos + m_fHeight * m_cAxis - c_ray.GetStart()).DotProduct(m_cAxis) / fDenominator;
         /* Make sure these t's are within the cylinder surface */
         CVector3 cPoint;
         c_ray.GetPoint(cPoint, fPotentialT[0]);
         CVector3 cDiff = cPoint - m_cBasePos;
         if((cDiff - cDiff.DotProduct(m_cAxis) * m_cAxis).SquareLength() > Square(m_fRadius))
            fPotentialT[0] = -1;
         c_ray.GetPoint(cPoint, fPotentialT[1]);
         cDiff = cPoint - m_cBasePos;
         if((cDiff - cDiff.DotProduct(m_cAxis) * m_cAxis).SquareLength() > Square(m_fRadius))
            fPotentialT[1] = -1;
         /* Go through all the potential t's and get the best */
         f_t_on_ray = 2.0;
         for(UInt32 i = 0; i < 2; ++i) {
            if(fPotentialT[i] > 0.0f) {
               f_t_on_ray = Min(f_t_on_ray, fPotentialT[i]);
            }
         }
         /* Return true only if the intersection point is within the ray limits */
         return (f_t_on_ray < 1.0f);
      }
   }

   /****************************************/
   /****************************************/

}
