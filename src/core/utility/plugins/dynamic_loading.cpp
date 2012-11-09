/**
 * @file core/utility/plugins/dynamic_loading.h
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "dynamic_loading.h"
#include <argos3/core/utility/configuration/argos_exception.h>
#include <dlfcn.h>

namespace argos {

   /****************************************/
   /****************************************/

   TDLHandleMap CDynamicLoading::m_tOpenLibs;

   /****************************************/
   /****************************************/

   CDynamicLoading::TDLHandle CDynamicLoading::LoadLibrary(const std::string& str_lib) {
      /** @todo Expand variable ARGOSPLUGINDIR and use it */
      TDLHandle tHandle = ::dlopen(str_lib.c_str(), RTLD_NOW);
      if(tHandle == NULL) {
         THROW_ARGOSEXCEPTION("Can't load library \""
                              << str_lib
                              << "\": "
                              << ::dlerror());
      }
      m_tOpenLibs[str_lib] = tHandle;
      return tHandle;
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

