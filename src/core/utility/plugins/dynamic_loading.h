/**
 * @file core/utility/plugins/dynamic_loading.h
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMIC_LOADING_H
#define DYNAMIC_LOADING_H

#include <map>
#include <string>

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
       * Class constructor
       */
      CDynamicLoading() {}

      /**
       * Class destructor
       */
      ~CDynamicLoading();

      /**
       * Loads a dynamic library
       * @param str_lib The path of the dynamic library to load
       * @return A handle to the loaded dynamic library
       */
      TDLHandle LoadLibrary(const std::string& str_lib);

      /**
       * Unloads a dynamic library
       * @param str_lib The path of the dynamic library to unload
       */
      void UnloadLibrary(const std::string& str_lib);

      /**
       * Unloads a dynamic library
       * @param t_lib The handle to the dynamic library to unload
       */
      void UnloadLibrary(TDLHandle t_lib);

   private:

      /**
       * A type definition of the map of loaded dynamic libraries
       */
      typedef std::map<std::string, TDLHandle> TDLHandleMap;

      /**
       * The map of loaded dynamic libraries
       */
      TDLHandleMap m_tOpenLibs;

   };

}

#endif
