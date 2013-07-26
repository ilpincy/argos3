/**
 * @file <argos3/plugins/simulator/actuators/leds_default_actuator.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef LEDS_DEFAULT_ACTUATOR_H
#define LEDS_DEFAULT_ACTUATOR_H

#include <string>
#include <map>

namespace argos {
   class CLEDsDefaultActuator;
   class CLEDMedium;
}

#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/core/simulator/actuator.h>

namespace argos {

   class CLEDsDefaultActuator : public CSimulatedActuator,
                                public CCI_LEDsActuator {

   public:

      CLEDsDefaultActuator();

      virtual ~CLEDsDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Update();
      virtual void Reset();
      virtual void Destroy();

   private:

      CLEDEquippedEntity* m_pcLEDEquippedEntity;
      CLEDMedium*         m_pcLEDMedium;

   };

}

#endif
