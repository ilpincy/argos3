/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_lidar_default_sensor.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Andreas Pasternak - <andreas.pasternak@gmx.ch>
 */

#ifndef FOOTBOT_LIDAR_DEFAULT_SENSOR_H
#define FOOTBOT_LIDAR_DEFAULT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CFootBotLidarDefaultSensor;
}

#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_lidar_sensor.h>
#include <argos3/plugins/robots/generic/simulator/lidar_default_sensor.h>

namespace argos {

   class CFootBotLidarDefaultSensor : public CCI_FootBotLidarSensor,
                                          public CSimulatedSensor {

   public:

      CFootBotLidarDefaultSensor();

      virtual ~CFootBotLidarDefaultSensor();

      virtual void SetRobot(CComposableEntity& c_entity);

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Update();

      virtual void Reset();

   private:

      CLidarDefaultSensor* m_pcLidarImpl;

   };

}

#endif
