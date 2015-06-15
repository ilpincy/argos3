/**
 * @file <argos3/core/utility/math/cylinder.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#ifndef CYLINDER_H
#define CYLINDER_H

namespace argos {
   class CCylinder;
   class CRay3;
}

#include <argos3/core/utility/math/vector3.h>

namespace argos {

   class CCylinder {

   public:

      CCylinder(Real f_radius,
                Real f_height,
                const CVector3& c_base_pos = CVector3(),
                const CVector3& c_axis = CVector3::Z) :
         m_fRadius(f_radius),
         m_fHeight(f_height),
         m_cBasePos(c_base_pos),
         m_cAxis(c_axis) {
      }

      inline Real GetRadius() const {
         return m_fRadius;
      }

      inline void SetRadius(Real f_radius) {
         m_fRadius = f_radius;
      }

      inline Real GetHeight() const {
         return m_fHeight;
      }

      inline void SetHeight(Real f_height) {
         m_fHeight = f_height;
      }

      inline const CVector3& GetBasePosition() const {
         return m_cBasePos;
      }

      inline void SetBasePosition(const CVector3& c_base_pos) {
         m_cBasePos = c_base_pos;
      }

      inline const CVector3& GetAxis() const {
         return m_cBasePos;
      }

      inline void SetAxis(const CVector3& c_axis) {
         m_cAxis = c_axis;
      }

      bool Intersects(Real& f_t_on_ray,
                      const CRay3& c_ray);

   private:

      Real m_fRadius;
      Real m_fHeight;
      CVector3 m_cBasePos;
      CVector3 m_cAxis;

   };
}

#endif
