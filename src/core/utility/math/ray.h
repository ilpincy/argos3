
/**
 * @file <argos3/core/utility/math/ray.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */
#ifndef CRAY_H
#define CRAY_H

namespace argos {
   class CRay;
}

#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/segment.h>

namespace argos {

   class CRay {

   public:

      CRay() {
      }

      CRay(const CVector3& c_start,
           const CVector3& c_end) :
         m_cStart(c_start), m_cEnd(c_end) {
      }

      CRay(const CVector3& c_start,
           const CVector3& c_direction,
           Real f_length) {
         Set(c_start, c_direction, f_length);
      }

      inline const CVector3& GetStart() const {
         return m_cStart;
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

      inline void ToVector(CVector3& c_buffer) const {
         /* Same as, but faster than
            c_buffer = m_cEnd - m_cStart; */
         c_buffer = m_cEnd;
         c_buffer -= m_cStart;
      }

      /* Returns the point on the line corresponding to f_t,
         which is in the range [0:1] */
      inline void GetPoint(CVector3& c_point,
                           Real f_t) const {
         ARGOS_ASSERT(f_t >= 0.0f && f_t <= 1.0f,
                      "CRay::GetPoint(): the f_t parameter must be in the range [0:1], but f_t = " << f_t);
         c_point.SetX(m_cStart.GetX() + f_t * (m_cEnd.GetX() - m_cStart.GetX()));
         c_point.SetY(m_cStart.GetY() + f_t * (m_cEnd.GetY() - m_cStart.GetY()));
         c_point.SetZ(m_cStart.GetZ() + f_t * (m_cEnd.GetZ() - m_cStart.GetZ()));
      }

      /* Returns the distance from the ray start to the point on the line corresponding to f_t,
         which is in the range [0:1] */
      inline Real GetDistance(Real f_t) const {
         ARGOS_ASSERT(f_t >= 0.0f && f_t <= 1.0f,
                      "CRay::GetDistance(): the f_t parameter must be in the range [0:1], but f_t = " << f_t);
         return ::sqrt(Square(f_t * (m_cEnd.GetX() - m_cStart.GetX())) +
                       Square(f_t * (m_cEnd.GetY() - m_cStart.GetY())) +
                       Square(f_t * (m_cEnd.GetZ() - m_cStart.GetZ())));
      }

      /* Return a segment that is the projection of the ray onto the
	 specified plane */
      inline CSegment ProjectOntoXY() const {
	return CSegment(m_cStart.ProjectOntoXY(),m_cEnd.ProjectOntoXY());
      }

      inline CSegment ProjectOntoXZ() const {
	return CSegment(m_cStart.ProjectOntoXZ(),m_cEnd.ProjectOntoXZ());
      }

      inline CSegment ProjectOntoYZ() const {
	return CSegment(m_cStart.ProjectOntoYZ(),m_cEnd.ProjectOntoYZ());
      }

   private:

      CVector3 m_cStart;
      CVector3 m_cEnd;

   };

}

#endif
