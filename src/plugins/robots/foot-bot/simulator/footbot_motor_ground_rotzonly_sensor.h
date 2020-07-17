/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_motor_ground_rotzonly_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FOOTBOT_MOTOR_GROUND_ROTZONLY_SENSOR_H
#define FOOTBOT_MOTOR_GROUND_ROTZONLY_SENSOR_H

#include <string>
#include <map>
#include <memory>

namespace argos {
   class CFootBotMotorGroundRotZOnlySensor;
   class CGroundSensorEquippedEntity;
   class CFloorEntity;
}

#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_motor_ground_sensor.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/generic/simulator/noise_injector.h>

namespace argos {

   class CFootBotMotorGroundRotZOnlySensor : public CCI_FootBotMotorGroundSensor,
                                             public CSimulatedSensor {

   public:

      CFootBotMotorGroundRotZOnlySensor();

      virtual ~CFootBotMotorGroundRotZOnlySensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   protected:

      /** Reference to embodied entity associated to this sensor */
      CEmbodiedEntity*                m_pcEmbodiedEntity;

      /** Reference to floor entity */
      CFloorEntity*                   m_pcFloorEntity;

      /** Reference to ground sensor equipped entity associated to this sensor */
      CGroundSensorEquippedEntity*    m_pcGroundSensorEntity;

      /** Reference to the space */
      CSpace&                         m_cSpace;

      /** Noise injector */
      std::unique_ptr<CNoiseInjector> m_pcNoiseInjector;
   };

}

#endif
