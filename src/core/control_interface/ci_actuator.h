/**
 * @file core/control_interface/ci_actuator.h
 *
 * @brief This file provides the basic interface for all actuators.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef CI_ACTUATOR_H
#define CI_ACTUATOR_H

namespace argos {
   class CCI_Actuator;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/configuration/base_configurable_resource.h>
#include <argos3/core/utility/configuration/memento.h>

#include <map>

namespace argos {

   class CCI_Actuator : public CBaseConfigurableResource,
                        public CMemento {

   public:

      typedef std::map<std::string, CCI_Actuator*, std::less<std::string> > TMap;

   public:

      virtual ~CCI_Actuator() = 0;

   };

}

#endif
