/**
 * @file <argos3/plugins/simulator/sensors/battery_default_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef BATTERY_DEFAULT_SENSOR_H
#define BATTERY_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CBatteryDefaultSensor;
   class CBatterySensorEquippedEntity;
   class CPhysicsEngine;
}

#include <argos3/plugins/robots/generic/control_interface/ci_battery_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CBatteryDefaultSensor : public CSimulatedSensor,
                                     public CCI_BatterySensor {

   public:

      CBatteryDefaultSensor();

      virtual ~CBatteryDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   protected:

      /** Reference to embodied entity associated to this sensor */
      CEmbodiedEntity* m_pcEmbodiedEntity;

      /** Reference to battery sensor equipped entity associated to this sensor */
      CBatterySensorEquippedEntity* m_pcBatteryEntity;

      /** Random number generator */
      CRandom::CRNG* m_pcRNG;

      /** Whether to add noise or not */
      bool m_bAddNoise;

      /** Noise range on battery level */
      CRange<Real> m_cLevelNoiseRange;

      /** Discharge rate */
      Real m_fDischarge;
   };

}

#endif
