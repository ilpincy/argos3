/**
 * @file <argos3/plugins/simulator/sensors/positioning_default_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef POSITIONING_DEFAULT_SENSOR_H
#define POSITIONING_DEFAULT_SENSOR_H

#include <string>
#include <map>
#include <memory>

namespace argos {
   class CPositioningDefaultSensor;
   class CEmbodiedEntity;
}

#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/generic/simulator/noise_injector.h>

namespace argos {

   class CPositioningDefaultSensor : public CSimulatedSensor,
                                     public CCI_PositioningSensor {

   public:

      CPositioningDefaultSensor();

      virtual ~CPositioningDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   protected:

      /** Reference to embodied entity associated to this sensor */
      CEmbodiedEntity*                m_pcEmbodiedEntity;

      /** Position noise injector */
      std::unique_ptr<CNoiseInjector> m_pcPosNoiseInjector;

      /** Angle noise injector */
      std::unique_ptr<CNoiseInjector> m_pcAngleNoiseInjector;

      /** Axis noise injector */
      std::unique_ptr<CNoiseInjector> m_pcAxisNoiseInjector;
   };

}

#endif
