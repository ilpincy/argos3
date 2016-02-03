/**
 * @file <argos3/plugins/simulator/sensors/light_default_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef LIGHT_DEFAULT_SENSOR_H
#define LIGHT_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CLightDefaultSensor;
   class CLightSensorEquippedEntity;
}

#include <argos3/plugins/robots/generic/control_interface/ci_light_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CLightDefaultSensor : public CSimulatedSensor,
                                   public CCI_LightSensor {

   public:

      CLightDefaultSensor();

      virtual ~CLightDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      /**
       * Calculates the light reading resulting from a light source at the given distance.
       * Denoting the intensity with <em>i</em> and the distance <em>x</em>, this function calculates <em>i</em> = (<em>I</em> / <em>x<em>)^2.
       * <em>I</em> is the reference intensity of the light, that is, the distance at which the light reading saturates.
       * It is dependent on the light entity being considered.
       * @param f_distance The distance of the considered light source.
       * @param f_intensity The reference intensity of the considered light source.
       * @returns A value in the range [0:1], where 0 means that the light is too far to be sensed, and 1 means the light is so close that it saturates the sensor.
       */
      virtual Real CalculateReading(Real f_distance, Real f_intensity);

   protected:

      /** Reference to light sensor equipped entity associated to this sensor */
      CLightSensorEquippedEntity* m_pcLightEntity;

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
