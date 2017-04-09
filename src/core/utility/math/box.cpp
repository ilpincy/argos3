#include "box.h"
#include "ray3.h"

namespace argos {

   /****************************************/
   /****************************************/

   bool CBox::Intersects(Real& f_t_on_ray,
                         const CRay3& c_ray) {
      /* Transform the ray so the origin is the axis-aligned box base */
      CVector3 cRayStart = c_ray.GetStart();
      CVector3 cRayDir;
      c_ray.GetDirection(cRayDir);
      cRayStart -= m_cBasePos;
      cRayStart.Rotate(m_cOrientation.Inverse());
      cRayDir.Rotate(m_cOrientation.Inverse());
      /* Buffer for the solutions found */
      Real pfSolutions[6];
      UInt32 unSolutionCount = 0;
      CVector3 cTest;
      if(Abs(cRayDir.GetZ()) > 1e-6) {
         /* Bottom intersection (z axis) */
         pfSolutions[0] = -cRayStart.GetZ() / cRayDir.GetZ();
         if(pfSolutions[0] > 0.0) {
            cTest = cRayStart + pfSolutions[0] * cRayDir;
            if(m_cXBounds.WithinMinBoundIncludedMaxBoundIncluded(cTest.GetX()) &&
               m_cYBounds.WithinMinBoundIncludedMaxBoundIncluded(cTest.GetY()))
               ++unSolutionCount;
         }
         /* Top intersection (z axis) */
         pfSolutions[unSolutionCount] = -(cRayStart.GetZ() - m_cZBounds.GetMax()) / cRayDir.GetZ();
         if(pfSolutions[unSolutionCount] > 0.0) {
            cTest = cRayStart + pfSolutions[unSolutionCount] * cRayDir;
            if(m_cXBounds.WithinMinBoundIncludedMaxBoundIncluded(cTest.GetX()) &&
               m_cYBounds.WithinMinBoundIncludedMaxBoundIncluded(cTest.GetY()))
               ++unSolutionCount;
         }
      }
      if(Abs(cRayDir.GetX()) > 1e-6) {
         /* North intersection (x axis) */
         pfSolutions[unSolutionCount] = -(cRayStart.GetX() - m_cXBounds.GetMax()) / cRayDir.GetX();
         if(pfSolutions[unSolutionCount] > 0.0) {
            cTest = cRayStart + pfSolutions[unSolutionCount] * cRayDir;
            if(m_cZBounds.WithinMinBoundIncludedMaxBoundIncluded(cTest.GetZ()) &&
               m_cYBounds.WithinMinBoundIncludedMaxBoundIncluded(cTest.GetY()))
               ++unSolutionCount;
         }
         /* South intersection (x axis) */
         pfSolutions[unSolutionCount] = -(cRayStart.GetX() - m_cXBounds.GetMin()) / cRayDir.GetX();
         if(pfSolutions[unSolutionCount] > 0.0) {
            cTest = cRayStart + pfSolutions[unSolutionCount] * cRayDir;
            if(m_cZBounds.WithinMinBoundIncludedMaxBoundIncluded(cTest.GetZ()) &&
               m_cYBounds.WithinMinBoundIncludedMaxBoundIncluded(cTest.GetY()))
               ++unSolutionCount;
         }
      }
      if(Abs(cRayDir.GetY()) > 1e-6) {
         /* West intersection (y axis) */
         pfSolutions[unSolutionCount] = -(cRayStart.GetY() - m_cYBounds.GetMax()) / cRayDir.GetY();
         if(pfSolutions[unSolutionCount] > 0.0) {
            cTest = cRayStart + pfSolutions[unSolutionCount] * cRayDir;
            if(m_cZBounds.WithinMinBoundIncludedMaxBoundIncluded(cTest.GetZ()) &&
               m_cXBounds.WithinMinBoundIncludedMaxBoundIncluded(cTest.GetX()))
               ++unSolutionCount;
         }
         /* East intersection (y axis) */
         pfSolutions[unSolutionCount] = -(cRayStart.GetY() - m_cYBounds.GetMin()) / cRayDir.GetY();
         if(pfSolutions[unSolutionCount] > 0.0) {
            cTest = cRayStart + pfSolutions[unSolutionCount] * cRayDir;
            if(m_cZBounds.WithinMinBoundIncludedMaxBoundIncluded(cTest.GetZ()) &&
               m_cXBounds.WithinMinBoundIncludedMaxBoundIncluded(cTest.GetX()))
               ++unSolutionCount;
         }
      }
      /* All possible solutions have been found, take the closest */
      if(unSolutionCount == 0) {
         return false;
      }
      f_t_on_ray = pfSolutions[0];
      for(UInt32 i = 1; i < unSolutionCount; ++i) {
         if(pfSolutions[i] < f_t_on_ray) {
            f_t_on_ray = pfSolutions[i];
         }
      }
      f_t_on_ray /= c_ray.GetLength();
      return true;
   }

   /****************************************/
   /****************************************/

}
