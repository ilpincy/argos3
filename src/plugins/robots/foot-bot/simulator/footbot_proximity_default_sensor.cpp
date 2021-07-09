/**
 * @file <argos3/plugins/robots/foot-bot/simulator/footbot_proximity_default_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/simulator.h>

#include "footbot_proximity_default_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   class CProximitySensorImpl : public CProximityDefaultSensor {

   public:

      virtual Real CalculateReading(Real f_distance) {
         if(f_distance < 0.009889556) {
            return 1.0;
         }
         else {
            return 0.0100527 / (f_distance + 0.000163144);
         }
      }

   };

   /****************************************/
   /****************************************/

   CFootBotProximityDefaultSensor::CFootBotProximityDefaultSensor() :
      m_pcProximityImpl(new CProximitySensorImpl()) {}

   /****************************************/
   /****************************************/

   CFootBotProximityDefaultSensor::~CFootBotProximityDefaultSensor() {
      delete m_pcProximityImpl;
   }

   /****************************************/
   /****************************************/

   void CFootBotProximityDefaultSensor::SetRobot(CComposableEntity& c_entity) {
      try {
         m_pcProximityImpl->SetRobot(c_entity);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Can't set robot for the foot-bot proximity default sensor", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CFootBotProximityDefaultSensor::Init(TConfigurationNode& t_tree) {
      m_pcProximityImpl->Init(t_tree);
   }

   /****************************************/
   /****************************************/

   void CFootBotProximityDefaultSensor::Update() {
      m_pcProximityImpl->Update();
      for(size_t i = 0; i < m_pcProximityImpl->GetReadings().size(); ++i) {
         m_tReadings[i].Value = m_pcProximityImpl->GetReadings()[i];
      }
   }

   /****************************************/
   /****************************************/

   void CFootBotProximityDefaultSensor::Reset() {
      m_pcProximityImpl->Reset();
   }

   /****************************************/
   /****************************************/

   void CFootBotProximityDefaultSensor::Enable() {
     CCI_Sensor::Enable();
     m_pcProximityImpl->Enable();
   }

   /****************************************/
   /****************************************/

   void CFootBotProximityDefaultSensor::Disable() {
     CCI_Sensor::Disable();
     m_pcProximityImpl->Disable();
   }


   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CFootBotProximityDefaultSensor,
                   "footbot_proximity", "default",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "The foot-bot proximity sensor.",
                   "This sensor accesses the foot-bot proximity sensor. For a complete description\n"
                   "of its usage, refer to the ci_footbot_proximity_sensor.h interface. For the XML\n"
                   "configuration, refer to the default proximity sensor.\n",
                   "Usable"
		  );

}
