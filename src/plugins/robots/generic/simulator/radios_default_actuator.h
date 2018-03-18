/*
 * @file <argos3/plugins/robots/generic/simulator/radios_default_actuator.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef RADIOS_DEFAULT_ACTUATOR_H
#define RADIOS_DEFAULT_ACTUATOR_H

namespace argos {
   class CRadiosDefaultActuator;
}

#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_radios_actuator.h>
#include <argos3/plugins/simulator/entities/radio_equipped_entity.h>

namespace argos {

   class CRadiosDefaultActuator : public CSimulatedActuator,
                                  public CCI_RadiosActuator {

   public:

      CRadiosDefaultActuator();

      virtual ~CRadiosDefaultActuator() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   protected:

      CRadioEquippedEntity* m_pcRadioEquippedEntity;

   };
}

#endif
