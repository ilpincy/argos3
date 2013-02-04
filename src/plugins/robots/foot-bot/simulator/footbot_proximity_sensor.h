/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_proximity_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FOOTBOT_PROXIMITY_SENSOR_H
#define FOOTBOT_PROXIMITY_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CFootBotProximitySensor;
}

#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/utility/math/rng.h>

namespace argos {

   class CFootBotProximitySensor : public CSimulatedSensor<CFootBotEntity>,
                                   public CCI_FootBotProximitySensor {

   public:

      CFootBotProximitySensor();
      virtual ~CFootBotProximitySensor() {}

      /**
       * @brief Sets entity that is equipped with these wheels.
       *
       * @param c_entity entity equipped with the wheels.
       */
      virtual void SetEntity(CEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();
      virtual void Reset();

   private:

      /* Adds noise to the sensor reading */
      virtual void AddNoise(UInt32 un_sensor_index);

   private:

      CSpace& m_cSpace;
      CSpaceHash<CEmbodiedEntity, CEmbodiedEntitySpaceHashUpdater>& m_cEmbodiedSpaceHash;
      CEmbodiedEntity* m_pcEmbodiedEntity;

      /* Flag for showing rays in the simulator */
      bool m_bShowRays;

      /* Flag for knowing if sensors are calibrated and readings normalised */
      bool m_bCalibrated;

      /* Random number generator */
      CRandom::CRNG* m_pcRNG;

      /* Noise parameter */
      Real m_fNoiseLevel;

   };

}

#endif
