/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_distance_scanner_rotzonly_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "footbot_distance_scanner_rotzonly_sensor.h"
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real FOOTBOT_RADIUS           = 0.085036758f;

   static const Real SHORT_RANGE_MIN_DISTANCE = 0.0f;
   static const Real SHORT_RANGE_RAY_START    = FOOTBOT_RADIUS;
   static const Real SHORT_RANGE_RAY_END      = FOOTBOT_RADIUS + 0.26f;

   static const Real LONG_RANGE_MIN_DISTANCE  = 0.12f;
   static const Real LONG_RANGE_RAY_START     = FOOTBOT_RADIUS;
   static const Real LONG_RANGE_RAY_END       = FOOTBOT_RADIUS + 1.42f;

   static const Real SENSOR_ELEVATION         = 0.123199866f;

   /****************************************/
   /****************************************/

   CFootBotDistanceScannerRotZOnlySensor::CFootBotDistanceScannerRotZOnlySensor() :
      m_pcRNG(nullptr),
      m_bAddNoise(false),
      m_cSpace(CSimulator::GetInstance().GetSpace()),
      m_bShowRays(false) {}

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerRotZOnlySensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_FootBotDistanceScannerSensor::Init(t_tree);
         /* Show rays? */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
         /* Noise range */
         GetNodeAttributeOrDefault(t_tree, "noise_range", m_cNoiseRange, m_cNoiseRange);
         if(m_cNoiseRange.GetSpan() > 0.0f) {
            m_bAddNoise = true;
            m_pcRNG = CRandom::CreateRNG("argos");
         }

         /* sensor is enabled by default */
         Enable();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in foot-bot distance scanner rot_z_only sensor.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerRotZOnlySensor::SetRobot(CComposableEntity& c_entity) {
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
      m_pcDistScanEntity = &(c_entity.GetComponent<CFootBotDistanceScannerEquippedEntity>("distance_scanner"));
      m_pcDistScanEntity->Enable();
   }

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerRotZOnlySensor::Update() {
      /* sensor is disabled--nothing to do */
      if (IsDisabled()) {
        return;
      }
      /* Clear the maps */
      m_tReadingsMap.clear();
      m_tShortReadingsMap.clear();
      m_tLongReadingsMap.clear();
      /* Perform calculations only if the sensor is on */
      if(m_pcDistScanEntity->GetMode() != CFootBotDistanceScannerEquippedEntity::MODE_OFF) {
         /* Update the readings wrt to device mode */
         if(m_pcDistScanEntity->GetMode() == CFootBotDistanceScannerEquippedEntity::MODE_POSITION_CONTROL) {
            /* Sensor blocked in a position */
            /* Recalculate the rays */
            CalculateRaysNotRotating();
            /* Save the rotation for next time */
            m_cLastDistScanRotation = m_pcDistScanEntity->GetRotation();
            /* Update the values */
            UpdateNotRotating();
         }
         else {
            /* Rotating sensor */
            /* Recalculate the rays */
            CalculateRaysRotating();
            /* Update the values */
            UpdateRotating();
            /* Save the rotation for next time */
            m_cLastDistScanRotation = m_pcDistScanEntity->GetRotation();
         }
      }
   }

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerRotZOnlySensor::Reset() {
      /* Clear the maps */
      m_tReadingsMap.clear();
      m_tShortReadingsMap.clear();
      m_tLongReadingsMap.clear();
      /* Zero the last rotation */
      m_cLastDistScanRotation = CRadians::ZERO;
   }

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerRotZOnlySensor::UpdateNotRotating() {
      /* Short range [0] */
      CRadians cAngle = m_cLastDistScanRotation;
      cAngle.SignedNormalize();
      Real fReading = CalculateReadingForRay(m_cShortRangeRays0[0], SHORT_RANGE_MIN_DISTANCE);
      m_tShortReadingsMap[cAngle] = fReading;
      m_tReadingsMap[cAngle] = fReading;
      /* Long range [1] */
      cAngle += CRadians::PI_OVER_TWO;
      cAngle.SignedNormalize();
      fReading = CalculateReadingForRay(m_cLongRangeRays1[0], LONG_RANGE_MIN_DISTANCE);
      m_tLongReadingsMap[cAngle] = fReading;
      m_tReadingsMap[cAngle] = fReading;
      /* Short range [2] */
      cAngle += CRadians::PI_OVER_TWO;
      cAngle.SignedNormalize();
      fReading = CalculateReadingForRay(m_cShortRangeRays2[0], SHORT_RANGE_MIN_DISTANCE);
      m_tShortReadingsMap[cAngle] = fReading;
      m_tReadingsMap[cAngle] = fReading;
      /* Long range [3] */
      cAngle += CRadians::PI_OVER_TWO;
      cAngle.SignedNormalize();
      fReading = CalculateReadingForRay(m_cLongRangeRays3[0], LONG_RANGE_MIN_DISTANCE);
      m_tLongReadingsMap[cAngle] = fReading;
      m_tReadingsMap[cAngle] = fReading;
   }

   /****************************************/
   /****************************************/

#define ADD_READING(RAYS,MAP,INDEX,MINDIST)                 \
   cAngle += cInterSensorSpan;                              \
   cAngle.SignedNormalize();                                \
   fReading = CalculateReadingForRay(RAYS[INDEX],MINDIST);  \
   MAP[cAngle] = fReading;                                  \
   m_tReadingsMap[cAngle] = fReading;

#define ADD_READINGS(RAYS,MAP,MINDIST)          \
   ADD_READING(RAYS,MAP,1,MINDIST)              \
   ADD_READING(RAYS,MAP,2,MINDIST)              \
   ADD_READING(RAYS,MAP,3,MINDIST)              \
   ADD_READING(RAYS,MAP,4,MINDIST)              \
   ADD_READING(RAYS,MAP,5,MINDIST)

   void CFootBotDistanceScannerRotZOnlySensor::UpdateRotating() {
      CRadians cInterSensorSpan = (m_pcDistScanEntity->GetRotation() - m_cLastDistScanRotation).UnsignedNormalize() / 6.0f;
      CRadians cStartAngle = m_cLastDistScanRotation;
      /* Short range [0] */
      CRadians cAngle = cStartAngle;
      cAngle.SignedNormalize();
      Real fReading = CalculateReadingForRay(m_cShortRangeRays0[0], SHORT_RANGE_MIN_DISTANCE);
      m_tShortReadingsMap[cAngle] = fReading;
      m_tReadingsMap[cAngle] = fReading;
      ADD_READINGS(m_cShortRangeRays0, m_tShortReadingsMap, SHORT_RANGE_MIN_DISTANCE);
      /* Short range [2] */
      cAngle = cStartAngle + CRadians::PI;
      cAngle.SignedNormalize();
      fReading = CalculateReadingForRay(m_cShortRangeRays2[0], SHORT_RANGE_MIN_DISTANCE);
      m_tShortReadingsMap[cAngle] = fReading;
      m_tReadingsMap[cAngle] = fReading;
      ADD_READINGS(m_cShortRangeRays2, m_tShortReadingsMap, SHORT_RANGE_MIN_DISTANCE);
      /* Long range [1] */
      cAngle = cStartAngle + CRadians::PI_OVER_TWO;
      cAngle.SignedNormalize();
      fReading = CalculateReadingForRay(m_cLongRangeRays1[0], LONG_RANGE_MIN_DISTANCE);
      m_tLongReadingsMap[cAngle] = fReading;
      m_tReadingsMap[cAngle] = fReading;
      ADD_READINGS(m_cLongRangeRays1, m_tLongReadingsMap, LONG_RANGE_MIN_DISTANCE);
      /* Long range [3] */
      cAngle = cStartAngle + CRadians::PI_OVER_TWO + CRadians::PI;
      cAngle.SignedNormalize();
      fReading = CalculateReadingForRay(m_cLongRangeRays3[0], LONG_RANGE_MIN_DISTANCE);
      m_tLongReadingsMap[cAngle] = fReading;
      m_tReadingsMap[cAngle] = fReading;
      ADD_READINGS(m_cLongRangeRays3, m_tLongReadingsMap, LONG_RANGE_MIN_DISTANCE);
   }

   /****************************************/
   /****************************************/

   Real CFootBotDistanceScannerRotZOnlySensor::CalculateReadingForRay(const CRay3& c_ray,
                                                                      Real f_min_distance) {
      /* Get the closest intersection */
      SEmbodiedEntityIntersectionItem sIntersection;
      if(GetClosestEmbodiedEntityIntersectedByRay(sIntersection,
                                                  c_ray,
                                                  *m_pcEmbodiedEntity)) {
         if(m_bShowRays) m_pcControllableEntity->AddIntersectionPoint(c_ray, sIntersection.TOnRay);
         /* There is an intersection! */
         Real fDistance = c_ray.GetDistance(sIntersection.TOnRay);
         if(fDistance > f_min_distance) {
            /* The distance is returned in meters, but the reading must be in cm */
            if(m_bShowRays) m_pcControllableEntity->AddCheckedRay(true, c_ray);
            return fDistance * 100.0f;
         }
         else {
            /* The detected intersection was too close */
            if(m_bShowRays) m_pcControllableEntity->AddCheckedRay(true, c_ray);
            return -1.0f;
         }
      }
      else {
         /* No intersection */
         if(m_bShowRays) m_pcControllableEntity->AddCheckedRay(false, c_ray);
         return -2.0f;
      }
   }

   /****************************************/
   /****************************************/

/* Highly reuse the vectors to speed up the computation */
#define CALCULATE_SHORT_RANGE_RAY(ANGLE,INDEX)              \
   m_cDirection.RotateZ(ANGLE);                             \
   m_cOriginRayStart = m_cDirection;                        \
   m_cOriginRayEnd = m_cDirection;                          \
   m_cOriginRayStart *= SHORT_RANGE_RAY_START;              \
   m_cOriginRayEnd *= SHORT_RANGE_RAY_END;                  \
   m_cRayStart = m_pcEmbodiedEntity->GetOriginAnchor().Position;  \
   m_cRayStart += m_cOriginRayStart;                        \
   m_cRayStart.SetZ(m_cRayStart.GetZ() + SENSOR_ELEVATION); \
   m_cRayEnd = m_pcEmbodiedEntity->GetOriginAnchor().Position;  \
   m_cRayEnd += m_cOriginRayEnd;                            \
   m_cRayEnd.SetZ(m_cRayEnd.GetZ() + SENSOR_ELEVATION);     \
   m_cShortRangeRays0[INDEX].Set(m_cRayStart, m_cRayEnd);   \
   m_cRayStart = m_pcEmbodiedEntity->GetOriginAnchor().Position;  \
   m_cRayStart -= m_cOriginRayStart;                        \
   m_cRayStart.SetZ(m_cRayStart.GetZ() + SENSOR_ELEVATION); \
   m_cRayEnd = m_pcEmbodiedEntity->GetOriginAnchor().Position;  \
   m_cRayEnd -= m_cOriginRayEnd;                            \
   m_cRayEnd.SetZ(m_cRayEnd.GetZ() + SENSOR_ELEVATION);     \
   m_cShortRangeRays2[INDEX].Set(m_cRayStart, m_cRayEnd);   \
                                                            \
/* Highly reuse the vectors to speed up the computation */
#define CALCULATE_LONG_RANGE_RAY(ANGLE,INDEX)               \
   m_cDirection.RotateZ(ANGLE);                             \
   m_cOriginRayStart = m_cDirection;                        \
   m_cOriginRayEnd = m_cDirection;                          \
   m_cOriginRayStart *= LONG_RANGE_RAY_START;               \
   m_cOriginRayEnd *= LONG_RANGE_RAY_END;                   \
   m_cRayStart = m_pcEmbodiedEntity->GetOriginAnchor().Position;  \
   m_cRayStart += m_cOriginRayStart;                        \
   m_cRayStart.SetZ(m_cRayStart.GetZ() + SENSOR_ELEVATION); \
   m_cRayEnd = m_pcEmbodiedEntity->GetOriginAnchor().Position;  \
   m_cRayEnd += m_cOriginRayEnd;                            \
   m_cRayEnd.SetZ(m_cRayEnd.GetZ() + SENSOR_ELEVATION);     \
   m_cLongRangeRays1[INDEX].Set(m_cRayStart, m_cRayEnd);    \
   m_cRayStart = m_pcEmbodiedEntity->GetOriginAnchor().Position;  \
   m_cRayStart -= m_cOriginRayStart;                        \
   m_cRayStart.SetZ(m_cRayStart.GetZ() + SENSOR_ELEVATION); \
   m_cRayEnd = m_pcEmbodiedEntity->GetOriginAnchor().Position;  \
   m_cRayEnd -= m_cOriginRayEnd;                            \
   m_cRayEnd.SetZ(m_cRayEnd.GetZ() + SENSOR_ELEVATION);     \
   m_cLongRangeRays3[INDEX].Set(m_cRayStart, m_cRayEnd);

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerRotZOnlySensor::CalculateRaysNotRotating() {
      /* We make the assumption that the foot-bot is rotated only around Z */
      /* Get the foot-bot orientation */
      CRadians cTmp1, cTmp2, cOrientationZ;
      m_pcEmbodiedEntity->GetOriginAnchor().Orientation.ToEulerAngles(cOrientationZ, cTmp1, cTmp2);
      /* Sum the distance scanner orientation */
      cOrientationZ += m_pcDistScanEntity->GetRotation();
      /* Calculate the 2D vector representing this rotation */
      CVector2 cAbsoluteOrientation(1.0, cOrientationZ);
      /* The short range sensors are oriented along the foot-bot local X */
      m_cDirection = CVector3::X;
      CALCULATE_SHORT_RANGE_RAY(cAbsoluteOrientation, 0);
      /* The short range sensors are oriented along the foot-bot local Y */
      m_cDirection = CVector3::Y;
      CALCULATE_LONG_RANGE_RAY(cAbsoluteOrientation, 0);
   }

   /****************************************/
   /****************************************/

   void CFootBotDistanceScannerRotZOnlySensor::CalculateRaysRotating() {
      /* We make the assumption that the foot-bot is rotated only around Z */
      /* Get the foot-bot orientation */
      CRadians cTmp1, cTmp2, cOrientationZ;
      m_pcEmbodiedEntity->GetOriginAnchor().Orientation.ToEulerAngles(cOrientationZ, cTmp1, cTmp2);
      /* Sum the distance scanner orientation */
      cOrientationZ += m_cLastDistScanRotation;
      /* Calculate the 2D vector representing this rotation */
      CVector2 cAbsoluteOrientation(1.0, cOrientationZ);
      /* The sensor is rotating, so calculate the span between each successive ray */
      CVector2 cInterSensorSpan(1.0f, (m_pcDistScanEntity->GetRotation() - m_cLastDistScanRotation).UnsignedNormalize() / 6.0f);
      /* The short range sensors are oriented along the foot-bot local X */
      m_cDirection = CVector3::X;
      CALCULATE_SHORT_RANGE_RAY(cAbsoluteOrientation, 0);
      CALCULATE_SHORT_RANGE_RAY(cInterSensorSpan, 1);
      CALCULATE_SHORT_RANGE_RAY(cInterSensorSpan, 2);
      CALCULATE_SHORT_RANGE_RAY(cInterSensorSpan, 3);
      CALCULATE_SHORT_RANGE_RAY(cInterSensorSpan, 4);
      CALCULATE_SHORT_RANGE_RAY(cInterSensorSpan, 5);
      /* The long range sensors are oriented along the foot-bot local Y */
      m_cDirection = CVector3::Y;
      CALCULATE_LONG_RANGE_RAY(cAbsoluteOrientation, 0);
      CALCULATE_LONG_RANGE_RAY(cInterSensorSpan, 1);
      CALCULATE_LONG_RANGE_RAY(cInterSensorSpan, 2);
      CALCULATE_LONG_RANGE_RAY(cInterSensorSpan, 3);
      CALCULATE_LONG_RANGE_RAY(cInterSensorSpan, 4);
      CALCULATE_LONG_RANGE_RAY(cInterSensorSpan, 5);
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CFootBotDistanceScannerRotZOnlySensor,
                   "footbot_distance_scanner", "rot_z_only",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "The foot-bot distance scanner sensor (optimized for 2D).",
                   "This sensor accesses the foot-bot distance scanner sensor. For a complete\n"
                   "description of its usage, refer to the common interface.\n"
                   "In this implementation, the readings are calculated under the assumption that\n"
                   "the foot-bot is always parallel to the XY plane, i.e., it rotates only around\n"
                   "the Z axis. This implementation is faster than a 3D one and should be used\n"
                   "only when the assumption about the foot-bot rotation holds.\n\n"

                   "This sensor is enabled by default.\n\n"

                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <footbot_distance_scanner implementation=\"rot_z_only\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "It is possible to draw the rays shot by the distance scanner in the OpenGL\n"
                   "visualization. This can be useful for sensor debugging but also to understand\n"
                   "what's wrong in your controller. In OpenGL, the rays are drawn in cyan when\n"
                   "they are not obstructed and in purple when they are. In case a ray is\n"
                   "obstructed, a black dot is drawn where the intersection occurred.\n"
                   "To turn this functionality on, add the attribute 'show_rays=\"true\"' in the\n"
                   "XML as in this example:\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <footbot_distance_scanner implementation=\"rot_z_only\"\n"
                   "                                  show_rays=\"true\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n",
                   "Usable"
      );

}
