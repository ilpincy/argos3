/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_electromagnet_system_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_ELECTROMAGNET_SYSTEM_DEFAULT_SENSOR_H
#define BUILDERBOT_ELECTROMAGNET_SYSTEM_DEFAULT_SENSOR_H

namespace argos {
   class CBuilderBotElectromagnetSystemDefaultSensor;
   class CBuilderBotElectromagnetSystemEntity;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_electromagnet_system_sensor.h>

namespace argos {

   class CBuilderBotElectromagnetSystemDefaultSensor : public CSimulatedSensor,
                                                       public CCI_BuilderBotElectromagnetSystemSensor {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotElectromagnetSystemDefaultSensor();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotElectromagnetSystemDefaultSensor();

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

      CBuilderBotElectromagnetSystemEntity* m_pcElectromagnetSystemEntity;
   };
}

#endif
