#ifndef REAL_ROBOT_H
#define REAL_ROBOT_H

#include <argos3/core/utility/configuration/argos_configuration.h>
#include <argos3/core/control_interface/ci_controller.h>

namespace argos {

   class CRealRobot {

   public:

      /**
       * Class constructor.
       */
      CRealRobot();
      
      /**
       * Class destructor.
       */
      virtual ~CRealRobot();

      /**
       * Initializes the robot and the controller.
       * Internally calls InitRobot() and InitController() in this order.
       */
      virtual void Init(const std::string& str_conf_fname,
                        const std::string& str_controller_id);

      /**
       * Put your robot initialization code here.
       */
      virtual void InitRobot() = 0;

      /**
       * Put your robot cleanup code here.
       */
      virtual void Destroy() = 0;

      /**
       * Creates an actuator given its name.
       * Returns NULL if no actuator corresponds to that name.
       */
      virtual CCI_Actuator* MakeActuator(const std::string& str_name) = 0;

      /**
       * Creates a sensor given its name.
       * Returns NULL if no sensor corresponds to that name.
       */
      virtual CCI_Sensor* MakeSensor(const std::string& str_name) = 0;

      /**
       * Collect data from the sensors.
       */
      virtual void Sense(Real f_elapsed_time) = 0;
      
      /**
       * Execute the robot controller.
       */
      virtual void Control();
      
      /**
       * Send data to the actuators.
       */
      virtual void Act(Real f_elapsed_time) = 0;

      /**
       * Performs the main loop.
       */
      virtual void Execute();

      /**
       * Cleanup function called when the controller is stopped.
       */
      static void Cleanup(int);

   protected:

      CCI_Controller* m_pcController;
      ticpp::Document m_tConfiguration;
      TConfigurationNode m_tConfRoot;
      TConfigurationNode* m_ptControllerConfRoot;
      Real m_fRate;
      static CRealRobot* m_pcInstance;
      
   };
   
}

#endif
