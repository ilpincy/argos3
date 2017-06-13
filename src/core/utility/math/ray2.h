/**
 * @file <argos3/core/utility/math/ray2.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#ifndef RAY2_H
#define RAY2_H

namespace argos {
   class CRay2;
}

#include <argos3/core/utility/math/vector2.h>

namespace argos {

   class CRay2 {

   public:

      CRay2() {
      }

      CRay2(const CVector2& c_start,
            const CVector2& c_end) :
         m_cStart(c_start), m_cEnd(c_end) {
      }

      CRay2(const CVector2& c_start,
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

      inline void Set(const CVector2& c_start, const CVector2& c_end) {
         m_cStart = c_start;
         m_cEnd = c_end;
      }

      inline void Set(const CVector2& c_start, const CVector2& c_direction, Real f_length) {
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

      inline void ToVector(CVector2& c_buffer) const {
         /* Same as, but faster than
            c_buffer = m_cEnd - m_cStart; */
         c_buffer = m_cEnd;
         c_buffer -= m_cStart;
      }

      /* Returns the point on the line corresponding to f_t */
      inline void GetPoint(CVector2& c_point,
                           Real f_t) const {
         c_point.SetX(m_cStart.GetX() + f_t * (m_cEnd.GetX() - m_cStart.GetX()));
         c_point.SetY(m_cStart.GetY() + f_t * (m_cEnd.GetY() - m_cStart.GetY()));
      }

      /* Returns the distance from the ray2 start to the point on the line corresponding to f_t */
      inline Real GetDistance(Real f_t) const {
         return ::sqrt(Square(f_t * (m_cEnd.GetX() - m_cStart.GetX())) +
                       Square(f_t * (m_cEnd.GetY() - m_cStart.GetY())));
      }

      /* Returns <tt>true</tt> if the passed ray intersects the current one. */
      inline bool Intersects(const CRay2& c_ray) const {
         Real fDiscriminant =
            (c_ray.m_cEnd.GetY() - c_ray.m_cStart.GetY()) *
            (m_cEnd.GetX() - m_cStart.GetX()) -
            (c_ray.m_cEnd.GetX() - c_ray.m_cStart.GetX()) *
            (m_cEnd.GetY() - m_cStart.GetY());
         if(Abs(fDiscriminant) < 1e-4) {
            /* The rays are parallel */
            return false;
         }
         /* If we get here, we know the rays are not parallel */
         /* Calculate value of T on ray 1 for the intersection point */
         Real fT1 =
            (m_cStart.GetX() - c_ray.m_cStart.GetX()) *
            (c_ray.m_cEnd.GetY() - c_ray.m_cStart.GetY()) -
            (m_cStart.GetY() - c_ray.m_cStart.GetY()) *
            (c_ray.m_cEnd.GetX() - c_ray.m_cStart.GetX());
         /* If T on ray 1 is outside the ray, no intersection */
         if(fT1 < 0.0 || fT1 > 1.0) return false;
         /* Calculate value of T on ray 2 for the intersection point */
         Real fT2;
         if(Abs(c_ray.m_cEnd.GetY() - c_ray.m_cStart.GetY()) > 1e-4) {
            /* Ray 2 is not vertical */
            fT2 =
               (m_cStart.GetY() - c_ray.m_cStart.GetY()) +
               (m_cEnd.GetY() - m_cStart.GetY()) *
               fT1 /
               (c_ray.m_cEnd.GetY() - c_ray.m_cStart.GetY());
         }
         else {
            /* Ray 2 is vertical */
            fT2 =
               (m_cStart.GetX() - c_ray.m_cStart.GetX()) +
               (m_cEnd.GetX() - m_cStart.GetX()) *
               fT1 /
               (c_ray.m_cEnd.GetX() - c_ray.m_cStart.GetX());
         }
         /* If T on ray 2 is inside the ray, intersection */
         return(fT2 >= 0.0 && fT2 <= 1.0);
      }

      /**
       * Serializes the contents of the passed ray onto a stream.
       * @param c_os The stream.
       * @param c_ray The ray.
       * @return The new state of the stream.
       */
      inline friend std::ostream& operator<<(std::ostream& c_os,
                                             const CRay2& c_ray) {
         c_os << c_ray.GetStart() << " -> " << c_ray.GetEnd();
         return c_os;
      }

   private:

      CVector2 m_cStart;
      CVector2 m_cEnd;

   };

}

#endif
