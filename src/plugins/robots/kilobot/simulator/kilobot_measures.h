/**
 * @file <argos3/plugins/robots/kilobot/simulator/kilobot_measures.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Vito Trianni - <vito.trianni@istc.cnr.it>
 */

#ifndef KILOBOT_MEASURES_H
#define KILOBOT_MEASURES_H

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/math/vector3.h>

namespace argos {

   /* Kilobot measures */
   static const Real KILOBOT_BODY_HEIGHT            = 0.0127;
   static const Real KILOBOT_PIN_HEIGHT             = 0.0217;
   static const Real KILOBOT_PIN_RADIUS             = 0.0007;
   static const Real KILOBOT_INTERPIN_DISTANCE      = 0.025;
   static const Real KILOBOT_HALF_INTERPIN_DISTANCE = KILOBOT_INTERPIN_DISTANCE * 0.5;
   static const Real KILOBOT_FRONT_PIN_DISTANCE     = 0.025;
   static const Real KILOBOT_RADIUS                 = 0.0165;
   static const Real KILOBOT_ECCENTRICITY           = 0.0092;
   static const Real KILOBOT_HEIGHT                 = KILOBOT_PIN_HEIGHT + KILOBOT_BODY_HEIGHT;
   static const Real KILOBOT_MASS                   = 0.01;

   /* RGB LED */
   static const Real KILOBOT_PIN_WHEEL_RADIUS       = 0.001;
   static const CRadians KILOBOT_LED_ANGLE          = CRadians(ARGOS_PI / 6.0);
   static const Real KILOBOT_LED_ELEVATION          = KILOBOT_HEIGHT;
   static const Real KILOBOT_LED_RADIUS             = KILOBOT_RADIUS * 0.25;
   static const Real KILOBOT_LED_HEIGHT             = KILOBOT_RADIUS * 0.05;

   /* Light Sensor */
   static const CRadians KILOBOT_LIGHT_SENSOR_ANGLE  = ToRadians(CDegrees(100.0));
   static const Real KILOBOT_LIGHT_SENSOR_ELEVATION  = KILOBOT_HEIGHT;
   static const Real KILOBOT_LIGHT_SENSOR_RADIUS     = KILOBOT_RADIUS - 0.002;
   static const Real KILOBOT_LIGHT_SENSOR_RANGE      = 0.1;
   static const CVector3 KILOBOT_LIGHT_SENSOR_OFFSET = 
      CVector3(0.0,
               0.0,
               KILOBOT_LIGHT_SENSOR_ELEVATION) +
      CVector3(KILOBOT_LIGHT_SENSOR_RADIUS * Cos(KILOBOT_LIGHT_SENSOR_ANGLE),
               KILOBOT_LIGHT_SENSOR_RADIUS * Sin(KILOBOT_LIGHT_SENSOR_ANGLE),
               0.0f) +
      CVector3(KILOBOT_ECCENTRICITY,
               0.0,
               0.0);

   /* Communication RAB */
   static const Real KILOBOT_RAB_ELEVATION          = KILOBOT_HEIGHT + 0.001;
   


}


#endif
