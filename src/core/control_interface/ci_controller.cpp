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
      for(auto itActuators = m_mapActuators.begin();
          itActuators != m_mapActuators.end();
          ++itActuators) {
         delete itActuators->second;
      }
      m_mapActuators.clear();

      /* Delete sensors */
      for(auto itSensors = m_mapSensors.begin();
          itSensors != m_mapSensors.end();
          ++itSensors) {
         delete itSensors->second;
      }
      m_mapSensors.clear();
   }

   /****************************************/
   /****************************************/

   bool CCI_Controller::HasActuator(const std::string& str_actuator_type) const {
      auto it = m_mapActuators.find(str_actuator_type);
      return (it != m_mapActuators.end());
   }

   /****************************************/
   /****************************************/

   bool CCI_Controller::HasSensor(const std::string& str_sensor_type) const {
      auto it = m_mapSensors.find(str_sensor_type);
      return (it != m_mapSensors.end());
   }

   /****************************************/
   /****************************************/

}
