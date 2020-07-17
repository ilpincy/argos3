/**
 * @file <argos3/plugins/simulator/sensors/differential_steering_default_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DIFFERENTIAL_STEERING_DEFAULT_SENSOR_H
#define DIFFERENTIAL_STEERING_DEFAULT_SENSOR_H

#include <string>
#include <map>
#include <memory>

namespace argos {
   class CDifferentialSteeringDefaultSensor;
   class CWheeledEntity;
}

#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_sensor.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/generic/simulator/noise_injector.h>

namespace argos {

   class CDifferentialSteeringDefaultSensor : public CSimulatedSensor,
                                     public CCI_DifferentialSteeringSensor {

   public:

      CDifferentialSteeringDefaultSensor();

      virtual ~CDifferentialSteeringDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   protected:

      /** Reference to wheeled entity associated to this sensor */
      CWheeledEntity*                 m_pcWheeledEntity;

      /** The wheel velocity taken from the wheeled entity */
      const Real*                     m_pfWheelVelocities;

      /** Distance noise injector */
      std::unique_ptr<CNoiseInjector> m_pcDistNoiseInjector;

      /** Velocity noise injector */
      std::unique_ptr<CNoiseInjector> m_pcVelNoiseInjector;
   };

}

#endif
