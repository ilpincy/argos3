/**
 * @file <argos3/plugins/simulator/sensors/lidar_default_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Andreas Pasternak - <andreas.pasternak@gmx.ch>
 */

#ifndef LIDAR_DEFAULT_SENSOR_H
#define LIDAR_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CLidarDefaultSensor;
   class CLidarSensorEquippedEntity;
}

#include <argos3/plugins/robots/generic/control_interface/ci_lidar_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CLidarDefaultSensor : public CSimulatedSensor,
                                   public CCI_LidarSensor {

   public:

      CLidarDefaultSensor();

      virtual ~CLidarDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      /**
       * Returns its input. This function exists for compatibility reasons only.
       * @param f_distance The distance of the closest occluding object in meters
       * @returns The distance of the closest occluding object in meters.
       */
      virtual Real CalculateReading(Real f_distance);

   protected:

      /** Reference to embodied entity associated to this sensor */
      CEmbodiedEntity* m_pcEmbodiedEntity;

      /** Reference to lidar sensor equipped entity associated to this sensor */
      CLidarSensorEquippedEntity* m_pcLidarEntity;

      /** Reference to controllable entity associated to this sensor */
      CControllableEntity* m_pcControllableEntity;

      /** Flag to show rays in the simulator */
      bool m_bShowRays;

      /** Random number generator */
      CRandom::CRNG* m_pcRNG;

      /** Whether to add noise or not */
      bool m_bAddNoise;

      /** Noise range */
      CRange<Real> m_cNoiseRange;

      /** Reference to the space */
      CSpace& m_cSpace;
   };

}

#endif
