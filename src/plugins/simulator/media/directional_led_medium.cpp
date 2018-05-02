/**
 * @file <argos3/plugins/simulator/media/directional_led_medium.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "directional_led_medium.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CDirectionalLEDMedium::Init(TConfigurationNode& t_tree) {
      try {
         CMedium::Init(t_tree);
         /* Get the positional index method */
         std::string strPosIndexMethod("grid");
         GetNodeAttributeOrDefault(t_tree, "index", strPosIndexMethod, strPosIndexMethod);
         /* Get the arena center and size */
         CVector3 cArenaCenter;
         CVector3 cArenaSize;
         TConfigurationNode& tArena = GetNode(CSimulator::GetInstance().GetConfigurationRoot(), "arena");
         GetNodeAttribute(tArena, "size", cArenaSize);
         GetNodeAttributeOrDefault(tArena, "center", cArenaCenter, cArenaCenter);
         /* Create the positional index for LED entities */
         if(strPosIndexMethod == "grid") {
            size_t punGridSize[3];
            if(!NodeAttributeExists(t_tree, "grid_size")) {
               punGridSize[0] = cArenaSize.GetX();
               punGridSize[1] = cArenaSize.GetY();
               punGridSize[2] = cArenaSize.GetZ();
            }
            else {
               std::string strPosGridSize;
               GetNodeAttribute(t_tree, "grid_size", strPosGridSize);
               ParseValues<size_t>(strPosGridSize, 3, punGridSize, ',');
            }
            CGrid<CDirectionalLEDEntity>* pcGrid = new CGrid<CDirectionalLEDEntity>(
               cArenaCenter - cArenaSize * 0.5f, cArenaCenter + cArenaSize * 0.5f,
               punGridSize[0], punGridSize[1], punGridSize[2]);
            m_pcDirectionalLEDEntityGridUpdateOperation = new CDirectionalLEDEntityGridUpdater(*pcGrid);
            pcGrid->SetUpdateEntityOperation(m_pcDirectionalLEDEntityGridUpdateOperation);
            m_pcDirectionalLEDEntityIndex = pcGrid;
         }
         else {
            THROW_ARGOSEXCEPTION("Unknown method \"" << strPosIndexMethod << "\" for the positional index.");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error in initialization of the LED medium", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDMedium::PostSpaceInit() {
      Update();
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDMedium::Reset() {
      m_pcDirectionalLEDEntityIndex->Reset();
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDMedium::Destroy() {
      delete m_pcDirectionalLEDEntityIndex;
      if(m_pcDirectionalLEDEntityGridUpdateOperation != nullptr) {
         delete m_pcDirectionalLEDEntityGridUpdateOperation;
      }
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDMedium::Update() {
      m_pcDirectionalLEDEntityIndex->Update();
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDMedium::AddEntity(CDirectionalLEDEntity& c_entity) {
      m_pcDirectionalLEDEntityIndex->AddEntity(c_entity);
   }

   /****************************************/
   /****************************************/

   void CDirectionalLEDMedium::RemoveEntity(CDirectionalLEDEntity& c_entity) {
      m_pcDirectionalLEDEntityIndex->RemoveEntity(c_entity);
   }

   /****************************************/
   /****************************************/

   REGISTER_MEDIUM(CDirectionalLEDMedium,
                   "directional_led",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "Manages directional LED entities.",
                   "This medium is required to manage the directional LED entities, so that\n"
                   "the associated camera sensors can find them. If you use a camera sensor,\n"
                   "you must add this medium the sensors XML configuration.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "<directional_led id=\"led\" />\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None for the time being\n",
                   "Under development"
      );

   /****************************************/
   /****************************************/

}
