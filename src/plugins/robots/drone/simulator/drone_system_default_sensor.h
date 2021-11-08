/**
 * @file <argos3/plugins/robots/drone/simulator/drone_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DRONE_SYSTEM_DEFAULT_SENSOR_H
#define DRONE_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CDroneSystemDefaultSensor;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/drone/control_interface/ci_drone_system_sensor.h>

namespace argos {

   class CDroneSystemDefaultSensor : public CSimulatedSensor,
                                     public CCI_DroneSystemSensor {

   public:

      /**
       * @brief Constructor.
       */
      CDroneSystemDefaultSensor();

      /**
       * @brief Destructor.
       */
      virtual ~CDroneSystemDefaultSensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Update();

   };   
}

#endif
