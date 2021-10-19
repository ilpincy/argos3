/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_electromagnet_system_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_ELECTROMAGNET_SYSTEM_DEFAULT_ACTUATOR_H
#define BUILDERBOT_ELECTROMAGNET_SYSTEM_DEFAULT_ACTUATOR_H

namespace argos {
   class CBuilderBotElectromagnetSystemDefaultActuator;
   class CBuilderBotElectromagnetSystemEntity;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_electromagnet_system_actuator.h>

namespace argos {

   class CBuilderBotElectromagnetSystemDefaultActuator : public CSimulatedActuator,
                                                         public CCI_BuilderBotElectromagnetSystemActuator {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotElectromagnetSystemDefaultActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotElectromagnetSystemDefaultActuator();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      CBuilderBotElectromagnetSystemEntity* m_pcElectromagnetSystemEntity;
   };
}

#endif
