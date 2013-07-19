/**
 * @file <argos3/core/simulator/medium/medium.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "medium.h"
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CMedium::Init(TConfigurationNode& t_tree) {
      try {
         /* Get id from the XML */
         GetNodeAttribute(t_tree, "id", m_strId);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing a medium entity", ex);
      }
   }
   
   /****************************************/
   /****************************************/

}
