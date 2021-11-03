/**
 * @file <argos3/plugins/robots/pipuck/simulator/pipuck_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef PIPUCK_SYSTEM_DEFAULT_SENSOR_H
#define PIPUCK_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CPiPuckSystemDefaultSensor;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/pi-puck/control_interface/ci_pipuck_system_sensor.h>

namespace argos {

   class CPiPuckSystemDefaultSensor : public CSimulatedSensor,
                                      public CCI_PiPuckSystemSensor {

   public:

      /**
       * @brief Constructor.
       */
      CPiPuckSystemDefaultSensor() {}

      /**
       * @brief Destructor.
       */
      virtual ~CPiPuckSystemDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Update();

   };   
}

#endif
