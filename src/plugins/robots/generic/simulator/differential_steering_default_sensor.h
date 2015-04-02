/**
 * @file <argos3/plugins/simulator/sensors/differential_steering_default_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DIFFERENTIAL_STEERING_DEFAULT_SENSOR_H
#define DIFFERENTIAL_STEERING_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CDifferentialSteeringDefaultSensor;
   class CWheeledEntity;
}

#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

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
      CWheeledEntity* m_pcWheeledEntity;

      /** The wheel velocity taken from the wheeled entity */
      const Real* m_pfWheelVelocities;

      /** Random number generator */
      CRandom::CRNG* m_pcRNG;

      /** Whether to add noise or not */
      bool m_bAddNoise;

      /** Noise range on velocity */
      CRange<Real> m_cVelNoiseRange;

      /** Noise range on distance */
      CRange<Real> m_cDistNoiseRange;
   };

}

#endif
