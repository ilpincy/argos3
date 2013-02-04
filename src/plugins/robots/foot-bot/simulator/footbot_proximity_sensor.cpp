/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_proximity_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/simulator.h>

#include "footbot_proximity_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   static const Real FOOTBOT_RADIUS     = 0.085036758f;
   static const Real RAY_END_DISTANCE   = FOOTBOT_RADIUS + 0.1f;
   static const Real SENSOR_ELEVATION   = 0.06f; //TODO: fix (measured by hand...)

   /****************************************/
   /****************************************/

   /* Transform the distance (in cm) into a reading */
   static Real MapReadingToDistance(Real f_distance){
      return (4117.58f/(f_distance + 0.0163144f)) - 493.662f;
   }

   /****************************************/
   /****************************************/

   CFootBotProximitySensor::CFootBotProximitySensor() :
      m_cSpace(CSimulator::GetInstance().GetSpace()),
      m_cEmbodiedSpaceHash(m_cSpace.GetEmbodiedEntitiesSpaceHash()),
      m_pcEmbodiedEntity(NULL),
      m_bShowRays(false),
      m_bCalibrated(true),
      m_pcRNG(NULL),
      m_bAddNoise(false) {}

   /****************************************/
   /****************************************/

   void CFootBotProximitySensor::SetEntity(CEntity& c_entity) {
      CSimulatedSensor<CFootBotEntity>::SetEntity(c_entity);
      m_pcEmbodiedEntity = &(GetEntity().GetEmbodiedEntity());
   }

   /****************************************/
   /****************************************/

   void CFootBotProximitySensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_FootBotProximitySensor::Init(t_tree);
         /* Show rays? */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
         /* Parse noise level */
         Real fNoiseLevel = 0.0f;
         GetNodeAttributeOrDefault(t_tree, "noise_level", fNoiseLevel, fNoiseLevel);
         if(fNoiseLevel < 0.0f) {
            THROW_ARGOSEXCEPTION("Can't specify a negative value for the noise level of the foot-bot proximity sensor");
         }
         else if(fNoiseLevel > 0.0f) {
            m_bAddNoise = true;
            m_cNoiseRange.Set(-fNoiseLevel * READING_RANGE.GetMax(),
                              fNoiseLevel * READING_RANGE.GetMax());
         }
         /* Parse calibration flag */
         GetNodeAttributeOrDefault(t_tree, "calibrate", m_bCalibrated, m_bCalibrated);
         /* Random number generator*/
         m_pcRNG = CRandom::CreateRNG("argos");
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in foot-bot proximity sensor", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   void CFootBotProximitySensor::Reset() {
      for(UInt32 i = 0; i < CCI_FootBotProximitySensor::NUM_READINGS; ++i) {
         m_tReadings[i].Value = 0.0f;
      }
   }

   /****************************************/
   /****************************************/

   void CFootBotProximitySensor::Update() {
      /* We make the assumption that the foot-bot is rotated only around Z */
      /* Get the foot-bot orientation */
      CRadians cTmp1, cTmp2, cOrientationZ;
      m_pcEmbodiedEntity->GetOrientation().ToEulerAngles(cOrientationZ, cTmp1, cTmp2);
      /* Ray used for scanning the environment for obstacles */
      CRay cScanningRay;
      CVector3 cRayStart, cRayEnd;
      /* Buffers to contain data about the intersection */
      CSpace::SEntityIntersectionItem<CEmbodiedEntity> sIntersection;
      Real fDistance;
      /* Ignore the sensing robot when checking for occlusions */
      TEmbodiedEntitySet tIgnoreEntities;
      tIgnoreEntities.insert(m_pcEmbodiedEntity);
      cRayStart.SetZ(SENSOR_ELEVATION);
      cRayStart += m_pcEmbodiedEntity->GetPosition();
      /* Go through the sensors */
      for(UInt32 i = 0; i < CCI_FootBotProximitySensor::NUM_READINGS; ++i) {
         /* Compute ray for sensor i */
         cRayEnd = CVector3::X;
         cRayEnd.RotateZ(m_tReadings[i].Angle + cOrientationZ);
         cRayEnd *= RAY_END_DISTANCE;
         cRayEnd.SetZ(SENSOR_ELEVATION);
         cRayEnd += m_pcEmbodiedEntity->GetPosition();
         cScanningRay.Set(cRayStart,cRayEnd);
         /* Compute reading */
         /* Get the closest intersection */
         if(m_cSpace.GetClosestEmbodiedEntityIntersectedByRay(sIntersection, cScanningRay, tIgnoreEntities)) {
            /* There is an intersection */
            if(m_bShowRays) {
               GetEntity().GetControllableEntity().AddIntersectionPoint(cScanningRay, sIntersection.TOnRay);
               GetEntity().GetControllableEntity().AddCheckedRay(true, cScanningRay);
            }
            /* There is an intersection */
            fDistance = (cScanningRay.GetDistance(sIntersection.TOnRay) - FOOTBOT_RADIUS) * 100.f;
            if(fDistance < 0.0) {
               fDistance = 0.0;
            }
            /* Convert distance to cm for the distance-reading mapping function */
            m_tReadings[i].Value = MapReadingToDistance(fDistance);
         }
         else {
            /* No intersection */
            m_tReadings[i].Value = 0.0f;
            if(m_bShowRays) {
               GetEntity().GetControllableEntity().AddCheckedRay(false, cScanningRay);
            }
         }
         /* Apply noise to the sensor */
         if(m_bAddNoise) {
            m_tReadings[i].Value += m_pcRNG->Uniform(m_cNoiseRange);
         }
         /* Normalize reading between 0 and 1, only if calibration has been performed */
         if( m_bCalibrated ) {
            m_tReadings[i].Value = READING_RANGE.NormalizeValue(m_tReadings[i].Value);
         }
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CFootBotProximitySensor,
                   "footbot_proximity", "rot_z_only",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "The foot-bot proximity sensor (optimized for 2D)",
                   "This sensor accesses the foot-bot proximity sensor. For a complete\n"
                   "description of its usage, refer to the ci_footbot_proximity_sensor.h file.\n"
                   "In this implementation, the readings are calculated under the assumption that\n"
                   "the foot-bot is always parallel to the XY plane, i.e., it rotates only around\n"
                   "the Z axis. This implementation is faster than a 3D one and should be used\n"
                   "only when the assumption about the foot-bot rotation holds.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <footbot_proximity implementation=\"rot_z_only\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "It is possible to draw the rays shot by the proximity sensor in the OpenGL\n"
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
                   "        <footbot_proximity implementation=\"rot_z_only\"\n"
                   "                           show_rays=\"true\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "It is possible to specify noisy sensors in order to match the characteristics\n"
                   "of the real robot. This can be done with two xml parameters: 'noise_level'\n"
                   "indicates a percentage of the sensor range that is added to the actual sensor\n"
                   "reading. The added noise value is uniformly random on this range.\n\n" 
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <footbot_proximity implementation=\"rot_z_only\"\n"
                   "                           noise_level=\"0.05\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "Use the flag 'calibrate' to specify that the readings must be scaled in [0:1]:\n\n"
                   "  <controllers>\n"
                   "    ...\n"		 
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <footbot_motor_ground implementation=\"rot_z_only\"\n"
                   "                              calibrate=\"true\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n",
                   "Usable"
		  );

}
