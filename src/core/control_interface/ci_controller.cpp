/**
 * @file <argos3/core/control_interface/ci_controller.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "ci_controller.h"

namespace argos {

   /****************************************/
   /****************************************/

   CCI_Controller::~CCI_Controller() {
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
