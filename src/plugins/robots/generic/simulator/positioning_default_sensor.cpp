/**
 * @file <argos3/plugins/simulator/sensors/positioning_default_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>

#include "positioning_default_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   CPositioningDefaultSensor::CPositioningDefaultSensor() :
      m_pcEmbodiedEntity(NULL),
      m_pcRNG(NULL),
      m_bAddNoise(false) {}

   /****************************************/
   /****************************************/

   void CPositioningDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      m_pcEmbodiedEntity = &(c_entity.GetComponent<CEmbodiedEntity>("body"));
      m_sReading.Position = m_pcEmbodiedEntity->GetOriginAnchor().Position;
      m_sReading.Orientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation;
   }

   /****************************************/
   /****************************************/

   void CPositioningDefaultSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_PositioningSensor::Init(t_tree);
         /* Parse noise range */
         GetNodeAttributeOrDefault(t_tree, "pos_noise_range", m_cPosNoiseRange, m_cPosNoiseRange);
         GetNodeAttributeOrDefault(t_tree, "angle_noise_range", m_cAngleNoiseRange, m_cAngleNoiseRange);
         GetNodeAttributeOrDefault(t_tree, "axis_noise_range", m_cAxisNoiseRange, m_cAxisNoiseRange);
         if(m_cPosNoiseRange.GetSpan() != 0 ||
            m_cAngleNoiseRange.GetSpan() != CRadians::ZERO ||
            m_cAxisNoiseRange.GetSpan() != 0) {
            m_bAddNoise = true;
            m_pcRNG = CRandom::CreateRNG("argos");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in default positioning sensor", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   void CPositioningDefaultSensor::Update() {
      m_sReading.Position = m_pcEmbodiedEntity->GetOriginAnchor().Position;
      if(m_bAddNoise) {
         m_sReading.Position += CVector3(m_pcRNG->Uniform(m_cPosNoiseRange),
                                         m_pcRNG->Uniform(m_cPosNoiseRange),
                                         m_pcRNG->Uniform(m_cPosNoiseRange));
         m_pcEmbodiedEntity->GetOriginAnchor().Orientation.ToAngleAxis(m_cAngle, m_cAxis);
         m_cAngle += CRadians(m_pcRNG->Uniform(m_cAngleNoiseRange));
         m_cAxis += CVector3(m_pcRNG->Uniform(m_cAxisNoiseRange),
                             m_pcRNG->Uniform(m_cAxisNoiseRange),
                             m_pcRNG->Uniform(m_cAxisNoiseRange));
         m_sReading.Orientation.FromAngleAxis(m_cAngle, m_cAxis);
      }
      else {
         m_sReading.Orientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation;
      }
   }

   /****************************************/
   /****************************************/

   void CPositioningDefaultSensor::Reset() {
      m_sReading.Position = m_pcEmbodiedEntity->GetOriginAnchor().Position;
      m_sReading.Orientation = m_pcEmbodiedEntity->GetOriginAnchor().Orientation;
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CPositioningDefaultSensor,
                   "positioning", "default",
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
                   "        <positioning implementation=\"default\" />\n"
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
                   "        <positioning implementation=\"default\"\n"
                   "                     pos_noise_range=\"-0.1:0.2\"\n"
                   "                     angle_noise_range=\"-10.5:13.7\"\n"
                   "                     axis_noise_range=\"-0.3:0.4\" />\n"
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
