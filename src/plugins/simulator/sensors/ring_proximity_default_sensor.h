/**
 * @file <argos3/plugins/robots/foot-bot/simulator/ring_proximity_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef RING_PROXIMITY_SENSOR_H
#define RING_PROXIMITY_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CRingProximityDefaultSensor;
}

#include <argos3/plugins/robots/generic/control_interface/ci_ring_proximity_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/utility/math/rng.h>

namespace argos {

   class CRingProximityDefaultSensor : public CSimulatedSensor,
                                       public CCI_RingProximitySensor {

   public:

      CRingProximityDefaultSensor();
      virtual ~CRingProximityDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();
      virtual void Reset();

   protected:

      /** Radius of the ring */
      Real m_fRadius;

      /** Center of the ring wrt embodied entity */
      CVector3 m_cCenter;

      /** Range of the readings */
      CRange<Real> m_cReadingRange;

      /** Reference to the space */
      CSpace& m_cSpace;

      /** Reference to the space hash of embodied entities */
      CSpaceHash<CEmbodiedEntity, CEmbodiedEntitySpaceHashUpdater>& m_cEmbodiedSpaceHash;

      /** Reference to embodied entity associated to this sensor */
      CEmbodiedEntity* m_pcEmbodiedEntity;

      /** Flag to show rays in the simulator */
      bool m_bShowRays;

      /** Flag to know if sensors are calibrated and readings normalized */
      bool m_bCalibrated;

      /** Random number generator */
      CRandom::CRNG* m_pcRNG;

      /** Whether to add noise or not */
      bool m_bAddNoise;

      /** Noise range */
      CRange<Real> m_cNoiseRange;

   };

}

#endif
