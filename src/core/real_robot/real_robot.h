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
      CRealRobot(const std::string& str_conf_fname,
                 const std::string& str_controller_id);
      
      /**
       * Class destructor.
       */
      virtual ~CRealRobot();

      /**
       * Put your robot initialization code here.
       */
      virtual void Init() = 0;

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
      virtual void Sense() = 0;
      
      /**
       * Execute the robot controller.
       */
      virtual void Control() = 0;
      
      /**
       * Send data to the actuators.
       */
      virtual void Act() = 0;

      /**
       * Perform the main loop.
       */
      virtual void Execute();

   protected:

      /**
       * Initializes the controller
       */
      virtual void InitController();

   protected:

      CCI_Controller* m_pcController;
      TConfigurationNode m_tConfRoot;
      TConfigurationNode* m_ptControllerConfRoot;
      Real m_fRate;
      
   };
   
}

#endif
