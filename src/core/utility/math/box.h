/**
 * @file <argos3/core/utility/math/box.h>
 *
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#ifndef BOX_H
#define BOX_H

namespace argos {
   class CBox;
   class CRay3;
}

#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CBox {

   public:

      CBox(const CVector3& c_size,
           const CVector3& c_base_pos = CVector3(),
           const CQuaternion& c_orient = CQuaternion()) :
         m_cBasePos(c_base_pos),
         m_cOrientation(c_orient) {
         SetSize(c_size);
      }

      inline const CVector3& GetSize() const {
         return m_cSize;
      }

      inline void SetSize(const CVector3& c_size) {
         m_cSize = c_size;
         m_cXBounds.Set(-m_cSize.GetX() * 0.5, m_cSize.GetX() * 0.5);
         m_cYBounds.Set(-m_cSize.GetY() * 0.5, m_cSize.GetY() * 0.5);
         m_cZBounds.Set(0.0, m_cSize.GetZ());
      }

      inline const CVector3& GetBasePosition() const {
         return m_cBasePos;
      }

      inline void SetBasePosition(const CVector3& c_base_pos) {
         m_cBasePos = c_base_pos;
      }

      inline const CQuaternion& GetOrientation() const {
         return m_cOrientation;
      }

      inline void SetOrientation(const CQuaternion& c_orient) {
         m_cOrientation = c_orient;
      }

      bool Intersects(Real& f_t_on_ray,
                      const CRay3& c_ray);

   private:

      CVector3 m_cSize;
      CVector3 m_cBasePos;
      CQuaternion m_cOrientation;
      CRange<Real> m_cXBounds;
      CRange<Real> m_cYBounds;
      CRange<Real> m_cZBounds;

   };
}

#endif
