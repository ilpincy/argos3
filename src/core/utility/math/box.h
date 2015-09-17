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

namespace argos {

   class CBox {

   public:

      CBox(const CVector3& c_size,
           const CVector3& c_base_pos = CVector3(),
           const CVector3& c_axis = CVector3::Z) :
         m_cBasePos(c_base_pos),
         m_cAxis(c_axis) {
         SetSize(c_size);
      }

      inline const CVector3& GetSize() const {
         return m_cSize;
      }

      inline void SetSize(const CVector3& c_size) {
         m_cSize = c_size;
         m_cBounds[1] = 0.5f * m_cSize;
         m_cBounds[0] = -m_cBounds[1];
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

      CVector3 m_cSize;
      CVector3 m_cBounds[2];
      CVector3 m_cBasePos;
      CVector3 m_cAxis;

   };
}

#endif
