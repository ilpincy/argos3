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
      m_pcWheeledEntity(NULL) {}

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
         /* Parse noise injection */
         if(NodeExists(t_tree, "vel_noise")) {
           TConfigurationNode& tNode = GetNode(t_tree, "vel_noise");
           m_cVelNoiseInjector.Init(tNode);
         }
         if(NodeExists(t_tree, "dist_noise")) {
           TConfigurationNode& tNode = GetNode(t_tree, "dist_noise");
           m_cDistNoiseInjector.Init(tNode);
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
      if (m_cDistNoiseInjector.Enabled()) {
        m_sReading.CoveredDistanceLeftWheel  += m_cDistNoiseInjector.InjectNoise();
        m_sReading.CoveredDistanceRightWheel += m_cDistNoiseInjector.InjectNoise();
      }
      if (m_cVelNoiseInjector.Enabled()) {
        m_sReading.VelocityLeftWheel  += m_cVelNoiseInjector.InjectNoise();
        m_sReading.VelocityRightWheel += m_cVelNoiseInjector.InjectNoise();
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

                   "----------------------------------------\n"
                   "Noise Injection\n"
                   "----------------------------------------\n"

                   "It is possible to one or more of the following NOISE_TYPE child tags\n"
                   "to differential steering sensor configuration (all noise types are independent):\n"
                   "['dist_noise', 'vel_noise']. "

                   "Tag 'dist_noise' controls the noise applied to the reading of distance\n"
                   "covered by the wheels. Tag 'vel_noise' controls the noise applied to the\n"
                   "sensor velocity reading.\n\n" +

                   CNoiseInjector::GetQueryDocumentation({
                       .strDocName = "differential steering sensor",
                           .strXMLParent = "differential_steering",
                           .strXMLTag = "NOISE_TYPE",
                           .strSAAType = "sensor",
                           .bShowExamples = true}),

                   "Usable"
		  );

}
