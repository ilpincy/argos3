/**
 * @file <argos3/plugins/robots/drone/simulator/drone_flight_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_FLIGHT_SYSTEM_DEFAULT_SENSOR_H
#define DRONE_FLIGHT_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CDroneFlightSystemDefaultSensor;
   class CDroneFlightSystemEntity;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/drone/control_interface/ci_drone_flight_system_sensor.h>

namespace argos {

   class CDroneFlightSystemDefaultSensor : public CSimulatedSensor,
                                           public CCI_DroneFlightSystemSensor {

   public:

      /**
       * @brief Constructor.
       */
      CDroneFlightSystemDefaultSensor();

      /**
       * @brief Destructor.
       */
      virtual ~CDroneFlightSystemDefaultSensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

   private:

      CDroneFlightSystemEntity* m_pcFlightSystemEntity;

   };
}

#endif
