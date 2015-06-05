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
       * Loads a dynamic library.
       * This method tries to load the library pointed to by the specified path.
       * <p>
       * If the given path is absolute, this method tries to load the library
       * pointed to by the specified path. If loading fails, it tries another time
       * appending the extension for shared libraries (e.g., <tt>.so</tt> on
       * Linux, <tt>.dylib</tt> on Mac) to the path. If also this attempt fails,
       * this method tries appending the extension for module libraries (e.g.,
       * <tt>.so</tt> on Linux and Mac). If all attempts fail, the method raises
       * an exception.
       * </p>
       * <p>
       * If the given path is relative, this method forms absolute paths
       * prepending the given path with the directories of the default loading
       * path (e.g., /usr/lib/argos3), the contents of ARGOS_PLUGIN_PATH, and
       * the current working directory. For each absolute path so formed, the
       * method attempts to load the library in the same way as described above
       * (e.g., appending the extension of shared/module libraries if needed).
       * </p>
       * @param str_lib The path of the dynamic library to load
       * @return A handle to the loaded dynamic library
       * @throws CARGoSException in case of error
       */
      static TDLHandle LoadLibrary(const std::string& str_lib);

      /**
       * Unloads a dynamic library.
       * @param str_lib The path of the dynamic library to unload
       * @throws CARGoSException in case of error
       */
      static void UnloadLibrary(const std::string& str_lib);

      /**
       * Loads all the dynamic libraries in the current ARGOS_PLUGIN_PATH.
       * Internally used to execute queries (<tt>argos3 --query</tt>).
       * @throws CARGoSException in case of error
       */
      static void LoadAllLibraries();

      /**
       * Unloads all the dynamic libraries.
       * @throws CARGoSException in case of error
       */
      static void UnloadAllLibraries();

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
