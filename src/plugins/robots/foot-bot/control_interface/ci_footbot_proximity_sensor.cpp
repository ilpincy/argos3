/**
 * @file <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_footbot_proximity_sensor.h"

namespace argos {

   /****************************************/
   /****************************************/

   const UInt32 CCI_FootBotProximitySensor::NUM_READINGS = 24;
   const CRange<Real> CCI_FootBotProximitySensor::READING_RANGE(0.0f, 4096.0f);

   /****************************************/
   /****************************************/

   CCI_FootBotProximitySensor::CCI_FootBotProximitySensor() :
      CCI_RingProximitySensor(NUM_READINGS) {}

   /****************************************/
   /****************************************/

}
