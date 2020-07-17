/**
 * @file <argos3/plugins/simulator/sensors/epuck_proximity_default_sensor.h>
 *
 * @author Danesh Tarapore - <daneshtarapore@gmail.com>
 */

#ifndef EPUCK_PROXIMITY_DEFAULT_SENSOR_H
#define EPUCK_PROXIMITY_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CEPuckProximityDefaultSensor;
   class CProximitySensorEquippedEntity;
}

#include <argos3/plugins/robots/e-puck/control_interface/ci_epuck_proximity_sensor.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/generic/simulator/noise_injector.h>

namespace argos {

   class CEPuckProximityDefaultSensor : public CSimulatedSensor,
                                   public CCI_EPuckProximitySensor {

   public:

      CEPuckProximityDefaultSensor();

      virtual ~CEPuckProximityDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

      /**
       * Calculates the proximity reading when the closest occluding object is located as the given distance.
       * @param f_distance The distance of the closest occluding object in meters
       * @returns A value in the range [0:1], where 0 means that the object is too far to be sensed, and 1 means the object is so close that it saturates the sensor.
       */
      virtual Real CalculateReading(Real f_distance);

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

   protected:

      /** Reference to embodied entity associated to this sensor */
      CEmbodiedEntity* m_pcEmbodiedEntity;

      /** Reference to proximity sensor equipped entity associated to this sensor */
      CProximitySensorEquippedEntity* m_pcProximityEntity;

      /** Reference to controllable entity associated to this sensor */
      CControllableEntity* m_pcControllableEntity;

      /** Flag to show rays in the simulator */
      bool m_bShowRays;

      /** Reference to the space */
      CSpace& m_cSpace;

     /** Noise injector */
     CNoiseInjector m_cNoiseInjector;
   };

}

#endif
