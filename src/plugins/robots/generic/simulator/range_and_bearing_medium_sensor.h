/**
 * @file <argos3/plugins/simulator/sensors/range_and_bearing_medium_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef RANGE_AND_BEARING_MEDIUM_SENSOR_H
#define RANGE_AND_BEARING_MEDIUM_SENSOR_H

#include <string>
#include <map>
#include <memory>

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
#include <argos3/plugins/robots/generic/simulator/noise_injector.h>

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

      CRABEquippedEntity*            m_pcRangeAndBearingEquippedEntity;
      CControllableEntity*           m_pcControllableEntity;
      CRABMedium*                    m_pcRangeAndBearingMedium;
      CSpace&                        m_cSpace;
      bool                           m_bShowRays;

     /* Position noise injectors */
     std::unique_ptr<CNoiseInjector> m_pcDistanceNoiseInjector;
     std::unique_ptr<CNoiseInjector> m_pcInclinationNoiseInjector;
     std::unique_ptr<CNoiseInjector> m_pcAzimuthNoiseInjector;

     /** Packet drop probability noise injector */
     std::unique_ptr<CNoiseInjector> m_pcPacketDropNoiseInjector;
   };
}

#endif
