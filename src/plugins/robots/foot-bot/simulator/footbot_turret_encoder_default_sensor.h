/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_turret_encoder_default_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FOOTBOT_TURRET_ENCODER_DEFAULT_SENSOR_H
#define FOOTBOT_TURRET_ENCODER_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CFootBotTurretEncoderDefaultSensor;
}

#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_turret_encoder_sensor.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_turret_encoder_default_sensor.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_turret_entity.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   class CFootBotTurretEncoderDefaultSensor : public CCI_FootBotTurretEncoderSensor,
                                              public CSimulatedSensor {

   public:

      CFootBotTurretEncoderDefaultSensor();

      virtual ~CFootBotTurretEncoderDefaultSensor() {}

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Update();

      virtual void Reset();

   private:

      CFootBotTurretEntity* m_pcTurretEntity;

   };

}

#endif
