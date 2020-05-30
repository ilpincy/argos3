/**
 * @file <argos3/core/utility/math/pose2.cpp>
 *
 * @author Tyagaraja Ramaswamy - <rtyagaraja@gmail.com>
 */

#include <argos3/core/utility/math/pose2.h>

namespace argos
{

   CPose2::CPose2(const CVector3& c_position, const CQuaternion& c_orientation) :
      m_cPosition(c_position[0], c_position[1]) {
      CRadians _angle; CVector3 _axis;
      c_orientation.ToAngleAxis(_angle, _axis);
      m_cOrientation = _axis[2]*_angle;
   }


   CPose2 CPose2::Inverse() const {
      CPose2 cResult;
      cResult.m_cOrientation = -1.0*m_cOrientation;
      Real fSin, fCos;
#ifdef ARGOS_SINCOS
      SinCos(cResult.m_cOrientation, fSin, fCos);
#else
      fSin = Sin(cResult.m_cOrientation);
      fCos = Cos(cResult.m_cOrientation);
#endif
      cResult.m_cPosition.m_fX = -m_cPosition.m_fX*fCos + m_cPosition.m_fY*fSin;
      cResult.m_cPosition.m_fY = -m_cPosition.m_fX*fSin - m_cPosition.m_fY*fCos;
      return cResult;
   }


   CPose2 CPose2::operator*(const CPose2& c_pose) const {
      CPose2 cResult(*this);
      Real fSin, fCos;
#ifdef ARGOS_SINCOS
      SinCos(cResult.m_cOrientation, fSin, fCos);
#else
      fSin = Sin(cResult.m_cOrientation);
      fCos = Cos(cResult.m_cOrientation);
#endif
      cResult.m_cPosition.m_fX += c_pose.m_cPosition.m_fX*fCos - c_pose.m_cPosition.m_fY*fSin;
      cResult.m_cPosition.m_fY += c_pose.m_cPosition.m_fX*fSin + c_pose.m_cPosition.m_fY*fCos;
      cResult.m_cOrientation += c_pose.m_cOrientation;
      return cResult;
   }


   CVector2 CPose2::operator*(const CVector2& c_vec) const {
      CPose2 cResult(*this);
      Real fSin, fCos;
#ifdef ARGOS_SINCOS
      SinCos(cResult.m_cOrientation, fSin, fCos);
#else
      fSin = Sin(cResult.m_cOrientation);
      fCos = Cos(cResult.m_cOrientation);
#endif
      cResult.m_cPosition.m_fX += c_vec.m_fX*fCos - c_vec.m_fY*fSin;
      cResult.m_cPosition.m_fY += c_vec.m_fX*fSin + c_vec.m_fY*fCos;
      return cResult.m_cPosition;
   }


   CPose2& CPose2::operator*=(const CPose2& c_pose) {
      Real fSin, fCos;
#ifdef ARGOS_SINCOS
      SinCos(m_cOrientation, fSin, fCos);
#else
      fSin = Sin(cResult.m_cOrientation);
      fCos = Cos(cResult.m_cOrientation);
#endif
      m_cPosition.m_fX += c_pose.m_cPosition.m_fX*fCos - c_pose.m_cPosition.m_fY*fSin;
      m_cPosition.m_fY += c_pose.m_cPosition.m_fX*fSin + c_pose.m_cPosition.m_fY*fCos;
      m_cOrientation += c_pose.m_cOrientation;
      return *this;
   }

} // namespace argos
