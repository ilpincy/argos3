/**
 * @file <argos3/plugins/simulator/actuators/differential_steering_default_actuator.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "differential_steering_default_actuator.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/plugins/factory.h>

namespace argos {

   /****************************************/
   /****************************************/

   CDifferentialSteeringDefaultActuator::CDifferentialSteeringDefaultActuator() :
      m_pcWheeledEntity(NULL) {
      m_fCurrentVelocity[LEFT_WHEEL] = 0.0;
      m_fCurrentVelocity[RIGHT_WHEEL] = 0.0;
      m_fNoiseBias[LEFT_WHEEL] = 0.0;
      m_fNoiseBias[RIGHT_WHEEL] = 0.0;
   }

   /****************************************/
   /****************************************/

   void CDifferentialSteeringDefaultActuator::SetRobot(CComposableEntity& c_entity) {
      try {
         m_pcWheeledEntity = &(c_entity.GetComponent<CWheeledEntity>("wheels"));
         if(m_pcWheeledEntity->GetNumWheels() != 2) {
            THROW_ARGOSEXCEPTION("The differential steering actuator can be associated only to a robot with 2 wheels");
         }
         m_pcWheeledEntity->Enable();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error setting differential steering actuator to entity \"" << c_entity.GetId() << "\"", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDifferentialSteeringDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_DifferentialSteeringActuator::Init(t_tree);
         ParseNoiseInjection(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in foot-bot steering actuator.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDifferentialSteeringDefaultActuator::ParseNoiseInjection(
       TConfigurationNode& t_tree) {
     CNoiseInjector cBiasGenerator;
     if(NodeExists(t_tree, "noise_bias")) {
       TConfigurationNode& tNode = GetNode(t_tree, "noise_bias");
       cBiasGenerator.Init(tNode);
       m_fNoiseBias[LEFT_WHEEL] = cBiasGenerator.InjectNoise();
       m_fNoiseBias[RIGHT_WHEEL] = cBiasGenerator.InjectNoise();
     } else {
       if(NodeExists(t_tree, "noise_bias_left")) {
         TConfigurationNode& tNode = GetNode(t_tree, "noise_bias_left");
         cBiasGenerator.Init(tNode);
         m_fNoiseBias[LEFT_WHEEL] = cBiasGenerator.InjectNoise();
       }
       if(NodeExists(t_tree, "noise_bias_right")) {
         TConfigurationNode& tNode = GetNode(t_tree, "noise_bias_right");
         cBiasGenerator.Init(tNode);
         m_fNoiseBias[RIGHT_WHEEL] = cBiasGenerator.InjectNoise();
       }
     }
     if(NodeExists(t_tree, "noise_factor")) {
       TConfigurationNode& tNode = GetNode(t_tree, "noise_factor");
       m_cNoiseFactor[LEFT_WHEEL] = std::make_unique<CNoiseInjector>();
       m_cNoiseFactor[LEFT_WHEEL]->Init(tNode);
       m_cNoiseFactor[RIGHT_WHEEL] = std::make_unique<CNoiseInjector>();
       m_cNoiseFactor[RIGHT_WHEEL]->Init(tNode);
     } else {
       if(NodeExists(t_tree, "noise_factor_left")) {
         TConfigurationNode& tNode = GetNode(t_tree, "noise_factor_left");
         m_cNoiseFactor[LEFT_WHEEL] = std::make_unique<CNoiseInjector>();
         m_cNoiseFactor[LEFT_WHEEL]->Init(tNode);
       }
       if(NodeExists(t_tree, "noise_factor_right")) {
         TConfigurationNode& tNode = GetNode(t_tree, "noise_factor_right");
         m_cNoiseFactor[RIGHT_WHEEL] = std::make_unique<CNoiseInjector>();
         m_cNoiseFactor[RIGHT_WHEEL]->Init(tNode);
       }
     }
   } /* ParseNoiseInjection() */

   /****************************************/
   /****************************************/

   void CDifferentialSteeringDefaultActuator::SetLinearVelocity(Real f_left_velocity,
                                                                Real f_right_velocity) {
      /* Convert speeds in m/s */
      m_fCurrentVelocity[LEFT_WHEEL] = f_left_velocity * 0.01;
      m_fCurrentVelocity[RIGHT_WHEEL] = f_right_velocity * 0.01;

      bool bApplyNoise = (m_fNoiseBias[LEFT_WHEEL] > 0.0 ||
                          m_fNoiseBias[RIGHT_WHEEL] > 0.0  ||
                          (m_cNoiseFactor[LEFT_WHEEL]) ||
                          (m_cNoiseFactor[RIGHT_WHEEL]));

      /*
       * Apply noise only if the robot is in motion (at least one of the wheels
       * is moving).
       */
      if(((f_left_velocity != 0) || (f_right_velocity != 0)) && bApplyNoise) {
        Real fNoiseFactorLeft = 0.0;
        Real fNoiseFactorRight = 0.0;
        if (m_cNoiseFactor[LEFT_WHEEL]) {
          fNoiseFactorLeft = m_cNoiseFactor[LEFT_WHEEL]->InjectNoise();
        }
        if (m_cNoiseFactor[RIGHT_WHEEL]) {
          fNoiseFactorLeft = m_cNoiseFactor[RIGHT_WHEEL]->InjectNoise();
        }
        m_fCurrentVelocity[LEFT_WHEEL] = fNoiseFactorLeft *
                                         (m_fCurrentVelocity[LEFT_WHEEL] + m_fNoiseBias[LEFT_WHEEL]);
        m_fCurrentVelocity[RIGHT_WHEEL] = fNoiseFactorRight *
            (m_fCurrentVelocity[RIGHT_WHEEL] + m_fNoiseBias[RIGHT_WHEEL]);

      }
   }

   /****************************************/
   /****************************************/

   void CDifferentialSteeringDefaultActuator::Update() {
      m_pcWheeledEntity->SetVelocities(m_fCurrentVelocity);
   }

   /****************************************/
   /****************************************/

   void CDifferentialSteeringDefaultActuator::Reset() {
      /* Zero the speeds */
      m_fCurrentVelocity[LEFT_WHEEL]  = 0.0;
      m_fCurrentVelocity[RIGHT_WHEEL] = 0.0;
   }

   /****************************************/
   /****************************************/

}

REGISTER_ACTUATOR(CDifferentialSteeringDefaultActuator,
                  "differential_steering", "default",
                  "Carlo Pinciroli [ilpincy@gmail.com]",
                  "1.0",
                  "The differential steering actuator.",

                  "This actuator controls the two wheels a differential steering robot. For a\n"
                  "complete description of its usage, refer to the\n"
                  "ci_differential_steering_actuator.h file.\n\n"

                  "REQUIRED XML CONFIGURATION\n\n"
                  "  <controllers>\n"
                  "    ...\n"
                  "    <my_controller ...>\n"
                  "      ...\n"
                  "      <actuators>\n"
                  "        ...\n"
                  "        <differential_steering implementation=\"default\" />\n"
                  "        ...\n"
                  "      </actuators>\n"
                  "      ...\n"
                  "    </my_controller>\n"
                  "    ...\n"
                  "  </controllers>\n\n"

                  "OPTIONAL XML CONFIGURATION\n\n"

                  "----------------------------------------\n"
                  "Noise Injection\n"
                  "----------------------------------------\n"

                  "It is possible to specify noisy speed in order to match the characteristics\n"
                  "of the real robot. For each wheel, the noise model is as follows:\n\n"
                  "w = ideal wheel actuation (as set in the controller)\n"
                  "b = random bias from a specified bdistribution\n"
                  "f = random factor from a specified distribution\n"
                  "a = actual actuated value\n\n"
                  "a = f * (w + b)\n\n"
                  "You can configure the distribution used to calculate both bias and factor. This\n"
                  "can be done with the optional NOISE_TYPE tags: ['noise_bias', 'noise_factor'].\n"
                  "'noise_bias' is expressed in m/s, while the 'noise_factor' is dimensionless.\n"
                  "If you want to set different noise parameters for each wheel, append '_left' \n"
                  "and '_right' to the NOISE_TYPE tags: ['noise_bias_left'/'noise_factor_left',\n"
                  "'noise_bias_right'/'noise_factor_right']. If at least one of these NOISE_TYPE\n"
                  "tags is specified, noise is injected.\n\n" +
                  CNoiseInjector::GetQueryDocumentation({
                       .strDocName = "differential steering",
                           .strXMLParent = "differential_steering",
                           .strXMLTag = "NOISE_TYPE",
                           .strSAAType = "actuator",
                           .bShowExamples = true}) +
                  "You cannot mix wheel-specific noise injection specifications and non-wheel\n"
                  "noise injection specifications. The bias is calculated during initialization\n"
                  "and remains the same throughout simulation, while the factor is re-calculated\n"
                  "each timestep that noise is injected.\n\n"

                  "Physics-engine-specific attributes that affect this actuator might also be\n"
                  "available. Check the documentation of the physics engine you're using for more\n"
                  "information.",
                  "Usable"
   );
