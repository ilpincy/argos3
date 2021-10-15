/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_lift_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_LIFT_SYSTEM_DEFAULT_SENSOR_H
#define BUILDERBOT_LIFT_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotLiftSystemDefaultSensor;
   class CBuilderBotLiftSystemEntity;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_lift_system_sensor.h>

namespace argos {

   class CBuilderBotLiftSystemDefaultSensor : public CSimulatedSensor,
                                              public CCI_BuilderBotLiftSystemSensor {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotLiftSystemDefaultSensor();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotLiftSystemDefaultSensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:
      /*
      Real ConvertToMetersPerSecond(SInt16 n_raw) {
         static const Real fConversionFactor = 1.0;
         return (fConversionFactor * n_raw);
      }
      */

      CBuilderBotLiftSystemEntity* m_pcLiftSystemEntity;
   };
}

#endif
