/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_lidar_default_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 * @author Andreas Pasternak - <andreas.pasternak@gmx.ch>
 */

#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/simulator.h>

#include "footbot_lidar_default_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   class CLidarSensorImpl : public CLidarDefaultSensor {

   public:

      virtual Real CalculateReading(Real f_distance) {
         return f_distance;
      }

   };

   /****************************************/
   /****************************************/

   CFootBotLidarDefaultSensor::CFootBotLidarDefaultSensor() :
      m_pcLidarImpl(new CLidarSensorImpl()) {}

   /****************************************/
   /****************************************/

   CFootBotLidarDefaultSensor::~CFootBotLidarDefaultSensor() {
      delete m_pcLidarImpl;
   }

   /****************************************/
   /****************************************/

   void CFootBotLidarDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      try {
         m_pcLidarImpl->SetRobot(c_entity);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the foot-bot lidar default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CFootBotLidarDefaultSensor::Init(TConfigurationNode& t_tree) {
      m_pcLidarImpl->Init(t_tree);
   }

   /****************************************/
   /****************************************/

   void CFootBotLidarDefaultSensor::Update() {
      m_pcLidarImpl->Update();
      for(size_t i = 0; i < m_pcLidarImpl->GetReadings().size(); ++i) {
         m_tReadings[i].Value = m_pcLidarImpl->GetReadings()[i];
      }
   }

   /****************************************/
   /****************************************/

   void CFootBotLidarDefaultSensor::Reset() {
      m_pcLidarImpl->Reset();
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CFootBotLidarDefaultSensor,
                   "footbot_lidar", "default",
                   "Carlo Pinciroli [ilpincy@gmail.com], Andreas Pasternak [andreas.pasternak@gmx.ch]",
                   "1.0",
                   "The foot-bot lidar sensor.",
                   "This sensor accesses the foot-bot lidar sensor. For a complete description\n"
                   "of its usage, refer to the ci_footbot_lidar_sensor.h interface. For the XML\n"
                   "configuration, refer to the default lidar sensor.\n",
                   "Usable"
		  );

}
