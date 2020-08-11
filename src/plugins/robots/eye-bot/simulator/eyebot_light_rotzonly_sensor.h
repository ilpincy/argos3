/**
 * @file <argos3/plugins/robots/eye-bot/simulator/eyebot_light_rotzonly_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef EYEBOT_LIGHT_ROTZONLY_SENSOR_H
#define EYEBOT_LIGHT_ROTZONLY_SENSOR_H

#include <string>
#include <map>
#include <memory>

namespace argos {
   class CEyeBotLightRotZOnlySensor;
   class CLightSensorEquippedEntity;
}

#include <argos3/plugins/robots/eye-bot/control_interface/ci_eyebot_light_sensor.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/generic/simulator/noise_injector.h>

namespace argos {

   class CEyeBotLightRotZOnlySensor : public CSimulatedSensor,
                                      public CCI_EyeBotLightSensor {

   public:

      CEyeBotLightRotZOnlySensor();

      virtual ~CEyeBotLightRotZOnlySensor() {}

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

      /** Reference to embodied entity associated to this sensor */
      CEmbodiedEntity*               m_pcEmbodiedEntity;

      /** Reference to light sensor equipped entity associated to this sensor */
      CLightSensorEquippedEntity*    m_pcLightEntity;

      /** Reference to controllable entity associated to this sensor */
      CControllableEntity*           m_pcControllableEntity;

      /** Flag to show rays in the simulator */
      bool                           m_bShowRays;

      /** Reference to the space */
      CSpace&                        m_cSpace;

      /** Noise injector */
     std::unique_ptr<CNoiseInjector> m_pcNoiseInjector;
   };

}

#endif
