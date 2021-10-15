/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_lift_system_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_LIFT_SYSTEM_DEFAULT_ACTUATOR_H
#define BUILDERBOT_LIFT_SYSTEM_DEFAULT_ACTUATOR_H

namespace argos {
   class CBuilderBotLiftSystemDefaultActuator;
   class CBuilderBotLiftSystemEntity;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_lift_system_actuator.h>

namespace argos {

   class CBuilderBotLiftSystemDefaultActuator : public CSimulatedActuator,
                                                public CCI_BuilderBotLiftSystemActuator {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotLiftSystemDefaultActuator();

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotLiftSystemDefaultActuator();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      Real ConvertToRaw(Real f_metres_per_second) {
         static const Real fConversionFactor = 1.0;
         return Real(f_metres_per_second * fConversionFactor);
      }

      CBuilderBotLiftSystemEntity* m_pcLiftSystemEntity;
   };
}

#endif
