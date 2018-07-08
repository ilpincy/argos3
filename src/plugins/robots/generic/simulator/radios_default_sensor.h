/**
 * @file <argos3/plugins/robots/generic/simulator/radios_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef RADIOS_DEFAULT_SENSOR_H
#define RADIOS_DEFAULT_SENSOR_H

namespace argos {
   class CRadiosDefaultSensor;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_radios_sensor.h>
#include <argos3/plugins/simulator/entities/radio_equipped_entity.h>

namespace argos {

   class CRadiosDefaultSensor : public CSimulatedSensor,
                                public CCI_RadiosSensor {

   public:

      CRadiosDefaultSensor();

      virtual ~CRadiosDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   protected:

      CRadioEquippedEntity* m_pcRadioEquippedEntity;
      CControllableEntity* m_pcControllableEntity;
      bool m_bShowRays;

   };
}

#endif
