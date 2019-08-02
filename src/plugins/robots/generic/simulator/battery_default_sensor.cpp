/**
 * @file <argos3/plugins/simulator/sensors/battery_default_sensor.cpp>
 *
 * @author Adhavan Jayabalan <jadhu94@gmail.com>
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/battery_equipped_entity.h>

#include "battery_default_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   static CRange<Real> UNIT(0.0f, 1.0f);

   /****************************************/
   /****************************************/

   CBatteryDefaultSensor::CBatteryDefaultSensor() :
      m_pcEmbodiedEntity(NULL),
      m_pcBatteryEntity(NULL),
      m_pcRNG(NULL),
      m_bAddNoise(false) {}

   /****************************************/
   /****************************************/

   void CBatteryDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      try {
          m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
          m_pcBatteryEntity = &(c_entity.GetComponent<CBatteryEquippedEntity>("battery"));
          m_pcBatteryEntity->Enable();
      }
      catch(CARGoSException&  ex) {
          THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the battery default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBatteryDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         /* Execute standard logic */
         CCI_BatterySensor::Init(t_tree);
         /* Parse noise range */
         GetNodeAttributeOrDefault(t_tree, "noise_range", m_cNoiseRange, m_cNoiseRange);
         if(m_cNoiseRange.GetSpan() != 0) {
            m_bAddNoise = true;
            m_pcRNG = CRandom::CreateRNG("argos");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in default battery sensor", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   void CBatteryDefaultSensor::Update() {
      /* Save old charge value (used later for time left estimation) */
      Real fOldCharge = m_sReading.AvailableCharge;
      /* Update available charge as seen by the robot */
      m_sReading.AvailableCharge =
         m_pcBatteryEntity->GetAvailableCharge() /
         m_pcBatteryEntity->GetFullCharge();
      /* Add noise */
      if(m_bAddNoise) {
         m_sReading.AvailableCharge += m_pcRNG->Uniform(m_cNoiseRange);
         /* To trunc battery level between 0 and 1 */
         UNIT.TruncValue(m_sReading.AvailableCharge);
      }
      /* Update time left */
      Real fDiff = fOldCharge - m_sReading.AvailableCharge;
      if(Abs(fDiff) > 1e-6) {
         m_sReading.TimeLeft =
            fOldCharge *
            CPhysicsEngine::GetSimulationClockTick() /
            fDiff;
      }
      else {
         m_sReading.TimeLeft = std::numeric_limits<Real>::infinity();
      }
   }

   /****************************************/
   /****************************************/

   void CBatteryDefaultSensor::Reset() {
      /* TODO */
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CBatteryDefaultSensor,
                   "battery", "default",
                   "Adhavan Jayabalan [jadhu94@gmail.com]",
                   "1.0",
                   "A generic battery level sensor.",

                   "This sensor returns the current battery level of a robot. This sensor\n"
                   "can be used with any robot, since it accesses only the body component. In\n"
                   "controllers, you must include the ci_battery_sensor.h header.\n\n"

                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <battery implementation=\"default\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"

                   "OPTIONAL XML CONFIGURATION\n\n"

                   "It is possible to add uniform noise to the sensor, thus matching the\n"
                   "characteristics of a real robot better. You can add noise through the\n"
                   "attribute 'noise_range' as follows:\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <battery implementation=\"default\"\n"
                   "                 noise_range=\"-0.3:0.4\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n",

                   "Usable"
		  );

}
