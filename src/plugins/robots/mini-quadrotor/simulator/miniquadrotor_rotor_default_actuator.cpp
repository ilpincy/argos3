/**
 * @file <argos3/plugins/robots/mini-quadrotor/simulator/miniquadrotor_rotor_default_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "miniquadrotor_rotor_default_actuator.h"
#include <argos3/plugins/simulator/entities/rotor_equipped_entity.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/plugins/factory.h>

namespace argos {

   /****************************************/
   /****************************************/

   CMiniQuadrotorRotorDefaultActuator::CMiniQuadrotorRotorDefaultActuator() :
      m_pcRotorEquippedEntity(NULL),
      m_pcRNG(NULL),
      m_fNoiseStdDeviation(0.0f) {
   }
   
   /****************************************/
   /****************************************/
   
   void CMiniQuadrotorRotorDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      try {
         m_pcRotorEquippedEntity = &(c_entity.GetComponent<CRotorEquippedEntity>("rotors"));
         if(m_pcRotorEquippedEntity->GetNumRotors() != 4) {
            THROW_ARGOSEXCEPTION("The mini-quadrotor rotor actuator can be associated only to a mini-quadrotor");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error setting mini-quadrotor rotor actuator to entity \"" << c_entity.GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CMiniQuadrotorRotorDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_MiniQuadrotorRotorActuator::Init(t_tree);
         GetNodeAttributeOrDefault<Real>(t_tree, "noise_std_dev", m_fNoiseStdDeviation, 0.0f);
         if(m_fNoiseStdDeviation > 0.0f) {
            m_pcRNG = CRandom::CreateRNG("argos");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in mini-quadrotor rotor actuator.", ex);
      }
   }

   /****************************************/
   /****************************************/
   
   void CMiniQuadrotorRotorDefaultActuator::SetRotorVelocities(const CCI_MiniQuadrotorRotorActuator::SVelocities& s_velocities) {
      /* Set velocities */
      m_sCurrentVelocities = s_velocities;
      /* Apply noise */
      if(m_fNoiseStdDeviation > 0.0f) {
         AddGaussianNoise();
      }
   }
   
   /****************************************/
   /****************************************/
   
   void CMiniQuadrotorRotorDefaultActuator::Update() {
      m_pcRotorEquippedEntity->SetVelocities(m_sCurrentVelocities.Velocities);
   }

   /****************************************/
   /****************************************/
   
   void CMiniQuadrotorRotorDefaultActuator::Reset() {
      m_sCurrentVelocities = SVelocities();
   }
   
   /****************************************/
   /****************************************/
   
   void CMiniQuadrotorRotorDefaultActuator::AddGaussianNoise() {
      m_sCurrentVelocities.Velocities[0] += m_sCurrentVelocities.Velocities[0] * m_pcRNG->Gaussian(m_fNoiseStdDeviation);
      m_sCurrentVelocities.Velocities[1] += m_sCurrentVelocities.Velocities[1] * m_pcRNG->Gaussian(m_fNoiseStdDeviation);
      m_sCurrentVelocities.Velocities[2] += m_sCurrentVelocities.Velocities[2] * m_pcRNG->Gaussian(m_fNoiseStdDeviation);
      m_sCurrentVelocities.Velocities[3] += m_sCurrentVelocities.Velocities[3] * m_pcRNG->Gaussian(m_fNoiseStdDeviation);
   }

   /****************************************/
   /****************************************/

}

REGISTER_ACTUATOR(CMiniQuadrotorRotorDefaultActuator,
                  "miniquadrotor_rotor", "default",
                  "Carlo Pinciroli [ilpincy@gmail.com]",
                  "1.0",
                  "The mini-quadrotor rotor actuator.",
                  "This actuator controls the four rotors of a mini-quadrotor robot. For a\n"
                  "complete description of its usage, refer to the\n"
                  "ci_miniquadrotor_rotor_actuator.h file.\n\n"
                  "REQUIRED XML CONFIGURATION\n\n"
                  "  <controllers>\n"
                  "    ...\n"
                  "    <my_controller ...>\n"
                  "      ...\n"
                  "      <actuators>\n"
                  "        ...\n"
                  "        <miniquadrotor_rotor implementation=\"default\" />\n"
                  "        ...\n"
                  "      </actuators>\n"
                  "      ...\n"
                  "    </my_controller>\n"
                  "    ...\n"
                  "  </controllers>\n\n"
                  "OPTIONAL XML CONFIGURATION\n\n"
                  "It is possible to specify noisy speed in order to match the characteristics\n"
                  "of the real robot. This can be done with the attribute: 'noise_std_dev',\n" 
                  "which indicates the standard deviation of a gaussian noise applied to the\n"
                  "desired velocity of the rotor:\n\n"
                  "  <controllers>\n"
                  "    ...\n"
                  "    <my_controller ...>\n"
                  "      ...\n"
                  "      <actuators>\n"
                  "        ...\n"
                  "        <miniquadrotor_rotor implementation=\"default\"\n"
                  "                               noise_std_dev=\"1\" />\n"
                  "        ...\n"
                  "      </actuators>\n"
                  "      ...\n"
                  "    </my_controller>\n"
                  "    ...\n"
                  "  </controllers>\n",
                  "Usable"
   );
   
