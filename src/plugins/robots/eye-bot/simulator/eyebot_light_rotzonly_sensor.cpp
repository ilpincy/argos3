/**
 * @file <argos3/plugins/robots/eye-bot/simulator/eyebot_light_rotzonly_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/light_entity.h>
#include <argos3/plugins/simulator/entities/light_sensor_equipped_entity.h>

#include "eyebot_light_rotzonly_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   static CRange<Real> SENSOR_RANGE(0.0f, 1.0f);
   static CRadians SENSOR_SPACING      = CRadians(ARGOS_PI / 12.0f);
   static CRadians SENSOR_HALF_SPACING = SENSOR_SPACING * 0.5;

   /****************************************/
   /****************************************/

   static SInt32 Modulo(SInt32 n_value, SInt32 un_modulo) {
      while(n_value < 0) n_value += un_modulo;
      while(n_value >= un_modulo) n_value -= un_modulo;
      return n_value;
   }

   static Real ComputeReading(Real f_distance) {
      if(f_distance > 2.5f) {
         return 0.0f;
      }
      else {
         return ::exp(-f_distance * 2.0f);
      }
   }

   static Real ScaleReading(const CRadians& c_angular_distance) {
      if(c_angular_distance > CRadians::PI_OVER_TWO) {
         return 0.0f;
      }
      else {
         return (1.0f - 2.0f * c_angular_distance / CRadians::PI);
      }
   }

   /****************************************/
   /****************************************/

   CEyeBotLightRotZOnlySensor::CEyeBotLightRotZOnlySensor() :
      m_pcEmbodiedEntity(NULL),
      m_bShowRays(false),
      m_pcRNG(NULL),
      m_bAddNoise(false),
      m_cSpace(CSimulator::GetInstance().GetSpace()) {}

   /****************************************/
   /****************************************/

   void CEyeBotLightRotZOnlySensor::SetRobot(CComposableEntity& c_entity) {
      try {
         m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
         m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
         m_pcLightEntity = &(c_entity.GetComponent<CLightSensorEquippedEntity>("light_sensors"));
         m_pcLightEntity->Enable();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the eye-bot light default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CEyeBotLightRotZOnlySensor::Init(TConfigurationNode& t_tree) {
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
            m_cNoiseRange.Set(-fNoiseLevel, fNoiseLevel);
            m_pcRNG = CRandom::CreateRNG("argos");
         }
         m_tReadings.resize(m_pcLightEntity->GetNumSensors());
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in rot_z_only light sensor", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   void CEyeBotLightRotZOnlySensor::Update() {
      /* Erase readings */
      for(size_t i = 0; i < m_tReadings.size(); ++i) {
         m_tReadings[i].Value = 0.0f;
      }
      /* Get eye-bot orientation */
      CRadians cTmp1, cTmp2, cOrientationZ;
      m_pcEmbodiedEntity->GetOriginAnchor().Orientation.ToEulerAngles(cOrientationZ, cTmp1, cTmp2);
      /* Ray used for scanning the environment for obstacles */
      CRay3 cOcclusionCheckRay;
      cOcclusionCheckRay.SetStart(m_pcEmbodiedEntity->GetOriginAnchor().Position);
      CVector3 cRobotToLight;
      /* Buffer for the angle of the light wrt to the eye-bot */
      CRadians cAngleLightWrtEyebot;
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
      for(CSpace::TMapPerType::iterator it = mapLights.begin();
          it != mapLights.end();
          ++it) {
         /* Get a reference to the light */
         CLightEntity& cLight = *(any_cast<CLightEntity*>(it->second));
         /* Consider the light only if it has non zero intensity */
         if(cLight.GetIntensity() > 0.0f) {
            /* Set the ray end */
            cOcclusionCheckRay.SetEnd(cLight.GetPosition());
            /* Check occlusion between the eye-bot and the light */
            if(! GetClosestEmbodiedEntityIntersectedByRay(sIntersection,
                                                          cOcclusionCheckRay,
                                                          *m_pcEmbodiedEntity)) {
               /* The light is not occluded */
               if(m_bShowRays) {
                  m_pcControllableEntity->AddCheckedRay(false, cOcclusionCheckRay);
               }
               /* Get the distance between the light and the eye-bot */
               cOcclusionCheckRay.ToVector(cRobotToLight);
               /*
                * Linearly scale the distance with the light intensity
                * The greater the intensity, the smaller the distance
                */
               cRobotToLight /= cLight.GetIntensity();
               /* Get the angle wrt to eye-bot rotation */
               cAngleLightWrtEyebot = cRobotToLight.GetZAngle();
               cAngleLightWrtEyebot -= cOrientationZ;
               /*
                * Find closest sensor index to point at which ray hits eyebot body
                * Rotate whole body by half a sensor spacing (corresponding to placement of first sensor)
                * Division says how many sensor spacings there are between first sensor and point at which ray hits eyebot body
                * Increase magnitude of result of division to ensure correct rounding
                */
               Real fIdx = (cAngleLightWrtEyebot - SENSOR_HALF_SPACING) / SENSOR_SPACING;
               SInt32 nReadingIdx = (fIdx > 0) ? fIdx + 0.5f : fIdx - 0.5f;
               /* Set the actual readings */
               Real fReading = cRobotToLight.Length();
               /*
                * Take 6 readings before closest sensor and 6 readings after - thus we
                * process sensors that are with 180 degrees of intersection of light
                * ray with robot body
                */
               for(SInt32 nIndexOffset = -6; nIndexOffset < 7; ++nIndexOffset) {
                  UInt32 unIdx = Modulo(nReadingIdx + nIndexOffset, 24);
                  CRadians cAngularDistanceFromOptimalLightReceptionPoint = Abs((cAngleLightWrtEyebot - m_tReadings[unIdx].Angle).SignedNormalize());
                  /*
                   * ComputeReading gives value as if sensor was perfectly in line with
                   * light ray. We then linearly decrease actual reading from 1 (dist
                   * 0) to 0 (dist PI/2)
                   */
                  m_tReadings[unIdx].Value += ComputeReading(fReading) * ScaleReading(cAngularDistanceFromOptimalLightReceptionPoint);
               }
            }
            else {
               /* The ray is occluded */
               if(m_bShowRays) {
                  m_pcControllableEntity->AddCheckedRay(true, cOcclusionCheckRay);
                  m_pcControllableEntity->AddIntersectionPoint(cOcclusionCheckRay, sIntersection.TOnRay);
               }
            }
         }
      }
      /* Apply noise to the sensors */
      if(m_bAddNoise) {
         for(size_t i = 0; i < 24; ++i) {
            m_tReadings[i].Value += m_pcRNG->Uniform(m_cNoiseRange);
         }
      }
      /* Trunc the reading between 0 and 1 */
      for(size_t i = 0; i < 24; ++i) {
         SENSOR_RANGE.TruncValue(m_tReadings[i].Value);
      }
   }

   /****************************************/
   /****************************************/

   void CEyeBotLightRotZOnlySensor::Reset() {
      for(UInt32 i = 0; i < GetReadings().size(); ++i) {
         m_tReadings[i].Value = 0.0f;
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CEyeBotLightRotZOnlySensor,
                   "eyebot_light", "rot_z_only",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "The eye-bot light sensor (optimized for 2D).",
                   "This sensor accesses a set of light sensors. The sensors all return a value\n"
                   "between 0 and 1, where 0 means nothing within range and 1 means the perceived\n"
                   "light saturates the sensor. Values between 0 and 1 depend on the distance of\n"
                   "the perceived light. Each reading R is calculated with R=(I/x)^2, where x is the\n"
                   "distance between a sensor and the light, and I is the reference intensity of the\n"
                   "perceived light. The reference intensity corresponds to the minimum distance at\n"
                   "which the light saturates a sensor. The reference intensity depends on the\n"
                   "individual light, and it is set with the \"intensity\" attribute of the light\n"
                   "entity. In case multiple lights are present in the environment, each sensor\n"
                   "reading is calculated as the sum of the individual readings due to each light.\n"
                   "In other words, light wave interference is not taken into account. In\n"
                   "controllers, you must include the ci_light_sensor.h header.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <eyebot_light implementation=\"rot_z_only\" />\n"
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
                   "        <eyebot_light implementation=\"rot_z_only\"\n"
                   "                      show_rays=\"true\" />\n"
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
                   "        <eyebot_light implementation=\"rot_z_only\"\n"
                   "                      noise_level=\"0.1\" />\n"
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
