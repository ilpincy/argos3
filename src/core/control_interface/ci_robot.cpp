/**
 * @file argos3/core/control_interface/ci_robot.h
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

   CCI_Actuator* CCI_Robot::GetActuator(const std::string& str_actuator_type) {
      CCI_Actuator::TMap::const_iterator it = m_mapActuators.find(str_actuator_type);
      if (it != m_mapActuators.end()) return it->second;
      THROW_ARGOSEXCEPTION("Unknown actuator type " << str_actuator_type << " requested in controller");
   }
   
   /****************************************/
   /****************************************/

   CCI_Sensor* CCI_Robot::GetSensor(const std::string& str_sensor_type) {
      CCI_Sensor::TMap::const_iterator it = m_mapSensors.find(str_sensor_type);
      if (it != m_mapSensors.end()) return it->second;
      THROW_ARGOSEXCEPTION("Unknown sensor type " << str_sensor_type << " requested in controller");
   }

   /****************************************/
   /****************************************/

}
