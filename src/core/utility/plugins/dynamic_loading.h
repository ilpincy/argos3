/**
 * @file <argos3/core/utility/plugins/dynamic_loading.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMIC_LOADING_H
#define DYNAMIC_LOADING_H

#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>

#include <map>
#include <string>

#include <dlfcn.h>
#include <cstdlib>

namespace argos {

   /**
    * Allows users to dynamically load shared libraries
    */
   class CDynamicLoading {

   public:

      /**
       * The handle to a loaded library
       */
      typedef void* TDLHandle;

   public:

      /**
       * Loads a dynamic library
       * @param str_lib The path of the dynamic library to load
       * @return A handle to the loaded dynamic library
       */
      static TDLHandle LoadLibrary(const std::string& str_lib);

      /**
       * Unloads a dynamic library
       * @param str_lib The path of the dynamic library to unload
       */
      static void UnloadLibrary(const std::string& str_lib);

      /**
       * Unloads a dynamic library
       * @param t_lib The handle to the dynamic library to unload
       */
      static void UnloadLibrary(TDLHandle t_lib);

      /**
       * Loads all the dynamic libraries in the current ARGOS_PLUGIN_PATH
       * Internally used to execute queries (<tt>argos3 --query</tt>)
       */
      static void LoadAllLibraries();

   private:

      /**
       * A type definition of the map of loaded dynamic libraries
       */
      typedef std::map<std::string, TDLHandle> TDLHandleMap;

      /**
       * The map of loaded dynamic libraries
       */
      static TDLHandleMap m_tOpenLibs;

      /**
       * Default plugin paths
       */
      static const std::string DEFAULT_PLUGIN_PATH;
   };

}

#endif
