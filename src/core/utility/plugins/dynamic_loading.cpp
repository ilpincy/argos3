/**
 * @file core/utility/plugins/dynamic_loading.cpp
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "dynamic_loading.h"

namespace argos {

   CDynamicLoading::TDLHandleMap CDynamicLoading::m_tOpenLibs;
   const std::string CDynamicLoading::DEFAULT_PLUGIN_PATH = "/usr/lib/argos3/:/usr/local/lib/argos3/";

}
