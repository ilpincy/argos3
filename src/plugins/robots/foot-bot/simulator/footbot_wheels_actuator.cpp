/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_wheels_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "footbot_wheels_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/plugins/factory.h>

namespace argos {

   /****************************************/
   /****************************************/

   CFootBotWheelsActuator::CFootBotWheelsActuator() :
      m_pcWheeledEntity(NULL),
      m_pcRNG(NULL),
      m_fNoiseStdDeviation(0.0f) {
      m_fCurrentVelocity[FOOTBOT_LEFT_WHEEL] = 0.0f;
      m_fCurrentVelocity[FOOTBOT_RIGHT_WHEEL] = 0.0f;
   }
   
   /****************************************/
   /****************************************/
   
   void CFootBotWheelsActuator::SetEntity(CEntity& c_entity) {
      CSimulatedActuator<CFootBotEntity>::SetEntity(c_entity);
      m_pcWheeledEntity = &(GetEntity().GetWheeledEntity());
   }

   /****************************************/
   /****************************************/

   void CFootBotWheelsActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_FootBotWheelsActuator::Init(t_tree);
         GetNodeAttributeOrDefault<Real>(t_tree, "noise_std_dev", m_fNoiseStdDeviation, 0.0f);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in foot-bot wheels actuator.", ex);
      }
      if(m_fNoiseStdDeviation > 0.0f) {
         m_pcRNG = CRandom::CreateRNG("argos");
      }
   }

   /****************************************/
   /****************************************/
   
   void CFootBotWheelsActuator::SetLinearVelocity(Real f_left_velocity,
                                                  Real f_right_velocity) {
      CCI_FootBotWheelsActuator::FOOTBOT_WHEELS_SPEED_RANGE.TruncValue(f_left_velocity);
      CCI_FootBotWheelsActuator::FOOTBOT_WHEELS_SPEED_RANGE.TruncValue(f_right_velocity);

      /* Convert speeds in m/s */
      m_fCurrentVelocity[FOOTBOT_LEFT_WHEEL] = f_left_velocity * 0.01f;
      m_fCurrentVelocity[FOOTBOT_RIGHT_WHEEL] = f_right_velocity * 0.01f;
      if(m_fNoiseStdDeviation > 0.0f) {
         AddGaussianNoise();
      }
   }
   
   /****************************************/
   /****************************************/
   
   void CFootBotWheelsActuator::Update() {
      m_pcWheeledEntity->SetSpeed(m_fCurrentVelocity);
   }

   /****************************************/
   /****************************************/
   
   void CFootBotWheelsActuator::Reset() {
      m_fCurrentVelocity[FOOTBOT_LEFT_WHEEL]  = 0.0f;
      m_fCurrentVelocity[FOOTBOT_RIGHT_WHEEL] = 0.0f;
   }
   
   /****************************************/
   /****************************************/
   
   void CFootBotWheelsActuator::AddGaussianNoise() {
      m_fCurrentVelocity[FOOTBOT_LEFT_WHEEL]  += m_fCurrentVelocity[FOOTBOT_LEFT_WHEEL] * m_pcRNG->Gaussian(m_fNoiseStdDeviation);
      m_fCurrentVelocity[FOOTBOT_RIGHT_WHEEL] += m_fCurrentVelocity[FOOTBOT_RIGHT_WHEEL] * m_pcRNG->Gaussian(m_fNoiseStdDeviation);
   }

   /****************************************/
   /****************************************/

}

REGISTER_ACTUATOR(CFootBotWheelsActuator,
                  "footbot_wheels", "default",
                  "Carlo Pinciroli [ilpincy@gmail.com]",
                  "1.0",
                  "The foot-bot wheels actuator.",
                  "This actuator controls the foot-bot wheels. For a complete description of its\n"
                  "usage, refer to the the ci_footbot_wheels_actuator.h file.\n\n"
                  "REQUIRED XML CONFIGURATION\n\n"
                  "  <controllers>\n"
                  "    ...\n"
                  "    <my_controller ...>\n"
                  "      ...\n"
                  "      <actuators>\n"
                  "        ...\n"
                  "        <footbot_wheels implementation=\"default\" />\n"
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
                  "desired velocity of the wheels:\n\n"
                  "  <controllers>\n"
                  "    ...\n"
                  "    <my_controller ...>\n"
                  "      ...\n"
                  "      <actuators>\n"
                  "        ...\n"
                  "        <footbot_wheels implementation=\"default\"\n"
                  "                        noise_std_dev=\"1\" />\n"
                  "        ...\n"
                  "      </actuators>\n"
                  "      ...\n"
                  "    </my_controller>\n"
                  "    ...\n"
                  "  </controllers>\n",
                  "Usable"
   );
   
