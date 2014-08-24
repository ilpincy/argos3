/**
 * @file <argos3/core/utility/math/plane.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#ifndef PLANE_H
#define PLANE_H

namespace argos {
   class CPlane;
   class CRay3;
}

#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CPlane {

   public:

      CPlane() :
         m_cNormal(CVector3::Z) {}

      CPlane(const CVector3& c_position,
             const CVector3& c_normal) :
         m_cPosition(c_position),
         m_cNormal(c_normal) {}

      ~CPlane() {}

      inline const CVector3& GetPosition() const {
         return m_cPosition;
      }

      inline void SetPosition(const CVector3& c_position) {
         m_cPosition = c_position;
      }

      inline const CVector3& GetNormal() const {
         return m_cNormal;
      }

      inline void SetNormal(const CVector3& c_normal) {
         m_cNormal = c_normal;
      }

      bool Intersects(Real& f_t_on_ray,
                      const CRay3& c_ray);

   private:

      CVector3 m_cPosition;
      CVector3 m_cNormal;

   };

}

#endif
