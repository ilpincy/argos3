/**
 * @file <argos3/core/simulator/query_plugins.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
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
      QuerySearchPlugins<CSimulatedActuator>(str_query, tResult);
      QuerySearchPlugins<CSimulatedSensor>  (str_query, tResult);
      QuerySearchPlugins<CPhysicsEngine>    (str_query, tResult);
      QuerySearchPlugins<CMedium>           (str_query, tResult);
      QuerySearchPlugins<CVisualization>    (str_query, tResult);
      QuerySearchPlugins<CEntity>           (str_query, tResult);
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
            LOG << "Version: " << tResult[i].Version << std::endl;
            LOG << "Status: " << tResult[i].Status << std::endl << std::endl;
            LOG << tResult[i].LongDescription << std::endl;
            LOG << std::endl;
            LOG << "==============================================================================" << std::endl << std::endl;
         }
      }
   }

   /****************************************/
   /****************************************/

   void QueryPlugins(const std::string& str_query) {
      bool bIsLogColored = LOG.IsColoredOutput();
      LOG.DisableColoredOutput();
      LOGERR.DisableColoredOutput();
      if(str_query == "actuators") {
         QueryShowList<CSimulatedActuator>("AVAILABLE ACTUATORS");
      } else if(str_query == "sensors") {
         QueryShowList<CSimulatedSensor>("AVAILABLE SENSORS");
      } else if(str_query == "physics_engines") {
         QueryShowList<CPhysicsEngine>("AVAILABLE PHYSICS ENGINES");
      } else if(str_query == "media") {
         QueryShowList<CMedium>("AVAILABLE MEDIA");
      } else if(str_query == "visualizations") {
         QueryShowList<CVisualization>("AVAILABLE VISUALIZATIONS");
      } else if(str_query == "entities") {
         QueryShowList<CEntity>("AVAILABLE ENTITIES");
      } else if(str_query == "all") {
         QueryShowList<CSimulatedActuator>("AVAILABLE ACTUATORS");
         QueryShowList<CSimulatedSensor>  ("AVAILABLE SENSORS");
         QueryShowList<CPhysicsEngine>    ("AVAILABLE PHYSICS ENGINES");
         QueryShowList<CMedium>           ("AVAILABLE MEDIA");
         QueryShowList<CVisualization>    ("AVAILABLE VISUALIZATIONS");
         QueryShowList<CEntity>           ("AVAILABLE ENTITIES");
      } else {
         QueryShowPluginDescription(str_query);
      }
      if(bIsLogColored) {
         LOG.EnableColoredOutput();
         LOGERR.EnableColoredOutput();
      }
   }

   /****************************************/
   /****************************************/

   /**
    * Get the plug-ins for a certain query in the vector.
    * Clears the vector.
    * @param str_query The given query to be searched.
    * @param vec_plugins The vector in which the results will be added.
    */
   void GetPlugins(const std::string& str_query, std::vector<std::string>& vec_plugins) 
   {
      /* Clear contents of vector */
      vec_plugins.clear();
      /* Add all plug-ins to the vector */
      if(str_query == "actuators") {
         GetQueryList<CSimulatedActuator>(vec_plugins);
      } else if(str_query == "sensors") {
         GetQueryList<CSimulatedSensor>(vec_plugins);
      } else if(str_query == "physics_engines") {
         GetQueryList<CPhysicsEngine>(vec_plugins);
      } else if(str_query == "media") {
         GetQueryList<CMedium>(vec_plugins);
      } else if(str_query == "visualizations") {
         GetQueryList<CVisualization>(vec_plugins);
      } else if(str_query == "entities") {
         GetQueryList<CEntity>(vec_plugins);
      } else if(str_query == "all") {
         GetQueryList<CSimulatedActuator>(vec_plugins);
         GetQueryList<CSimulatedSensor>  (vec_plugins);
         GetQueryList<CPhysicsEngine>    (vec_plugins);
         GetQueryList<CMedium>           (vec_plugins);
         GetQueryList<CVisualization>    (vec_plugins);
         GetQueryList<CEntity>           (vec_plugins);
      } else {
         QueryShowPluginDescription(str_query);
      }
   }

   /****************************************/
   /****************************************/

}
