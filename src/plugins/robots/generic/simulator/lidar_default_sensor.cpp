/**
 * @file <argos3/plugins/simulator/sensors/lidar_default_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Andreas Pasternak - <andreas.pasternak@gmx.ch>
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/lidar_sensor_equipped_entity.h>

#include "lidar_default_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   CLidarDefaultSensor::CLidarDefaultSensor() :
      m_pcEmbodiedEntity(NULL),
      m_bShowRays(false),
      m_pcRNG(NULL),
      m_bAddNoise(false),
      m_cSpace(CSimulator::GetInstance().GetSpace()) {}

   /****************************************/
   /****************************************/

   void CLidarDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      try {
         m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
         m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
         m_pcLidarEntity = &(c_entity.GetComponent<CLidarSensorEquippedEntity>("lidar_sensors"));
         m_pcLidarEntity->Enable();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the lidar default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CLidarDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_LidarSensor::Init(t_tree);
         /* Show rays? */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
         /* Parse noise level */
         Real fNoiseLevel = 0.0f;
         GetNodeAttributeOrDefault(t_tree, "noise_level", fNoiseLevel, fNoiseLevel);
         if(fNoiseLevel < 0.0f) {
            THROW_ARGOSEXCEPTION("Can't specify a negative value for the noise level of the lidar sensor");
         }
         else if(fNoiseLevel > 0.0f) {
            m_bAddNoise = true;
            m_cNoiseRange.Set(-fNoiseLevel, fNoiseLevel);
            m_pcRNG = CRandom::CreateRNG("argos");
         }
         m_tReadings.resize(m_pcLidarEntity->GetNumSensors());
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in default lidar sensor", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   void CLidarDefaultSensor::Update() {
      /* Ray used for scanning the environment for obstacles */
      CRay3 cScanningRay;
      CVector3 cRayStart, cRayEnd;
      /* Buffers to contain data about the intersection */
      SEmbodiedEntityIntersectionItem sIntersection;
      /* Go through the sensors */
      for(UInt32 i = 0; i < m_tReadings.size(); ++i) {
         /* Compute ray for sensor i */
         cRayStart = m_pcLidarEntity->GetSensor(i).Offset;
         cRayStart.Rotate(m_pcLidarEntity->GetSensor(i).Anchor.Orientation);
         cRayStart += m_pcLidarEntity->GetSensor(i).Anchor.Position;
         cRayEnd = m_pcLidarEntity->GetSensor(i).Offset;
         cRayEnd += m_pcLidarEntity->GetSensor(i).Direction;
         cRayEnd.Rotate(m_pcLidarEntity->GetSensor(i).Anchor.Orientation);
         cRayEnd += m_pcLidarEntity->GetSensor(i).Anchor.Position;
         cScanningRay.Set(cRayStart,cRayEnd);
         /* Compute reading */
         /* Get the closest intersection */
         if(GetClosestEmbodiedEntityIntersectedByRay(sIntersection,
                                                     cScanningRay,
                                                     *m_pcEmbodiedEntity)) {
            /* There is an intersection */
            if(m_bShowRays) {
               m_pcControllableEntity->AddIntersectionPoint(cScanningRay,
                                                            sIntersection.TOnRay);
               m_pcControllableEntity->AddCheckedRay(true, cScanningRay);
            }
            m_tReadings[i] = CalculateReading(cScanningRay.GetDistance(sIntersection.TOnRay));
         }
         else {
            /* No intersection, reading will show the maximum sensor range. */
            m_tReadings[i] = (cRayEnd-cRayStart).Length();
            if(m_bShowRays) {
               m_pcControllableEntity->AddCheckedRay(false, cScanningRay);
            }
         }
         /* Apply noise to the sensor */
         if(m_bAddNoise) {
            m_tReadings[i] += m_pcRNG->Uniform(m_cNoiseRange);
         }

      }
   }

   /****************************************/
   /****************************************/

   void CLidarDefaultSensor::Reset() {
      CVector3 cRayStart, cRayEnd;
      for(UInt32 i = 0; i < GetReadings().size(); ++i) {
         /* Compute ray for sensor i */
         cRayStart = m_pcLidarEntity->GetSensor(i).Offset;
         cRayStart.Rotate(m_pcLidarEntity->GetSensor(i).Anchor.Orientation);
         cRayStart += m_pcLidarEntity->GetSensor(i).Anchor.Position;
         cRayEnd = m_pcLidarEntity->GetSensor(i).Offset;
         cRayEnd += m_pcLidarEntity->GetSensor(i).Direction;
         cRayEnd.Rotate(m_pcLidarEntity->GetSensor(i).Anchor.Orientation);
         cRayEnd += m_pcLidarEntity->GetSensor(i).Anchor.Position;

         /* Set the readings to the maximum sensor range. */
         m_tReadings[i] = (cRayEnd-cRayStart).Length();
      }
   }

   /****************************************/
   /****************************************/

   Real CLidarDefaultSensor::CalculateReading(Real f_distance) {
      return f_distance;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CLidarDefaultSensor,
                   "lidar", "default",
                   "Carlo Pinciroli [ilpincy@gmail.com], Andreas Pasternak [andreas.pasternak@gmx.ch]",
                   "1.0",
                   "A generic lidar sensor.",
                   "This sensor accesses a lidar scanner. The readings are\n"
                   "between 0 and the configured sensor maximal range and represent the distance \n"
                   "of the occluding object. In controllers, you must include the ci_lidar_sensor.h header.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <lidar implementation=\"default\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "It is possible to draw the rays shot by the lidar sensor in the OpenGL\n"
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
                   "        <lidar implementation=\"default\"\n"
                   "                   show_rays=\"true\" />\n"
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
                   "        <lidar implementation=\"default\"\n"
                   "                   noise_level=\"0.1\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n",
                   "Usable"
		  );

}
