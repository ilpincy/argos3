/**
 * @file <argos3/core/control_interface/ci_sensor.h>
 *
 * @brief This file provides the basic interface for all actuators.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CI_SENSOR_H
#define CI_SENSOR_H

namespace argos {
   class CCI_Sensor;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/configuration/base_configurable_resource.h>

#include <map>

namespace argos {

   class CCI_Sensor : public CBaseConfigurableResource {

   public:

      typedef std::map<std::string, CCI_Sensor*, std::less<std::string> > TMap;

   public:

      virtual ~CCI_Sensor() {}

      virtual void Init(TConfigurationNode& t_node) {}

      virtual void Reset() {}

      virtual void Destroy() {}
   };

}

#endif
