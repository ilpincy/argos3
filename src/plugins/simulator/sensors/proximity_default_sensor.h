/**
 * @file <argos3/plugins/robots/foot-bot/simulator/ring_proximity_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PROXIMITY_SENSOR_H
#define PROXIMITY_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CProximityDefaultSensor;
}

#include <argos3/plugins/robots/generic/control_interface/ci_proximity_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/utility/math/rng.h>

namespace argos {

   class CProximityDefaultSensor : public CSimulatedSensor,
                                   public CCI_ProximitySensor {

   public:

      CProximityDefaultSensor();

      virtual ~CProximityDefaultSensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual Real CalculateReading(Real f_distance);

   protected:

      struct SSensor {
         CVector3 Position;
         CVector3 Direction;
         SSensor(const CVector3& c_position,
                 const CVector3& c_direction,
                 Real f_range) :
            Position(c_position),
            Direction(c_direction) {
            Direction.Normalize();
            Direction *= f_range;
         }
      };

   protected:

      /** The list of sensors */
      std::vector<SSensor*> m_vecSensors;

      /** Reference to embodied entity associated to this sensor */
      CEmbodiedEntity* m_pcEmbodiedEntity;

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

      /** Reference to the space hash of embodied entities */
      CSpaceHash<CEmbodiedEntity, CEmbodiedEntitySpaceHashUpdater>& m_cEmbodiedSpaceHash;

      /** Used to calculate the ray intersection */
      TEmbodiedEntitySet m_tIgnoreMe;
   };

}

#endif
