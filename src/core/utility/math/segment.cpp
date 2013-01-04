
/**
 * @file <argos3/core/utility/math/segment.cpp>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#include "segment.h"

namespace argos {

   /****************************************/
   /****************************************/

   bool CSegment::Intersect(const CSegment& c_segment,
                         CVector2& c_intersection) {
      Real fDenom = (((c_segment.GetEnd().GetY() - c_segment.GetStart().GetY())*(m_cEnd.GetX() - m_cStart.GetX())) -
                     ((c_segment.GetEnd().GetX() - c_segment.GetStart().GetX())*(m_cEnd.GetY() - m_cStart.GetY())));
      Real fNumA = (((c_segment.GetEnd().GetX() - c_segment.GetStart().GetX())*(m_cStart.GetY() - c_segment.GetStart().GetY())) -
                    ((c_segment.GetEnd().GetY() - c_segment.GetStart().GetY())*(m_cStart.GetX() - c_segment.GetStart().GetX())));
      Real fNumB = (((m_cEnd.GetX() - m_cStart.GetX())*(m_cStart.GetY() - c_segment.GetStart().GetY())) -
                    ((m_cEnd.GetY() - m_cStart.GetY())*(m_cStart.GetX() - c_segment.GetStart().GetX())));
      if( fDenom == 0.0f ) {
         // the segments are coincident
         if(fNumA == 0.0f && fNumB == 0.0f) {
            c_intersection = m_cStart;
            return true;
         }
         // the segments are parallel
         return false;
      }
      // Get the intersection point, it it exists
      Real fUA = fNumA / fDenom;
      Real fUB = fNumB / fDenom;
      if( fUA >= 0.0f && fUA <= 1.0f && fUB >= 0.0f && fUB <= 1.0f ) {
         c_intersection.SetX(m_cStart.GetX() + fUA*(m_cEnd.GetX() - m_cStart.GetX()));
         c_intersection.SetY(m_cStart.GetY() + fUA*(m_cEnd.GetY() - m_cStart.GetY()));
         return true;
      }
      return false;
   }


   /**
    * This function computes the minimum distance points of a
    * given point from the line identified by a segment, i.e.,
    * the projection point to the line and the closest point on
    * the segment
    * (http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/)
    *
    * @return a real value indicating wether the closest point
    * lays within the segment, or oustise. A value < 0 indicates
    * that the intersection is before the start point, a value
    * between 0 and 1 indicates that the intersection is within
    * the segment, and a value > 1 indicates it is beyond the end
    * point.
    */
   Real CSegment::GetMinimumDistancePoints(const CVector2& c_point,
                                 CVector2& c_closest_point,
                                 CVector2& c_closest_segment_point) {
      Real fSegLenSquare = GetSquareLength();
      Real fSegLenFrac = ( (c_point.GetX() - m_cStart.GetX())*(m_cEnd.GetX() - m_cStart.GetX()) +
                           (c_point.GetY() - m_cStart.GetY())*(m_cEnd.GetY() - m_cStart.GetY()) ) / fSegLenSquare;
      c_closest_point.SetX(m_cStart.GetX() + fSegLenFrac*(m_cEnd.GetX()-m_cStart.GetX()));
      c_closest_point.SetY(m_cStart.GetY() + fSegLenFrac*(m_cEnd.GetY()-m_cStart.GetY()));
      if( fSegLenFrac < 0 ) {
         c_closest_segment_point = m_cStart;
      }
      else if( fSegLenFrac > 1 ) {
         c_closest_segment_point = m_cEnd;
      }
      else {
         c_closest_segment_point = c_closest_point;
      }
      return fSegLenFrac;
   }


   /****************************************/
   /****************************************/

}
