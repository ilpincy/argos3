/**
 * @file <argos3/core/utility/logging/argos_log.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "argos_log.h"

namespace argos {

   size_t DEBUG_INDENTATION = 0;
   CARGoSLog LOG(std::cout, SLogColor(ARGOS_LOG_ATTRIBUTE_BRIGHT, ARGOS_LOG_COLOR_GREEN));
   CARGoSLog LOGERR(std::cerr, SLogColor(ARGOS_LOG_ATTRIBUTE_BRIGHT, ARGOS_LOG_COLOR_RED));

}
