#ifndef PLANE_H
#define PLANE_H

namespace argos {
   class CPlane;
}

#include <argos3/utility/math/vector3.h>

namespace argos {

   class CPlane {

   public:

      CPlane() :
         m_cNormal(CVector3::Z) {}

      CPlane(const CVector3& c_position,
             const CVector3& c_normal) :
         m_cPosition(c_position),
         m_cNormal(c_normal) {}

      ~Plane() {}

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

   private:

      CVector3 m_cPosition;
      CVector3 m_cNormal;

   };

}
