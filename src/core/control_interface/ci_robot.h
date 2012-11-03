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

      virtual CCI_Actuator* GetActuator(const std::string& str_actuator_type);

      virtual CCI_Sensor* GetSensor(const std::string& str_sensor_type);

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
