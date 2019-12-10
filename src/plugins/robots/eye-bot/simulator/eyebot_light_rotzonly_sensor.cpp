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
         /* Parse noise injection */
         if(NodeExists(t_tree, "noise")) {
           TConfigurationNode& tNode = GetNode(t_tree, "noise");
           m_cNoiseInjector.Init(tNode);
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
      if(m_cNoiseInjector.Enabled()) {
         for(size_t i = 0; i < 24; ++i) {
            m_tReadings[i].Value += m_cNoiseInjector.InjectNoise();
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
                   "This sensor accesses the eye-bot light sensor. For a complete description\n"
                   "of its usage, refer to the ci_eyebot_light_sensor.h interface. For the XML\n"
                   "configuration, refer to the default light sensor.\n",
                   "Usable"
      );

}
