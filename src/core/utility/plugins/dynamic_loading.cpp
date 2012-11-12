/**
 * @file core/utility/plugins/dynamic_loading.cpp
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "dynamic_loading.h"

namespace argos {

   /****************************************/
   /****************************************/

   CDynamicLoading::TDLHandleMap CDynamicLoading::m_tOpenLibs;
   const std::string CDynamicLoading::DEFAULT_PLUGIN_PATH = "/usr/lib/argos3/:/usr/local/lib/argos3/";

   /****************************************/
   /****************************************/

   CDynamicLoading::TDLHandle CDynamicLoading::LoadLibrary(const std::string& str_lib) {
      TDLHandle tHandle;
      /* Check if the provided path is absolute or relative */
      if(str_lib[0] == '/') {
         /*
          * Absolute path, load the library directly and bomb out in case of failure
          */
         tHandle = ::dlopen(str_lib.c_str(), RTLD_NOW);
         if(tHandle == NULL) {
            THROW_ARGOSEXCEPTION(::dlerror());
         }
         /* Store the handle to the loaded library */
         m_tOpenLibs[str_lib] = tHandle;
         return tHandle;
      }
      else {
         /*
          * Relative path, go through the plugin directories
          */
         /* String to store the full path to a library */
         std::string strLibPath;
         /* String to store the list of paths to search */
         std::string strPluginPath = DEFAULT_PLUGIN_PATH;
         /* Get variable ARGOS_PLUGIN_PATH from the environment */
         if(::getenv("ARGOS_PLUGIN_PATH") != NULL) {
            /* Add value of the variable to list of paths to check */
            strPluginPath += ":" + std::string(::getenv("ARGOS_PLUGIN_PATH"));
         }
         /*
          * Go through paths and try to load the library
          */
         /* Add : at the end to make parsing easier */
         if(strPluginPath[strPluginPath.length()-1] != ':') {
            strPluginPath.append(":");
         }
         /* Parse the string */
         std::istringstream issPluginPath(strPluginPath);
         std::string strDir;
         while(std::getline(issPluginPath, strDir, ':')) {
            /* Add / if missing */
            if(strDir[strDir.length()-1] != '/') {
               strDir.append("/");
            }
            strLibPath = strDir + str_lib;
            tHandle = ::dlopen(strLibPath.c_str(), RTLD_NOW);
            if(tHandle != NULL) {
               /* Store the handle to the loaded library */
               m_tOpenLibs[strLibPath] = tHandle;
               return tHandle;
            }
            else {
               LOGERR << ::dlerror()
                      << std::endl;
            }
         }
         /* If we get here, it's because no directory worked */
         THROW_ARGOSEXCEPTION("Can't load library \""
                              << str_lib
                              << "\".");
      }
   }

   /****************************************/
   /****************************************/

   void CDynamicLoading::UnloadLibrary(const std::string& str_lib) {
      TDLHandleMap::iterator it = m_tOpenLibs.find(str_lib);
      if(it != m_tOpenLibs.end()) {
         UnloadLibrary(it->second);
      }
      else {
         THROW_ARGOSEXCEPTION("Can't load library \""
                              << str_lib
                              << "\": library does not appear to have been loaded.");
      }
   }

   /****************************************/
   /****************************************/

   void CDynamicLoading::UnloadLibrary(TDLHandle t_lib) {
      ::dlclose(t_lib);
   }

   /****************************************/
   /****************************************/

}
