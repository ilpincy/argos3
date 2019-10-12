/**
 * @file <argos3/plugins/simulator/sensors/differential_steering_default_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/simulator/entities/wheeled_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>

#include "differential_steering_default_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   CDifferentialSteeringDefaultSensor::CDifferentialSteeringDefaultSensor() :
      m_pcWheeledEntity(NULL),
      m_pcRNG(NULL),
      m_bAddNoise(false) {}

   /****************************************/
   /****************************************/

   void CDifferentialSteeringDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      try {
         m_pcWheeledEntity = &(c_entity.GetComponent<CWheeledEntity>("wheels"));
         if(m_pcWheeledEntity->GetNumWheels() != 2) {
            THROW_ARGOSEXCEPTION("The differential steering sensor can be associated only to a robot with 2 wheels");
         }
         m_pfWheelVelocities = m_pcWheeledEntity->GetWheelVelocities();
         m_sReading.WheelAxisLength = 100.0f * Distance(m_pcWheeledEntity->GetWheelPosition(0),
                                                        m_pcWheeledEntity->GetWheelPosition(1));
         m_pcWheeledEntity->Enable();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error setting differential steering sensor to entity \"" << c_entity.GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDifferentialSteeringDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_DifferentialSteeringSensor::Init(t_tree);
         /* Parse noise range */
         GetNodeAttributeOrDefault(t_tree, "vel_noise_range",  m_cVelNoiseRange,  m_cVelNoiseRange);
         GetNodeAttributeOrDefault(t_tree, "dist_noise_range", m_cDistNoiseRange, m_cDistNoiseRange);
         if(m_cVelNoiseRange.GetSpan() != 0 ||
            m_cDistNoiseRange.GetSpan() != 0) {
            m_bAddNoise = true;
            m_pcRNG = CRandom::CreateRNG("argos");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in default differential steering sensor", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   void CDifferentialSteeringDefaultSensor::Update() {
      m_sReading.VelocityLeftWheel = m_pfWheelVelocities[0] * 100.0f;
      m_sReading.VelocityRightWheel = m_pfWheelVelocities[1] * 100.0f;
      m_sReading.CoveredDistanceLeftWheel = m_sReading.VelocityLeftWheel * CPhysicsEngine::GetSimulationClockTick();
      m_sReading.CoveredDistanceRightWheel = m_sReading.VelocityRightWheel * CPhysicsEngine::GetSimulationClockTick();
      if(m_bAddNoise) {
         m_sReading.VelocityLeftWheel  += m_pcRNG->Uniform(m_cVelNoiseRange);
         m_sReading.VelocityRightWheel += m_pcRNG->Uniform(m_cVelNoiseRange);
         m_sReading.CoveredDistanceLeftWheel  += m_pcRNG->Uniform(m_cDistNoiseRange);
         m_sReading.CoveredDistanceRightWheel += m_pcRNG->Uniform(m_cDistNoiseRange);
      }
   }

   /****************************************/
   /****************************************/

   void CDifferentialSteeringDefaultSensor::Reset() {
      m_sReading.VelocityLeftWheel         = 0.0f;
      m_sReading.VelocityRightWheel        = 0.0f;
      m_sReading.CoveredDistanceLeftWheel  = 0.0f;
      m_sReading.CoveredDistanceRightWheel = 0.0f;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CDifferentialSteeringDefaultSensor,
                   "differential_steering", "default",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "A generic differential steering sensor.",

                   "This sensor returns the current position and orientation of a robot. This sensor\n"
                   "can be used with any robot, since it accesses only the body component. In\n"
                   "controllers, you must include the ci_differential_steering_sensor.h header.\n\n"

                   "REQUIRED XML CONFIGURATION\n\n"

                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <differential_steering implementation=\"default\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"

                   "OPTIONAL XML CONFIGURATION\n\n"

                   "It is possible to add uniform noise to the sensor, thus matching the\n"
                   "characteristics of a real robot better. You can add noise through the\n"
                   "attributes 'vel_noise_range' and 'dist_noise_range'.\n"
                   "Attribute 'vel_noise_range' regulates the noise range on the velocity returned\n"
                   "by the sensor. Attribute 'dist_noise_range' sets the noise range on the\n"
                   "distance covered by the wheels.\n\n"

                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <differential_steering implementation=\"default\"\n"
                   "                               vel_noise_range=\"-0.1:0.2\"\n"
                   "                               dist_noise_range=\"-10.5:13.7\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n",

                   "Usable"
		  );

}
