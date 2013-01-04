/**
 * @file <argos3/core/utility/configuration/base_configurable_resource.h>
 *
 * @brief This file provides the definition of the base configurable resource interface.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef BASE_CONFIGURABLE_RESOURCE_H
#define BASE_CONFIGURABLE_RESOURCE_H

namespace argos {
   class CBaseConfigurableResource;
}

#include <argos3/core/utility/configuration/argos_configuration.h>

namespace argos {

   /**
    * This class is the base of all XML-configurable ARGoS interface.
    */
   class CBaseConfigurableResource {

   public:

      /**
       * Class destructor.
       */
      virtual ~CBaseConfigurableResource() {}

      /**
       * Initializes the resource.
       * @param t_tree the base of the XML configuration tree to parse
       * @throw CARGoSException if an error occurs
       */
      virtual void Init(TConfigurationNode& t_tree) = 0;

      /**
       * Resets the resource.
       */
      virtual void Reset() = 0;

      /**
       * Undoes whatever was done by Init().
       */
      virtual void Destroy() = 0;

   };

}

#endif
