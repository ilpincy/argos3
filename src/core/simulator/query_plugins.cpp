/**
 * @file <argos3/core/simulator/query_plugins.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "query_plugins.h"
#include <argos3/core/simulator/visualization/visualization.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/simulator/actuator.h>
#include <argos3/core/simulator/sensor.h>

namespace argos {

   /****************************************/
   /****************************************/

   void QueryShowPluginDescription(const std::string& str_query) {
      LOG << "Plugins matching \"" << str_query << "\":" << std::endl << std::endl;
      /* Buffer to contain the search results */
      TQueryResult tResult;
      /* Search among the plugins */
      QuerySearchPlugins<CActuator>(str_query, tResult);
      QuerySearchPlugins<CSensor>(str_query, tResult);
      QuerySearchPlugins<CPhysicsEngine>(str_query, tResult);
      QuerySearchPlugins<CVisualization>(str_query, tResult);
      QuerySearchPlugins<CEntity>(str_query, tResult);
      /* Print the result */
      if(tResult.empty()) {
         LOG << "   None found." << std::endl << std::endl;
      }
      else {
         LOG << "==============================================================================" << std::endl << std::endl;
         for(UInt32 i = 0; i < tResult.size(); ++i) {
            LOG << "[ " << tResult[i].Label << " ] " << std::endl;
            LOG << tResult[i].BriefDescription << std::endl;
            LOG << "by " << tResult[i].Author << std::endl;
            LOG << "Status: " << tResult[i].Status << std::endl << std::endl;
            LOG << tResult[i].LongDescription << std::endl;
            LOG << "==============================================================================" << std::endl << std::endl;
         }
      }
   }

   /****************************************/
   /****************************************/

   void QueryPlugins(const std::string& str_query) {
      if(str_query == "actuators") {
         QueryShowList<CActuator>("AVAILABLE ACTUATORS");
      } else if(str_query == "sensors") {
         QueryShowList<CSensor>("AVAILABLE SENSORS");
      } else if(str_query == "physics_engines") {
         QueryShowList<CPhysicsEngine>("AVAILABLE PHYSICS ENGINES");
      } else if(str_query == "visualizations") {
         QueryShowList<CVisualization>("AVAILABLE VISUALIZATIONS");
      } else if(str_query == "entities") {
         QueryShowList<CEntity>("AVAILABLE ENTITIES");
      } else if(str_query == "all") {
         QueryShowList<CActuator>("AVAILABLE ACTUATORS");
         QueryShowList<CSensor>("AVAILABLE SENSORS");
         QueryShowList<CPhysicsEngine>("AVAILABLE PHYSICS ENGINES");
         QueryShowList<CVisualization>("AVAILABLE VISUALIZATIONS");
         QueryShowList<CEntity>("AVAILABLE ENTITIES");
      } else {
         QueryShowPluginDescription(str_query);
      }
   }

   /****************************************/
   /****************************************/

}
