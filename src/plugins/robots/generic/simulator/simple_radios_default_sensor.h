/**
 * @file <argos3/plugins/robots/generic/simulator/simple_radios_default_sensor.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef SIMPLE_RADIOS_DEFAULT_SENSOR_H
#define SIMPLE_RADIOS_DEFAULT_SENSOR_H

namespace argos {
   class CSimpleRadiosDefaultSensor;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_simple_radios_sensor.h>
#include <argos3/plugins/simulator/entities/simple_radio_equipped_entity.h>

namespace argos {

   class CSimpleRadiosDefaultSensor : public CSimulatedSensor,
                                      public CCI_SimpleRadiosSensor {

   public:

      CSimpleRadiosDefaultSensor();

      virtual ~CSimpleRadiosDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      /**
       * Returns true if the rays must be shown in the GUI.
       * @return true if the rays must be shown in the GUI.
       */
      inline bool IsShowRays() {
         return m_bShowRays;
      }

      /**
       * Sets whether or not the rays must be shown in the GUI.
       * @param b_show_rays true if the rays must be shown, false otherwise
       */
      inline void SetShowRays(bool b_show_rays) {
         m_bShowRays = b_show_rays;
      }

   protected:

      CSimpleRadioEquippedEntity* m_pcSimpleRadioEquippedEntity;
      CControllableEntity* m_pcControllableEntity;
      bool m_bShowRays;

   };
}

#endif
