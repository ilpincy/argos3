/**
 * @file <argos3/plugins/robots/kilobot/control_interface/ci_kilobot_controller.h>
 *
 * @brief This file provides the definition of the kilobot controller.
 *
 * This file provides the definition of the kilobot controller.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CCI_KILOBOT_CONTROLLER_H
#define CCI_KILOBOT_CONTROLLER_H

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/plugins/robots/kilobot/control_interface/kilolib.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
#include <argos3/plugins/robots/kilobot/control_interface/ci_kilobot_light_sensor.h>
#include <argos3/plugins/robots/kilobot/control_interface/ci_kilobot_communication_actuator.h>
#include <argos3/plugins/robots/kilobot/control_interface/ci_kilobot_communication_sensor.h>
#include <unistd.h>

using namespace argos;

class CCI_KilobotController : public CCI_Controller {

public:

   CCI_KilobotController();
   virtual ~CCI_KilobotController() {}

   virtual void Init(TConfigurationNode& t_tree);

   virtual void ControlStep();

   virtual void Reset();

   virtual void Destroy();

   int GetSharedMemFD() const {
      return m_nSharedMemFD;
   }

   pid_t GetBehaviorPID() const {
      return m_tBehaviorPID;
   }

private:

   /** Pointer to the motor actuator */
   CCI_DifferentialSteeringActuator* m_pcMotors;

   /** Pointer to LED actuator */
   CCI_LEDsActuator* m_pcLED;

   /** Pointer to the light sensor */
   CCI_KilobotLightSensor* m_pcLight;

   /** Pointer to the communication actuator */
   CCI_KilobotCommunicationActuator* m_pcCommA;

   /** Pointer to the communication sensor */
   CCI_KilobotCommunicationSensor* m_pcCommS;

   /* The random number generator */
   CRandom::CRNG* m_pcRNG;

   /** File descriptor for shared memory area */
   int m_nSharedMemFD;

   /** Pointer to the shared memory area */
   kilobot_state_t* m_ptRobotState;

   /** PID of the process executing the behavior */
   pid_t m_tBehaviorPID;

};

#endif
