/**
 * @file core/control_interface/ci_robot.h
 *
 * @brief This file provides the definition of the robot class.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CI_ROBOT_H
#define CI_ROBOT_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_Robot;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/control_interface/ci_actuator.h>

#include <map>
#include <string>
#include <cxxabi.h>
#include <typeinfo>

namespace argos {

   class CCI_Robot {

   public:

      virtual ~CCI_Robot();

      virtual CCI_Actuator* InsertActuator(const std::string& str_actuator_type) {
          return NULL;
      }

      virtual CCI_Sensor* InsertSensor(const std::string& str_sensor_type) {
          return NULL;
      }

      template<typename ACTUATOR_IMPL>
      ACTUATOR_IMPL* GetActuator(const std::string& str_actuator_type) {
         CCI_Actuator::TMap::const_iterator it = m_mapActuators.find(str_actuator_type);
         if (it != m_mapActuators.end()) {
            ACTUATOR_IMPL* pcActuator = dynamic_cast<ACTUATOR_IMPL>(it->second);
            if(pcActuator != NULL) {
               return pcActuator;
            }
            else {
               char* pchDemangledType = abi::__cxa_demangle(typeid(ACTUATOR_IMPL).name(), NULL, NULL, NULL);
               THROW_ARGOSEXCEPTION("Actuator type " << str_actuator_type << " cannot be cast to type " << pchDemangledType);
            }
         }
         else {
            THROW_ARGOSEXCEPTION("Unknown actuator type " << str_actuator_type << " requested in controller");
         }
      }

      template<typename SENSOR_IMPL>
      SENSOR_IMPL* GetSensor(const std::string& str_sensor_type) {
         CCI_Sensor::TMap::const_iterator it = m_mapSensors.find(str_sensor_type);
         if (it != m_mapSensors.end()) {
            SENSOR_IMPL* pcSensor = dynamic_cast<SENSOR_IMPL>(it->second);
            if(pcSensor != NULL) {
               return pcSensor;
            }
            else {
               char* pchDemangledType = abi::__cxa_demangle(typeid(SENSOR_IMPL).name(), NULL, NULL, NULL);
               THROW_ARGOSEXCEPTION("Sensor type " << str_sensor_type << " cannot be cast to type " << pchDemangledType);
            }
         }
         else {
            THROW_ARGOSEXCEPTION("Unknown sensor type " << str_sensor_type << " requested in controller");
         }
      }

      inline CCI_Actuator::TMap& GetAllActuators() {
    	  return m_mapActuators;
      }

      inline CCI_Sensor::TMap& GetAllSensors() {
    	  return m_mapSensors;
      }

      inline void AddActuator(const std::string& str_actuator_type,
                              CCI_Actuator* pc_actuator) {
         m_mapActuators[str_actuator_type] = pc_actuator;
      }

      inline void AddSensor(const std::string& str_sensor_type,
                            CCI_Sensor* pc_sensor) {
         m_mapSensors[str_sensor_type] = pc_sensor;
      }

      inline const std::string& GetRobotId() const {
         return m_strRobotId;
      }

      inline void SetRobotId(const std::string& str_robot_id) {
         m_strRobotId = str_robot_id;
      }

   protected:

      CCI_Actuator::TMap m_mapActuators;
      CCI_Sensor::TMap m_mapSensors;

      std::string m_strRobotId;

   };

}

#endif
