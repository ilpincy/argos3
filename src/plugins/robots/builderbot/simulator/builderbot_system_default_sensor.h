/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_SYSTEM_DEFAULT_SENSOR_H
#define BUILDERBOT_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotSystemDefaultSensor;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_system_sensor.h>

namespace argos {

   class CBuilderBotSystemDefaultSensor : public CSimulatedSensor,
                                          public CCI_BuilderBotSystemSensor {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotSystemDefaultSensor();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotSystemDefaultSensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset();

      virtual void Update();

   };   
}

#endif
