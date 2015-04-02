/**
 * @file <argos3/plugins/robots/eye-bot/simulator/eyebot_proximity_default_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef EYEBOT_PROXIMITY_DEFAULT_SENSOR_H
#define EYEBOT_PROXIMITY_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CEyeBotProximityDefaultSensor;
}

#include <argos3/plugins/robots/eye-bot/control_interface/ci_eyebot_proximity_sensor.h>
#include <argos3/plugins/robots/generic/simulator/proximity_default_sensor.h>

namespace argos {

   class CEyeBotProximityDefaultSensor : public CCI_EyeBotProximitySensor,
                                         public CSimulatedSensor {

   public:

      CEyeBotProximityDefaultSensor();

      virtual ~CEyeBotProximityDefaultSensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      CProximityDefaultSensor* m_pcProximityImpl;

   };

}

#endif
