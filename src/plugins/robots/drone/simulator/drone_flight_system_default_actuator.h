/**
 * @file <argos3/plugins/robots/drone/simulator/drone_flight_system_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_FLIGHT_SYSTEM_DEFAULT_ACTUATOR_H
#define DRONE_FLIGHT_SYSTEM_DEFAULT_ACTUATOR_H

namespace argos {
   class CDroneFlightSystemDefaultActuator;
   class CDroneFlightSystemEntity;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/drone/control_interface/ci_drone_flight_system_actuator.h>

namespace argos {

   class CDroneFlightSystemDefaultActuator : public CSimulatedActuator,
                                             public CCI_DroneFlightSystemActuator {

   public:

      /**
       * @brief Constructor.
       */
      CDroneFlightSystemDefaultActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CDroneFlightSystemDefaultActuator();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual bool Ready() {
         return true;
      };

      virtual void Arm(bool b_arm, bool b_bypass_safety_checks) {}
      
      virtual void SetOffboardMode(bool b_offboard_mode) {}

   private:

      CDroneFlightSystemEntity* m_pcFlightSystemEntity;

   };
}

#endif
