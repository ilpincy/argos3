/**
 * @file <argos3/plugins/simulator/sensors/battery_default_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>

#include "battery_default_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   CBatteryDefaultSensor::CBatteryDefaultSensor() :
      m_pcEmbodiedEntity(NULL),
      m_pcRNG(NULL),
      m_bAddNoise(false) {}

   /****************************************/
   /****************************************/

   void CBatteryDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      m_sReading.Level = 1;  //todo
//      m_sReading.Orientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation;
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
         Real discharge=0;
         GetNodeAttributeOrDefault(t_tree, "discharge", discharge, discharge);
         m_fDischarge = discharge;
         if(m_cLevelNoiseRange.GetSpan() != 0 ) {
            m_bAddNoise = true;
            m_pcRNG = CRandom::CreateRNG("argos");
         }
         std::cout<<" Initial level "<<m_sReading.Level;
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in default battery sensor", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   void CBatteryDefaultSensor::Update() {
      m_sReading.Level -= m_fDischarge;//todo
      if(m_bAddNoise) {
         m_sReading.Level += m_pcRNG->Uniform(m_cLevelNoiseRange);
      }
      std::cout<<" BL: "<<m_sReading.Level<< std::endl;
      if(m_sReading.Level <= 0.0) {
          std::cout<<"Disabling..";
          m_pcEmbodiedEntity->Disable();
      }
   }

   /****************************************/
   /****************************************/

   void CBatteryDefaultSensor::Reset() {
      m_sReading.Level = 1;
//      m_sReading.Orientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CBatteryDefaultSensor,
                   "battery", "default",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "A generic positioning sensor.",
                   "This sensor returns the current position and orientation of a robot. This sensor\n"
                   "can be used with any robot, since it accesses only the body component. In\n"
                   "controllers, you must include the ci_positioning_sensor.h header.\n\n"
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
                   "attributes 'pos_noise_range', 'angle_noise_range', and 'axis_noise_range'.\n"
                   "Attribute 'pos_noise_range' regulates the noise range on the position returned\n"
                   "by the sensor. Attribute 'angle_noise_range' sets the noise range on the angle\n"
                   "(values expressed in degrees). Attribute 'axis_noise_range' sets the noise for\n"
                   "the rotation axis. Angle and axis are used to calculate a quaternion, which is\n"
                   "the actual returned value for rotation.\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <battery implementation=\"default\"\n"
                   "                 level_noise_range=\"-0.3:0.4\"\n"
                   "                 discharge=\"0.01\" />\n"
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
