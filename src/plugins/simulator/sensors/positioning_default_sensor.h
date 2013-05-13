/**
 * @file <argos3/plugins/simulator/sensors/positioning_default_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef POSITIONING_DEFAULT_SENSOR_H
#define POSITIONING_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CPositioningDefaultSensor;
   class CEmbodiedEntity;
}

#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

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
      CEmbodiedEntity* m_pcEmbodiedEntity;

      /** Random number generator */
      CRandom::CRNG* m_pcRNG;

      /** Whether to add noise or not */
      bool m_bAddNoise;

      /** Noise range on position */
      CRange<Real> m_cPosNoiseRange;

      /** Noise range on angle */
      CRange<CRadians> m_cAngleNoiseRange;

      /** Noise range on axis */
      CRange<Real> m_cAxisNoiseRange;
   };

}

#endif
