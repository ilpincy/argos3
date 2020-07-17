/**
 * @file <argos3/plugins/simulator/sensors/ground_rotzonly_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef GROUND_ROTZONLY_SENSOR_H
#define GROUND_ROTZONLY_SENSOR_H

#include <string>
#include <map>
#include <memory>

namespace argos {
   class CGroundRotZOnlySensor;
   class CGroundSensorEquippedEntity;
   class CFloorEntity;
}

#include <argos3/plugins/robots/generic/control_interface/ci_ground_sensor.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/generic/simulator/noise_injector.h>

namespace argos {

   class CGroundRotZOnlySensor : public CSimulatedSensor,
                                 public CCI_GroundSensor {

   public:

      CGroundRotZOnlySensor();

      virtual ~CGroundRotZOnlySensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   protected:

      /** Reference to embodied entity associated to this sensor */
      CEmbodiedEntity* m_pcEmbodiedEntity;

      /** Reference to floor entity */
      CFloorEntity* m_pcFloorEntity;

      /** Reference to ground sensor equipped entity associated to this sensor */
      CGroundSensorEquippedEntity* m_pcGroundSensorEntity;

      /** Noise injector */
     std::unique_ptr<CNoiseInjector> m_pcNoiseInjector;

      /** Reference to the space */
      CSpace& m_cSpace;
   };

}

#endif
