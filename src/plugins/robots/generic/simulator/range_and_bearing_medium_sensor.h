/**
 * @file <argos3/plugins/simulator/sensors/range_and_bearing_medium_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef RANGE_AND_BEARING_MEDIUM_SENSOR_H
#define RANGE_AND_BEARING_MEDIUM_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CRangeAndBearingMediumSensor;
   class CRABEquippedEntity;
   class CControllableEntity;
   class CRABMedium;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>

namespace argos {

   class CRangeAndBearingMediumSensor : public CSimulatedSensor,
                                        public CCI_RangeAndBearingSensor {

   public:

      CRangeAndBearingMediumSensor();
      virtual ~CRangeAndBearingMediumSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      virtual void Destroy();

      virtual void Enable();

      virtual void Disable();

      /**
       * Returns true if the rays must be shown in the GUI.
       * @return true if the rays must be shown in the GUI.
       */
      inline bool IsShowRays() {
         return m_bShowRays;
      }

      /**
       * Sets whether or not the rays must be shown in the GUI.
       * @param b_show_rays true if the rays must be shown, false otherwise
       */
      inline void SetShowRays(bool b_show_rays) {
         m_bShowRays = b_show_rays;
      }

   private:

      CRABEquippedEntity*  m_pcRangeAndBearingEquippedEntity;
      CControllableEntity* m_pcControllableEntity;
      CRABMedium*          m_pcRangeAndBearingMedium;
      Real                 m_fDistanceNoiseStdDev;
      Real                 m_fPacketDropProb;
      CRandom::CRNG*       m_pcRNG;
      CSpace&              m_cSpace;
      bool                 m_bShowRays;
   };
}

#endif
