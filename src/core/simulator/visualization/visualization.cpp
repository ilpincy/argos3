/**
 * @file core/simulator/visualization/visualization.cpp
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include <argos3/core/simulator/visualization/visualization.h>
#include <argos3/core/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CVisualization::Init(TConfigurationNode& t_tree) {
      /* Get visualization id from the XML */
      GetNodeAttribute(t_tree, "id", m_strId);
   }

   /****************************************/
   /****************************************/

   void CVisualization::Execute() {
      /* Main cycle */
      while(!m_cSimulator.IsExperimentFinished()) {
         m_cSimulator.UpdateSpace();
      }
   }

   /****************************************/
   /****************************************/

}
