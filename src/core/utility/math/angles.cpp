/**
 * @file <argos3/core/utility/math/angles.cpp>
 * 
 * @author Carlo Pinciroli <ilpincy@gmail.com>
 */

#include "angles.h"

namespace argos {

   const CRadians CRadians::PI(ARGOS_PI);
   const CRadians CRadians::TWO_PI(2.0f * CRadians::PI);
   const CRadians CRadians::PI_OVER_TWO(CRadians::PI / 2.0f);
   const CRadians CRadians::PI_OVER_THREE(CRadians::PI / 3.0f);
   const CRadians CRadians::PI_OVER_FOUR(CRadians::PI / 4.0f);
   const CRadians CRadians::PI_OVER_SIX(CRadians::PI / 6.0f);
   const CRadians CRadians::ZERO;

   const CRange<CRadians> CRadians::SIGNED_RANGE(-CRadians::PI, CRadians::PI);
   const CRange<CRadians> CRadians::UNSIGNED_RANGE(CRadians(), CRadians::TWO_PI);
   const Real CRadians::RADIANS_TO_DEGREES(180.0/CRadians::PI.GetValue());

   const CRange<CDegrees> CDegrees::SIGNED_RANGE(CDegrees(-180.0), CDegrees(180.0));
   const CRange<CDegrees> CDegrees::UNSIGNED_RANGE(CDegrees(0.0), CDegrees(360.0));
   const Real CDegrees::DEGREES_TO_RADIANS(CRadians::PI.GetValue()/180.0);

}
