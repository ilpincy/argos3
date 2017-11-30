/**
 * @file <argos3/plugins/simulator/sensors/battery_default_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/battery_sensor_equipped_entity.h>

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
          m_pcBatteryEntity = &(c_entity.GetComponent<CBatterySensorEquippedEntity>("battery_sensors"));
          m_pcBatteryEntity->Enable();
          SetReading(1.0f,0,0); //todo
      }
      catch(CARGoSException&  ex) {
          THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the battery default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CBatteryDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         std::cout<<"Battery loaded";
         CCI_BatterySensor::Init(t_tree);
         /* Parse noise range */
         GetNodeAttributeOrDefault(t_tree, "level_noise_range", m_cLevelNoiseRange, m_cLevelNoiseRange);
         /* Parse discharge rate */
         Real discharge = 0.0f;
         GetNodeAttributeOrDefault(t_tree, "discharge_rate", discharge, discharge);
         m_fDischarge = discharge;
         if(m_cLevelNoiseRange.GetSpan() != 0 ) {
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
      /** For now  the battery losses energy at a constant rate */
      m_sReading.Level -= m_fDischarge; //todo
      if(m_bAddNoise) {
         m_sReading.Level += m_pcRNG->Uniform(m_cLevelNoiseRange);
         /* To trunc battery level between 0 and 1 */
         UNIT.TruncValue(m_sReading.Level);
      }
      // for now hard coded
      UInt8 tick_per_second = 10;  //Real CPhysicsEngine::GetInverseSimulationClockTick()
      UInt16 FullCapacity = 34000;
      m_sReading.TimeRemaining = m_sReading.Level/(tick_per_second*m_fDischarge);
      m_sReading.AvailCapacity = m_sReading.Level*FullCapacity;
//      std::cout<<" ABL: "<<m_sReading.Level<< std::endl;
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
                   "Carlo Pinciroli [ilpincy@gmail.com]",
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
                   "attribute 'level_noise_range'. You can add dicharge rate through the\n"
                   "attribute 'discharge'.\n"
                   "Attribute 'level_noise_range' regulates the noise range on the battery level\n"
                   "returned by the sensor. Attribute 'discharge' sets the rate at which battery\n"
                   "is discharged.\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <battery implementation=\"default\"\n"
                   "                 level_noise_range=\"-0.3:0.4\"\n"
                   "                 discharge_rate=\"0.01\" />\n"
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
