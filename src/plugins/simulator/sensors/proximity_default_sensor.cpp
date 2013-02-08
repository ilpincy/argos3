/**
 * @file <argos3/plugins/robots/foot-bot/simulator/ring_proximity_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/simulator.h>

#include "proximity_default_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   static CRange<Real> UNIT(0.0f, 0.1f);

   /****************************************/
   /****************************************/

   CProximityDefaultSensor::CProximityDefaultSensor() :
      m_pcEmbodiedEntity(NULL),
      m_bShowRays(false),
      m_pcRNG(NULL),
      m_bAddNoise(false),
      m_cSpace(CSimulator::GetInstance().GetSpace()),
      m_cEmbodiedSpaceHash(m_cSpace.GetEmbodiedEntitiesSpaceHash()) {}

   /****************************************/
   /****************************************/

   CProximityDefaultSensor::~CProximityDefaultSensor() {
      while(!m_vecSensors.empty()) {
         delete m_vecSensors.back();
         m_vecSensors.pop_back();
      }
   }

   /****************************************/
   /****************************************/

   void CProximityDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      m_pcControllableEntity = &(c_entity.GetComponent<CControllableEntity>("controller"));
      /* Ignore the sensing robot when checking for occlusions */
      m_tIgnoreMe.insert(m_pcEmbodiedEntity);
   }

   /****************************************/
   /****************************************/

   void CProximityDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_ProximitySensor::Init(t_tree);
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
         if(t_tree.NoChildren()) {
            THROW_ARGOSEXCEPTION("No sensors defined");
         }
         TConfigurationNodeIterator it;
         CVector3 cPos, cDir;
         Real fRange;
         for(it = it.begin(&t_tree); it != it.end(); ++it) {
            if(it->Value() != "sensor") {
               THROW_ARGOSEXCEPTION("Parse error: expected <sensor />, read \"" << it->Value() << "\"");
            }
            GetNodeAttribute(*it, "position", cPos);
            GetNodeAttribute(*it, "direction", cDir);
            GetNodeAttribute(*it, "range", fRange);
            m_vecSensors.push_back(new SSensor(cPos, cDir, fRange));
         }
         m_tReadings.resize(m_vecSensors.size());
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in default proximity sensor", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   void CProximityDefaultSensor::Update() {
      /* Ray used for scanning the environment for obstacles */
      CRay cScanningRay;
      CVector3 cRayStart, cRayEnd;
      /* Buffers to contain data about the intersection */
      CSpace::SEntityIntersectionItem<CEmbodiedEntity> sIntersection;
      /* Go through the sensors */
      for(UInt32 i = 0; i < m_vecSensors.size(); ++i) {
         /* Compute ray for sensor i */
         cRayStart = m_vecSensors[i]->Position;
         cRayStart.Rotate(m_pcEmbodiedEntity->GetOrientation());
         cRayStart += m_pcEmbodiedEntity->GetPosition();
         cRayEnd = m_vecSensors[i]->Position;
         cRayEnd += m_vecSensors[i]->Direction;
         cRayEnd.Rotate(m_pcEmbodiedEntity->GetOrientation());
         cRayEnd += m_pcEmbodiedEntity->GetPosition();
         cScanningRay.Set(cRayStart,cRayEnd);
         /* Compute reading */
         /* Get the closest intersection */
         if(m_cSpace.GetClosestEmbodiedEntityIntersectedByRay(sIntersection,
                                                              cScanningRay,
                                                              m_tIgnoreMe)) {
            /* There is an intersection */
            if(m_bShowRays) {
               m_pcControllableEntity->AddIntersectionPoint(cScanningRay,
                                                            sIntersection.TOnRay);
               m_pcControllableEntity->AddCheckedRay(true, cScanningRay);
            }
            CalculateReading(cScanningRay.GetDistance(sIntersection.TOnRay));
         }
         else {
            /* No intersection */
            m_tReadings[i] = 0.0f;
            if(m_bShowRays) {
               m_pcControllableEntity->AddCheckedRay(false, cScanningRay);
            }
         }
         /* Apply noise to the sensor */
         if(m_bAddNoise) {
            m_tReadings[i] += m_pcRNG->Uniform(m_cNoiseRange);
         }
         /* Trunc the reading between 0 and 1 */
         UNIT.TruncValue(m_tReadings[i]);
      }
   }

   /****************************************/
   /****************************************/

   void CProximityDefaultSensor::Reset() {
      for(UInt32 i = 0; i < GetReadings().size(); ++i) {
         m_tReadings[i] = 0.0f;
      }
   }

   /****************************************/
   /****************************************/

   Real CProximityDefaultSensor::CalculateReading(Real f_distance) {
      return Exp(-f_distance);
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CProximityDefaultSensor,
                   "ring_proximity", "rot_z_only",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "The foot-bot proximity sensor (optimized for 2D)",
                   "This sensor accesses the foot-bot proximity sensor. For a complete\n"
                   "description of its usage, refer to the ci_ring_proximity_sensor.h file.\n"
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
                   "        <ring_proximity implementation=\"rot_z_only\" />\n"
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
                   "        <ring_proximity implementation=\"rot_z_only\"\n"
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
                   "        <ring_proximity implementation=\"rot_z_only\"\n"
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
                   "        <ring_motor_ground implementation=\"rot_z_only\"\n"
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
