/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_proximity_default_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef FOOTBOT_PROXIMITY_DEFAULT_SENSOR_H
#define FOOTBOT_PROXIMITY_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CFootBotProximityDefaultSensor;
}

#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
#include <argos3/plugins/robots/generic/simulator/proximity_default_sensor.h>

namespace argos {

   class CFootBotProximityDefaultSensor : public CCI_FootBotProximitySensor,
                                          public CSimulatedSensor {

   public:

      CFootBotProximityDefaultSensor();

      virtual ~CFootBotProximityDefaultSensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      CProximityDefaultSensor* m_pcProximityImpl;

   };

}

#endif
