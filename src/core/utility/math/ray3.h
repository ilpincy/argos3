/**
 * @file <argos3/core/utility/math/ray3.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#ifndef RAY3_H
#define RAY3_H

namespace argos {
   class CRay3;
   class CPlane;
}

#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CRay3 {

   public:

      CRay3() {
      }

      CRay3(const CVector3& c_start,
            const CVector3& c_end) :
         m_cStart(c_start), m_cEnd(c_end) {
      }

      CRay3(const CVector3& c_start,
            const CVector3& c_direction,
            Real f_length) {
         Set(c_start, c_direction, f_length);
      }

      inline CVector3& GetStart() {
         return m_cStart;
      }

      inline const CVector3& GetStart() const {
         return m_cStart;
      }

      inline CVector3& GetEnd() {
         return m_cEnd;
      }

      inline const CVector3& GetEnd() const {
         return m_cEnd;
      }

      inline void SetStart(const CVector3& c_start) {
         m_cStart = c_start;
      }

      inline void SetEnd(const CVector3& c_end) {
         m_cEnd = c_end;
      }

      inline void SetLength(Real f_length) {
          CVector3 c_direction;
          GetDirection(c_direction);
          Set(m_cStart, c_direction, f_length);
      }

      inline void Set(const CVector3& c_start, const CVector3& c_end) {
         m_cStart = c_start;
         m_cEnd = c_end;
      }

      inline void Set(const CVector3& c_start, const CVector3& c_direction, Real f_length) {
         m_cStart = c_start;
         /* Same as, but faster than
            m_cEnd = m_cStart + f_length * c_direction; */
         m_cEnd = m_cStart;
         m_cEnd += f_length * c_direction;
      }

      inline void GetDirection(CVector3& c_buffer) const {
         /* Same as, but faster than
            c_buffer = (m_cEnd - m_cStart).Normalize(); */
         c_buffer = m_cEnd;
         c_buffer -= m_cStart;
         c_buffer.Normalize();
      }

      inline void GetInverseDirection(CVector3& c_buffer) const {
         /* Same as, but faster than
            c_buffer = (m_cEnd - m_cStart).Normalize(); */
         c_buffer = m_cStart;
         c_buffer -= m_cEnd;
         c_buffer.Normalize();
      }

      inline Real GetLength() const {
         return (m_cEnd - m_cStart).Length();
      }

      inline CVector3& ToVector(CVector3& c_buffer) const {
         /* Same as, but faster than
            c_buffer = m_cEnd - m_cStart; */
         c_buffer = m_cEnd;
         c_buffer -= m_cStart;
         return c_buffer;
      }

      /* Returns the point on the line corresponding to f_t */
      inline void GetPoint(CVector3& c_point,
                           Real f_t) const {
         c_point.SetX(m_cStart.GetX() + f_t * (m_cEnd.GetX() - m_cStart.GetX()));
         c_point.SetY(m_cStart.GetY() + f_t * (m_cEnd.GetY() - m_cStart.GetY()));
         c_point.SetZ(m_cStart.GetZ() + f_t * (m_cEnd.GetZ() - m_cStart.GetZ()));
      }

      /* Returns the distance from the ray3 start to the point on the line corresponding to f_t */
      inline Real GetDistance(Real f_t) const {
         return ::sqrt(Square(f_t * (m_cEnd.GetX() - m_cStart.GetX())) +
                       Square(f_t * (m_cEnd.GetY() - m_cStart.GetY())) +
                       Square(f_t * (m_cEnd.GetZ() - m_cStart.GetZ())));
      }

      /*
       * Calculates the ray-plane intersection point.
       * @param c_plane The plane whose intersection with this ray must be calculated
       * @param c_point The resulting intersection point is returned here
       * @return true if intersection occurred, false if ray and plane are parallel or ray lies on plane
       */
      bool Intersects(const CPlane& c_plane,
                      CVector3& c_point) const;

      /**
       * Serializes the contents of the passed ray onto a stream.
       * @param c_os The stream.
       * @param c_ray The ray.
       * @return The new state of the stream.
       */
      inline friend std::ostream& operator<<(std::ostream& c_os,
                                             const CRay3& c_ray) {
         c_os << c_ray.GetStart() << " -> " << c_ray.GetEnd();
         return c_os;
      }

   private:

      CVector3 m_cStart;
      CVector3 m_cEnd;

   };

}

#endif
