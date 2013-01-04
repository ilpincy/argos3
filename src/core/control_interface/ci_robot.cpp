/**
 * @file <argos3/core/control_interface/ci_robot.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_robot.h"

namespace argos {

   /****************************************/
   /****************************************/

   CCI_Robot::~CCI_Robot() {
      /* Delete actuators*/
      for(CCI_Actuator::TMap::iterator itActuators = m_mapActuators.begin();
          itActuators != m_mapActuators.end();
          ++itActuators) {
         delete itActuators->second;
      }
      m_mapActuators.clear();

      /* Delete sensors */
      for(CCI_Sensor::TMap::iterator itSensors = m_mapSensors.begin();
          itSensors != m_mapSensors.end();
          ++itSensors) {
         delete itSensors->second;
      }
      m_mapSensors.clear();
   }

   /****************************************/
   /****************************************/

}
