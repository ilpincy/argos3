/**
 * @file <argos3/plugins/robots/builderbot/simulator/builderbot_leds_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef BUILDERBOT_LEDS_DEFAULT_ACTUATOR_H
#define BUILDERBOT_LEDS_DEFAULT_ACTUATOR_H

namespace argos {
   class CBuilderBotLEDsDefaultActuator;
   class CDirectionalLEDEntity;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/core/utility/datatypes/color.h>
#include <argos3/plugins/robots/builderbot/control_interface/ci_builderbot_leds_actuator.h>

#include <array>

namespace argos {

   class CBuilderBotLEDsDefaultActuator : public CSimulatedActuator,
                                          public CCI_BuilderBotLEDsActuator {

   public:

      /**
       * @brief Constructor.
       */
      CBuilderBotLEDsDefaultActuator() {}

      /**
       * @brief Destructor.
       */
      virtual ~CBuilderBotLEDsDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void SetLEDIndex(UInt32 un_index, const CColor& c_color);

      virtual void SetLEDs(const CColor& c_color);

   private:

      std::array<std::pair<CDirectionalLEDEntity*, CColor>, 
         CCI_BuilderBotLEDsActuator::NUM_LEDS> m_arrLEDs;

   };
}

#endif
