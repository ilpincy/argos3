
/**
 * @file <argos3/core/utility/math/segment.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef SEGMENT_H
#define SEGMENT_H

namespace argos {
   class CSegment;
}

#include <argos3/core/utility/math/vector2.h>

namespace argos {

   class CSegment {

   public:

      CSegment() {
      }

      CSegment(const CVector2& c_start,
               const CVector2& c_end) :
         m_cStart(c_start), m_cEnd(c_end) {
      }

      CSegment(const CVector2& c_start,
               const CVector2& c_direction,
               Real f_length) {
         Set(c_start, c_direction, f_length);
      }

      inline const CVector2& GetStart() const {
         return m_cStart;
      }

      inline const CVector2& GetEnd() const {
         return m_cEnd;
      }

      inline void SetStart(const CVector2& c_start) {
         m_cStart = c_start;
      }

      inline void SetEnd(const CVector2& c_end) {
         m_cEnd = c_end;
      }

      inline void Set(const CVector2& c_start,
                      const CVector2& c_end) {
         m_cStart = c_start;
         m_cEnd = c_end;
      }

      inline void Set(const CVector2& c_start,
                      const CVector2& c_direction,
                      Real f_length) {
         m_cStart = c_start;
         /* Same as, but faster than
            m_cEnd = m_cStart + f_length * c_direction; */
         m_cEnd = m_cStart;
         m_cEnd += f_length * c_direction;
      }

      inline void GetDirection(CVector2& c_buffer) const {
         /* Same as, but faster than
            c_buffer = (m_cEnd - m_cStart).Normalize(); */
         c_buffer = m_cEnd;
         c_buffer -= m_cStart;
         c_buffer.Normalize();
      }

      inline void GetInverseDirection(CVector2& c_buffer) const {
         /* Same as, but faster than
            c_buffer = (m_cEnd - m_cStart).Normalize(); */
         c_buffer = m_cStart;
         c_buffer -= m_cEnd;
         c_buffer.Normalize();
      }

      inline Real GetLength() const {
         return (m_cEnd - m_cStart).Length();
      }

      inline Real GetSquareLength() const {
         return (m_cEnd - m_cStart).SquareLength();
      }

      inline void ToVector(CVector2& c_buffer) const {
         /* Same as, but faster than
            c_buffer = m_cEnd - m_cStart; */
         c_buffer = m_cEnd;
         c_buffer -= m_cStart;
      }

      /* Returns the point on the line corresponding to f_t,
         which is in the range [0:1] */
      inline void GetPoint(CVector2& c_point,
                           Real f_t) const {
         ARGOS_ASSERT(f_t >= 0.0f && f_t <= 1.0f,
                      "CSegment::GetPoint(): the f_t parameter must be in the range [0:1], but f_t = " << f_t);
         c_point.SetX(m_cStart.GetX() + f_t * (m_cEnd.GetX() - m_cStart.GetX()));
         c_point.SetY(m_cStart.GetY() + f_t * (m_cEnd.GetY() - m_cStart.GetY()));
      }

      /* Returns the distance from the segment start to the point on the line corresponding to f_t,
         which is in the range [0:1] */
      inline Real GetDistance(Real f_t) const {
         ARGOS_ASSERT(f_t >= 0.0f && f_t <= 1.0f,
                      "CSegment::GetDistance(): the f_t parameter must be in the range [0:1], but f_t = " << f_t);
         return ::sqrt(Square(f_t * (m_cEnd.GetX() - m_cStart.GetX())) +
                       Square(f_t * (m_cEnd.GetY() - m_cStart.GetY())));
      }


      /**
       * This function check intersection with another segment (from http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/)
       *
       * @return true if there is an intersection or if the two segments are coincident (in this case, the starting point is returned), false otherwise
       */
      bool Intersect(const CSegment& c_segment,
                     CVector2& c_intersection);
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
      Real GetMinimumDistancePoints(const CVector2& c_point,
                                    CVector2& c_closest_point,
                                    CVector2& c_closest_segment_point);
   private:

      CVector2 m_cStart;
      CVector2 m_cEnd;

   };

}

#endif
