/**
 * @file <argos3/plugins/robots/generic/simulator/directional_leds_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DIRECTIONAL_LEDS_DEFAULT_ACTUATOR_H
#define DIRECTIONAL_LEDS_DEFAULT_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CDirectionalLEDsDefaultActuator;
   class CDirectionalLEDMedium;
}

#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
#include <argos3/plugins/simulator/entities/directional_led_equipped_entity.h>
#include <argos3/core/simulator/actuator.h>

namespace argos {

   class CDirectionalLEDsDefaultActuator : public CSimulatedActuator,
                                           public CCI_LEDsActuator {

   public:

      CDirectionalLEDsDefaultActuator();

      virtual ~CDirectionalLEDsDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Update();
      virtual void Reset();
      virtual void Destroy();

   private:

      CDirectionalLEDEquippedEntity* m_pcDirectionalLEDEquippedEntity;
      CDirectionalLEDMedium*         m_pcDirectionalLEDMedium;

   };

}

#endif
