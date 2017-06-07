/**
 * @file <argos3/plugins/robots/kilobot/simulator/kilobot_light_rotzonly_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/light_entity.h>
#include <argos3/plugins/simulator/entities/light_sensor_equipped_entity.h>

#include "kilobot_measures.h"
#include "kilobot_light_rotzonly_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   static CRange<SInt16> SENSOR_RANGE(-1, 1024);

   /****************************************/
   /****************************************/

   /* This function models the actual response of the sensor to
      distance and direction. It should match the specific conditions
      used in experiments */
   static SInt16 ComputeReading(const Real f_distance,
                                const CRadians& c_angular_distance) {
      if(f_distance < 2.5f) {
         Real fReading = Exp(-f_distance * 2.0f);
         fReading *= 1.0f - c_angular_distance / CRadians::PI;
         fReading *= 1024.0;
         return Floor(fReading);
      }
      else
         return 0;
   }

   /****************************************/
   /****************************************/

   CKilobotLightRotZOnlySensor::CKilobotLightRotZOnlySensor() :
      m_pcEmbodiedEntity(NULL),
      m_bShowRays(false),
      m_pcRNG(NULL),
      m_bAddNoise(false),
      m_cSpace(CSimulator::GetInstance().GetSpace()) {}

   /****************************************/
   /****************************************/

   void CKilobotLightRotZOnlySensor::SetRobot(CComposableEntity& c_entity) {
      try {
         m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
         m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
         m_pcLightEntity = &(c_entity.GetComponent<CLightSensorEquippedEntity>("light_sensors"));
         m_pcLightEntity->Enable();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the kilobot light default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CKilobotLightRotZOnlySensor::Init(TConfigurationNode& t_tree) {
      try {
         /* Show rays? */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
         /* Parse noise level */
         Real fNoiseLevel = 0.0f;
         GetNodeAttributeOrDefault(t_tree, "noise_level", fNoiseLevel, fNoiseLevel);
         if(fNoiseLevel < 0.0f) {
            THROW_ARGOSEXCEPTION("Can't specify a negative value for the noise level of the light sensor");
         }
         else if(fNoiseLevel > 0.0f) {
            m_bAddNoise = true;
            m_cNoiseRange.Set(-fNoiseLevel*SENSOR_RANGE.GetMax(), fNoiseLevel*SENSOR_RANGE.GetMax());
            m_pcRNG = CRandom::CreateRNG("argos");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in rot_z_only light sensor", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   void CKilobotLightRotZOnlySensor::Update() {
      /* Erase reading */
      m_nReading = 0;
      /* Get kilobot orientation in the world */
      CRadians cOrientationZ, cOrientationY, cOrientationX;
      m_pcEmbodiedEntity->GetOriginAnchor().Orientation.ToEulerAngles(cOrientationZ,
                                                                      cOrientationY,
                                                                      cOrientationX);
      /* Ray used for scanning the environment for obstacles */
      CRay3 cOcclusionCheckRay;
      cOcclusionCheckRay.SetStart(m_pcLightEntity->GetSensor(0).Anchor.Position);
      CVector3 cRobotToLight;
      /* Buffer for the angle of the light wrt to the kilobot */
      CRadians cAngleLightWrtKilobot;
      /* Buffers to contain data about the intersection */
      SEmbodiedEntityIntersectionItem sIntersection;
      /* List of light entities */
      CSpace::TMapPerType& mapLights = m_cSpace.GetEntitiesByType("light");
      /*
       * 1. go through the list of light entities in the scene
       * 2. check if a light is occluded
       * 3. if it isn't, distribute the reading across the sensors
       *    NOTE: the readings are additive
       * 4. go through the sensors and clamp their values
       */
      for(CSpace::TMapPerType::iterator it = mapLights.begin(); it != mapLights.end(); ++it) {
         /* Get a reference to the light */
         CLightEntity& cLight = *(any_cast<CLightEntity*>(it->second));
         /* Consider the light only if it has non zero intensity */
         if(cLight.GetIntensity() > 0.0f) {
            /* Set the ray end */
            cOcclusionCheckRay.SetEnd(cLight.GetPosition());
            /* Check occlusion between the kilobot and the light */
            if( !GetClosestEmbodiedEntityIntersectedByRay(sIntersection,
                                                          cOcclusionCheckRay,
                                                          *m_pcEmbodiedEntity)) {
               /* The light is not occluded */
               if(m_bShowRays)
                  m_pcControllableEntity->AddCheckedRay(false, cOcclusionCheckRay);
               /* Get the distance between the light and the kilobot */
               cOcclusionCheckRay.ToVector(cRobotToLight);
               /*
                * Linearly scale the distance with the light intensity
                * The greater the intensity, the smaller the distance
                */
               cRobotToLight /= cLight.GetIntensity();
               /* Get the angle wrt to kilobot rotation */
               cAngleLightWrtKilobot = cRobotToLight.GetZAngle();
               cAngleLightWrtKilobot -= cOrientationZ;
               /* Set the actual readings */
               Real fReading = cRobotToLight.Length();
               CRadians cAngularDistanceFromOptimalLightReceptionPoint =
                  Abs((cAngleLightWrtKilobot - KILOBOT_LIGHT_SENSOR_ANGLE).SignedNormalize());
               m_nReading += ComputeReading(fReading,
                                            cAngularDistanceFromOptimalLightReceptionPoint);
            }
            else {
               /* The ray is occluded */
               if(m_bShowRays) {
                  m_pcControllableEntity->AddCheckedRay(true, cOcclusionCheckRay);
                  m_pcControllableEntity->AddIntersectionPoint(cOcclusionCheckRay,
                                                               sIntersection.TOnRay);
               }
            }
         }
      }
      /* Apply noise to the sensors */
      if(m_bAddNoise)
         m_nReading += m_pcRNG->Uniform(m_cNoiseRange);
      /* Trunc the reading between 0 and 1 */
      SENSOR_RANGE.TruncValue(m_nReading);
   }

   /****************************************/
   /****************************************/

   void CKilobotLightRotZOnlySensor::Reset() {
      m_nReading = 0;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CKilobotLightRotZOnlySensor,
                   "kilobot_light", "rot_z_only",
                   "Carlo Pinciroli [ilpincy@gmail.com] - Vito Trianni [vito.trianni@istc.cnr.it]",
                   "1.0",
                   "The kilobot light sensor (optimized for 2D).",
                   "This sensor returns a value between 0 and 1, where 0 means nothing within range\n"
                   "and 1 means that the perceived light saturates the sensor. Values between 0 and 1\n"
                   "depend on the distance of the perceived light. Each reading R is calculated with\n"
                   "R=(I/x)^2, where x is the distance between a sensor and the light, and I is the\n"
                   "reference intensity of the perceived light. The reference intensity corresponds to\n"
                   "the minimum distance at which the light saturates a sensor. The reference\n"
                   "intensity depends on the individual light, and it is set with the \"intensity\"\n"
                   "attribute of the light entity. In case multiple lights are present in the\n"
                   "environment, each sensor reading is calculated as the sum of the individual readings\n"
                   "due to each light. In other words, light wave interference is not taken into account.\n"
                   "In controllers, you must include the ci_light_sensor.h header.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <kilobot_light implementation=\"rot_z_only\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "It is possible to draw the rays shot by the light sensor in the OpenGL\n"
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
                   "        <kilobot_light implementation=\"rot_z_only\"\n"
                   "                       show_rays=\"true\" />\n"
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
                   "        <kilobot_light implementation=\"rot_z_only\"\n"
                   "                       noise_level=\"0.1\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None.\n",
                   "Usable"
      );

}
