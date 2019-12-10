/**
 * @file <argos3/plugins/simulator/sensors/battery_default_sensor.h>
 *
 * @author Adhavan Jayabalan <jadhu94@gmail.com>
 */

#ifndef BATTERY_DEFAULT_SENSOR_H
#define BATTERY_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CBatteryDefaultSensor;
   class CBatteryEquippedEntity;
   class CPhysicsEngine;
}

#include <argos3/plugins/robots/generic/control_interface/ci_battery_sensor.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/generic/simulator/noise_injector.h>

namespace argos {

   class CBatteryDefaultSensor : public CSimulatedSensor,
                                     public CCI_BatterySensor {

   public:

      CBatteryDefaultSensor();

      virtual ~CBatteryDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   protected:

      /** Reference to embodied entity associated to this sensor */
      CEmbodiedEntity* m_pcEmbodiedEntity;

      /** Reference to battery sensor equipped entity associated to this sensor */
      CBatteryEquippedEntity* m_pcBatteryEntity;

     /** Noise injector */
      CNoiseInjector m_cNoiseInjector;
   };

}

#endif
