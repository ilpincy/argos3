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
      m_pcWheeledEntity(NULL),
      m_pcRNG(NULL) {
      m_fCurrentVelocity[LEFT_WHEEL] = 0.0;
      m_fCurrentVelocity[RIGHT_WHEEL] = 0.0;
      m_fNoiseBias[LEFT_WHEEL] = 1.0;
      m_fNoiseBias[RIGHT_WHEEL] = 1.0;
      m_fNoiseFactorAvg[LEFT_WHEEL] = 1.0;
      m_fNoiseFactorAvg[RIGHT_WHEEL] = 1.0;
      m_fNoiseFactorStdDev[LEFT_WHEEL] = 0.0;
      m_fNoiseFactorStdDev[RIGHT_WHEEL] = 0.0;
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

#define CHECK_ATTRIBUTE(ATTR)                     \
   (NodeAttributeExists(t_tree, ATTR)          || \
    NodeAttributeExists(t_tree, ATTR "_left")  || \
    NodeAttributeExists(t_tree, ATTR "_right"))

#define PARSE_ATTRIBUTES(ATTR, VAR)                                     \
   GetNodeAttributeOrDefault<Real>(t_tree, ATTR, VAR[LEFT_WHEEL], VAR[LEFT_WHEEL]); \
   VAR[RIGHT_WHEEL] = VAR[LEFT_WHEEL];                                  \
   GetNodeAttributeOrDefault<Real>(t_tree, ATTR "_left", VAR[LEFT_WHEEL], VAR[LEFT_WHEEL]); \
   GetNodeAttributeOrDefault<Real>(t_tree, ATTR "_right", VAR[RIGHT_WHEEL], VAR[RIGHT_WHEEL]);

#define PICK_BIAS(LRW) m_fNoiseBias[LRW ## _WHEEL] = m_pcRNG->Gaussian(fNoiseBiasStdDev[LRW ## _WHEEL], fNoiseBiasAvg[LRW ## _WHEEL])
   
   void CDifferentialSteeringDefaultActuator::Init(TConfigurationNode& t_tree) {
      try {
         CCI_DifferentialSteeringActuator::Init(t_tree);
         /* Check if any noise attribute was specified */
         bool bNoise =
            CHECK_ATTRIBUTE("bias_avg")    ||
            CHECK_ATTRIBUTE("bias_stddev") ||
            CHECK_ATTRIBUTE("factor_avg")  ||
            CHECK_ATTRIBUTE("factor_stddev");
         /* Handle noise attributes, if any */
         if(bNoise) {
            /* Create RNG */
            m_pcRNG = CRandom::CreateRNG("argos");
            /* Parse noise attributes */
            Real fNoiseBiasAvg[2];
            Real fNoiseBiasStdDev[2];
            PARSE_ATTRIBUTES("bias_avg", fNoiseBiasAvg);
            PARSE_ATTRIBUTES("bias_stddev", fNoiseBiasStdDev);
            PARSE_ATTRIBUTES("factor_avg", m_fNoiseFactorAvg);
            PARSE_ATTRIBUTES("factor_stddev", m_fNoiseFactorStdDev);
            /* Choose robot bias */
            PICK_BIAS(LEFT);
            PICK_BIAS(RIGHT);
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in foot-bot steering actuator.", ex);
      }
   }

   /****************************************/
   /****************************************/

#define ADD_GAUSSIAN(LRW)                                  \
   (m_fNoiseFactorStdDev[LRW ## _WHEEL] > 0.0 ?            \
    m_pcRNG->Gaussian(m_fNoiseFactorStdDev[LRW ## _WHEEL], \
                      m_fNoiseFactorAvg[LRW ## _WHEEL]) :  \
    m_fNoiseFactorAvg[LRW ## _WHEEL])

#define ADD_NOISE(LRW)                                     \
   m_fCurrentVelocity[LRW ## _WHEEL] =                     \
      ADD_GAUSSIAN(LRW)                                    \
      *                                                    \
      (m_fCurrentVelocity[LRW ## _WHEEL] +                 \
       m_fNoiseBias[LRW ## _WHEEL]);
   
   void CDifferentialSteeringDefaultActuator::SetLinearVelocity(Real f_left_velocity,
                                                                Real f_right_velocity) {
      /* Convert speeds in m/s */
      m_fCurrentVelocity[LEFT_WHEEL] = f_left_velocity * 0.01;
      m_fCurrentVelocity[RIGHT_WHEEL] = f_right_velocity * 0.01;
      /* Apply noise only if the robot is in motion (at least one of the wheels is moving)*/
      if( m_pcRNG &&
          ((f_left_velocity  != 0) ||
           (f_right_velocity != 0) )) {
         ADD_NOISE(LEFT);
         ADD_NOISE(RIGHT);
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
      /*
       * Throw away two RNG Gaussian calls to make sure the RNG sequence is the same after resetting
       * These two calls were used to pick the bias in Init()
       */
      if(m_pcRNG) {
         m_pcRNG->Gaussian(1.0, 0.0);
         m_pcRNG->Gaussian(1.0, 0.0);
      }
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

                  "It is possible to specify noisy speed in order to match the characteristics\n"
                  "of the real robot. For each wheel, the noise model is as follows:\n\n"
                  "w = ideal wheel actuation (as set in the controller)\n"
                  "b = random bias from a Gaussian distribution\n"
                  "f = random factor from a Gaussian distribution\n"
                  "a = actual actuated value\n\n"
                  "a = f * (w + b)\n\n"
                  "You can configure the average and stddev of both the bias and the factor. This\n"
                  "can be done with the optional attributes: 'bias_avg', 'bias_stddev',\n"
                  "'factor_avg', and 'factor_stddev'. Bias attributes are expressed in m/s, while\n"
                  "factor attributes are dimensionless. If none of these attributed is specified,\n"
                  "no noise is added. If at least one of these attributed is specified, noise is\n"
                  "added and, for the non-specified attributes, the default value of 1 is used for\n"
                  "the '*_avg' attributes, while 0 is used for '*_stddev' attributes. Examples:\n\n"

                  "  <controllers>\n"
                  "    ...\n"
                  "    <my_controller ...>\n"
                  "      ...\n"
                  "      <actuators>\n"
                  "        ...\n"
                  "        <!-- Only the stddev of the bias\n"
                  "             Noise is on, other attributes are default -->\n"
                  "        <differential_steering implementation=\"default\"\n"
                  "                               bias_stddev=\"2\" />\n"
                  "        <!-- Only the stddev of the factor\n"
                  "             Noise is on, other attributes are default -->\n"
                  "        <differential_steering implementation=\"default\"\n"
                  "                               factor_stddev=\"4\" />\n"
                  "        <!-- All attributes set\n"
                  "             Noise is on, specified values are set -->\n"
                  "        <differential_steering implementation=\"default\"\n"
                  "                               bias_avg=\"1\"\n"
                  "                               bias_stddev=\"2\"\n"
                  "                               factor_avg=\"3\"\n"
                  "                               factor_stddev=\"4\" />\n"
                  "        ...\n"
                  "      </actuators>\n"
                  "      ...\n"
                  "    </my_controller>\n"
                  "    ...\n"
                  "  </controllers>\n\n"

                  "The above examples set the same noise for both wheels. If you want to set\n"
                  "different noise parameters for each wheel, append '_left' and '_right' to the\n"
                  "attribute names:\n\n"

                  "  <controllers>\n"
                  "    ...\n"
                  "    <my_controller ...>\n"
                  "      ...\n"
                  "      <actuators>\n"
                  "        ...\n"
                  "        <!-- Mix of wheel-specific attributes set\n"
                  "             Noise is on, specified values are set -->\n"
                  "        <differential_steering implementation=\"default\"\n"
                  "                               bias_avg_left=\"1\"\n"
                  "                               bias_stddev_right=\"2\"\n"
                  "                               factor_avg_left=\"3\"\n"
                  "                               factor_stddev_right=\"4\" />\n"
                  "        ...\n"
                  "      </actuators>\n"
                  "      ...\n"
                  "    </my_controller>\n"
                  "    ...\n"
                  "  </controllers>\n\n"

                  "Wheel-specific attributes overwrite the values of non-wheel specific attributes.\n"
                  "So, if you set 'bias_avg' = 2 and then 'bias_avg_left' = 3, the left wheel will\n"
                  "use 3 and the right wheel will use 2.\n\n"
                  "Physics-engine-specific attributes that affect this actuator might also be\n"
                  "available. Check the documentation of the physics engine you're using for more\n"
                  "information.",
                  "Usable"
   );
