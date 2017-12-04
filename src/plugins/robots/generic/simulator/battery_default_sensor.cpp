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
          SetReading(m_pcBatteryEntity->GetBatLevel(),0,0);
      }
      catch(CARGoSException&  ex) {
          THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the battery default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBatteryDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_BatterySensor::Init(t_tree);
         /* Parse noise range */
         GetNodeAttributeOrDefault(t_tree, "level_noise_range", m_cLevelNoiseRange, m_cLevelNoiseRange);

         if(m_cLevelNoiseRange.GetSpan() != 0 ) {
            m_bAddNoise = true;
            m_pcRNG = CRandom::CreateRNG("argos");
         }

         m_unTicksPerSecond = (UInt8)CPhysicsEngine::GetInverseSimulationClockTick();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in default battery sensor", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   void CBatteryDefaultSensor::Update() {

      static UInt16 FullCapacity = m_pcBatteryEntity->GetFullCapacity();
      static Real PrevLevel = m_sReading.Level;
      /** For now  the battery losses energy at a constant rate */
      m_sReading.Level = m_pcBatteryEntity->GetBatLevel();
      if(m_bAddNoise) {
         m_sReading.Level += m_pcRNG->Uniform(m_cLevelNoiseRange);
         /* To trunc battery level between 0 and 1 */
         UNIT.TruncValue(m_sReading.Level);
      }
      Real Discharge = Abs(PrevLevel - m_sReading.Level);

      /* If discharge is zero or close to zero, don't change other battery parameter */
      if (Discharge > 1e-10) {
          m_sReading.TimeRemaining = (UInt16)(m_sReading.Level/(m_unTicksPerSecond*Discharge));
          m_sReading.AvailCapacity = (UInt16)(m_sReading.Level*FullCapacity);
          PrevLevel = m_sReading.Level;
      }
   }

   /****************************************/
   /****************************************/

   void CBatteryDefaultSensor::Reset() {
      SetReading(1.0f,0,0); //todo
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
                   "attribute 'level_noise_range'.\n"
                   "Attribute 'level_noise_range' regulates the noise range on the battery level\n"
                   "returned by the sensor.\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <battery implementation=\"default\"\n"
                   "                 level_noise_range=\"-0.3:0.4\" />\n"
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
