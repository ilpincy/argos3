/**
 * @file <argos3/plugins/robots/foot-bot/simulator/ring_proximity_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/simulator.h>

#include "ring_proximity_default_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   void CRingProximityDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         /* Show rays? */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
         /* Parse noise level */
         Real fNoiseLevel = 0.0f;
         GetNodeAttributeOrDefault(t_tree, "noise_level", fNoiseLevel, fNoiseLevel);
         if(fNoiseLevel < 0.0f) {
            THROW_ARGOSEXCEPTION("Can't specify a negative value for the noise level of the proximity sensor");
         }
         else if(fNoiseLevel > 0.0f) {
            m_bAddNoise = true;
            m_cNoiseRange.Set(-fNoiseLevel, fNoiseLevel);
            m_pcRNG = CRandom::CreateRNG("argos");
         }
         /* Parse proximity sensors */
         CVector3 cRingCenter;
         GetNodeAttributeOrDefault(t_tree, "center", cRingCenter, cRingCenter);
         Real fRadius;
         GetNodeAttribute(t_tree, "radius", fRadius);
         CDegrees cRingStartAngleDegrees;
         GetNodeAttributeOrDefault(t_tree, "start_angle", cRingStartAngleDegrees, cRingStartAngleDegrees);
         CRadians cRingStartAngleRadians = ToRadians(cRingStartAngleDegrees);
         Real fRange;
         GetNodeAttribute(t_tree, "range", fRange);
         UInt32 unNumSensors;
         GetNodeAttribute(t_tree, "num_sensors", unNumSensors);
         CRadians cSensorSpacing = CRadians::TWO_PI / unNumSensors;
         CVector3 cPos, cDir;
         m_tReadings.resize(unNumSensors);
         m_tAngles.resize(unNumSensors);
         m_vecSensors.resize(unNumSensors);
         for(UInt32 i = 0; i < unNumSensors; ++i) {
            m_tAngles[i] = cRingStartAngleRadians + i * cSensorSpacing;
            m_tAngles[i].SignedNormalize();
            cPos.Set(fRadius, 0.0f, 0.0f);
            cPos.RotateZ(m_tAngles[i]);
            cPos += cRingCenter;
            cDir.Set(fRange, 0.0f, 0.0f);
            cDir.RotateZ(m_tAngles[i]);
            cDir += cPos;
            m_vecSensors[i] = new SSensor(cPos, cDir, fRange);
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in default ring proximity sensor", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   REGISTER_SENSOR(CRingProximityDefaultSensor,
                   "ring_proximity", "default",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "A configurable proximity sensor in which sensors are placed along a ring.",
                   "This sensor accesses a set of identical proximity sensors placed along a ring.\n"
                   "The sensors all return a value between 0 and 1, where 0 means nothing within\n"
                   "range and 1 means an external object is touching the sensor. Values between 0\n"
                   "and 1 depend on the distance of the occluding object, and are calculated as\n"
                   "value=exp(-distance). In controllers, you must include the\n"
                   "ci_ring_proximity_sensor.h header.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <ring_proximity implementation=\"default\"\n"
                   "                        num_sensors=\"8\"\n"
                   "                        radius=\"0.1\"\n"
                   "                        range=\"1\">\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "The number of sensors to place is set by the attribute \"num_sensors\". The\n"
                   "attribute \"radius\" sets the radius of the ring. The range of the sensors\n"
                   "is set through the attribute \"range\".\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "By default, the center of the radius corresponds to the position of the embodied\n"
                   "entity associated to the robot. To change the center, the optional attribute\n"
                   "\"center\" is available:\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <ring_proximity implementation=\"default\"\n"
                   "                        num_sensors=\"8\"\n"
                   "                        radius=\"0.1\"\n"
                   "                        center=\"0.1,0.2,0.3\"\n"
                   "                        range=\"1\">\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "Also, by default, the first sensor is placed at angle 0 (i.e., along the local\n"
                   "X axis of the robot). To change this, set the attribute \"start_angle\":\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <ring_proximity implementation=\"default\"\n"
                   "                        num_sensors=\"8\"\n"
                   "                        radius=\"0.1\"\n"
                   "                        start_angle=\"30\"\n"
                   "                        range=\"1\">\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "It is possible to draw the rays shot by the proximity sensor in the OpenGL\n"
                   "visualization. This can be useful for sensor debugging but also to understand\n"
                   "what's wrong in your controller. In OpenGL, the rays are drawn in cyan when\n"
                   "they are not obstructed and in purple when they are. In case a ray is\n"
                   "obstructed, a black dot is drawn where the intersection occurred.\n"
                   "To turn this functionality on, add the attribute \"show_rays\" as in this\n"
                   "example:\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <ring_proximity implementation=\"default\"\n"
                   "                        num_sensors=\"8\"\n"
                   "                        radius=\"0.1\"\n"
                   "                        range=\"1\"\n"
                   "                        show_rays=\"true\">\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "It is possible to add uniform noise to the sensors, thus matching the\n"
                   "characteristics of a real robot better. This can be done with the attribute\n"
                   "\"noise_level\", whose allowed range is in [-1,1] and is added to the calculated\n"
                   "reading. The final sensor reading is always normalized in the [0-1] range.\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <ring_proximity implementation=\"default\"\n"
                   "                        num_sensors=\"8\"\n"
                   "                        radius=\"0.1\"\n"
                   "                        range=\"1\"\n"
                   "                        noise_level=\"0.1\">\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>",
                   "Usable"
		  );

}
