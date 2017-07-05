#include "cylinder.h"
#include "ray3.h"

namespace argos {

   /****************************************/
   /****************************************/

   bool CCylinder::Intersects(Real& f_t_on_ray,
                              const CRay3& c_ray) {
      /* Solution candidates */
      UInt32 unSolutionCount = 0;
      Real pfSolutions[4];
      /* Position of top cap relative to bottom one */
      CVector3 cRelTopCap = m_fHeight * m_cAxis;
      /*
       * Check intersection with cylinder
       */
      CVector3 cPRA = c_ray.GetStart() - m_cBasePos;
      CVector3 cBeta = cPRA - m_cAxis.DotProduct(cPRA) * m_cAxis;
      CVector3 cRayDir;
      c_ray.GetDirection(cRayDir);
      Real fDRA = m_cAxis.DotProduct(cRayDir);
      CVector3 cAlpha = cRayDir - fDRA * m_cAxis;
      Real fA = cAlpha.SquareLength();
      Real fB = 2.0 * cAlpha.DotProduct(cBeta);
      Real fC = cBeta.SquareLength() - Square(m_fRadius);
      Real fDelta = Square(fB) - 4.0 * fA * fC;
      if(fDelta == 0) {
         /* One candidate solution, is it within the cylinder caps? */
         pfSolutions[0] = -fB / (2.0 * fA);
         if(pfSolutions[0] > 0.0) {
            CVector3 cTest = cPRA + pfSolutions[0] * cRayDir;
            if(m_cAxis.DotProduct(cTest) > 0) {
               cTest -= cRelTopCap;
               if(m_cAxis.DotProduct(cTest) < 0) {
                  ++unSolutionCount;
               }
            }
         }
      }
      else if(fDelta > 0) {
         /* Two candidate solutions, are they within the cylinder caps? */
         CVector3 cTest;
         /* Test first solution */
         pfSolutions[0] = (-fB + Sqrt(fDelta)) / (2.0 * fA);
         if(pfSolutions[0] > 0.0) {
            cTest = cPRA + pfSolutions[0] * cRayDir;
            if(m_cAxis.DotProduct(cTest) > 0) {
               cTest -= cRelTopCap;
               if(m_cAxis.DotProduct(cTest) < 0) {
                  ++unSolutionCount;
               }
            }
         }
         /* Test second solution */
         pfSolutions[unSolutionCount] = (-fB - Sqrt(fDelta)) / (2.0 * fA);
         if(pfSolutions[unSolutionCount] > 0.0) {
            cTest = cPRA + pfSolutions[unSolutionCount] * cRayDir;
            if(m_cAxis.DotProduct(cTest) > 0) {
               cTest -= cRelTopCap;
               if(m_cAxis.DotProduct(cTest) < 0) {
                  ++unSolutionCount;
               }
            }
         }
      }
      /*
       * Check intersection with bottom and top caps
       */
      /* If the directions of the cylinder axis and the ray are parallel,
       * nothing to do */
      if(fDRA > 10e-6) {
         Real fPPRA = m_cAxis.DotProduct(cPRA);
         /* Bottom cap */
         pfSolutions[unSolutionCount] = -fPPRA / fDRA;
         if(pfSolutions[unSolutionCount] > 0.0 &&
            (cPRA + pfSolutions[unSolutionCount] * cRayDir).SquareLength() < Square(m_fRadius)) {
            ++unSolutionCount;
         }
         /* Top cap */
         pfSolutions[unSolutionCount] = -(fPPRA - m_fHeight) / fDRA;
         if(pfSolutions[unSolutionCount] > 0.0 &&
            (cPRA - cRelTopCap + pfSolutions[unSolutionCount] * cRayDir).SquareLength() < Square(m_fRadius)) {
            ++unSolutionCount;
         }
      }
      /*
       * All possible solutions have been found, take the closest
       */
      if(unSolutionCount == 0) {
         return false;
      }
      f_t_on_ray = pfSolutions[0];
      for(UInt32 i = 1; i < unSolutionCount; ++i) {
         if(pfSolutions[i] < f_t_on_ray)
            f_t_on_ray = pfSolutions[i];
      }
      f_t_on_ray /= c_ray.GetLength();
      return true;
   }

   /****************************************/
   /****************************************/

}
