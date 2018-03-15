/**
 * @file <argos3/plugins/simulator/media/radio_medium.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "radio_medium.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CRadioMedium::Init(TConfigurationNode& t_tree) {
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
         /* Create the positional index for Radio entities */
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
            CGrid<CRadioEntity>* pcGrid = new CGrid<CRadioEntity>(
               cArenaCenter - cArenaSize * 0.5f, cArenaCenter + cArenaSize * 0.5f,
               punGridSize[0], punGridSize[1], punGridSize[2]);
            m_pcRadioEntityGridUpdateOperation = new CRadioEntityGridUpdater(*pcGrid);
            pcGrid->SetUpdateEntityOperation(m_pcRadioEntityGridUpdateOperation);
            m_pcRadioEntityIndex = pcGrid;
         }
         else {
            THROW_ARGOSEXCEPTION("Unknown method \"" << strPosIndexMethod << "\" for the positional index.");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error in initialization of the radio medium", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CRadioMedium::PostSpaceInit() {
      Update();
   }

   /****************************************/
   /****************************************/

   void CRadioMedium::Reset() {
      m_pcRadioEntityIndex->Reset();
   }

   /****************************************/
   /****************************************/

   void CRadioMedium::Destroy() {
      delete m_pcRadioEntityIndex;
      if(m_pcRadioEntityGridUpdateOperation != nullptr) {
         delete m_pcRadioEntityGridUpdateOperation;
      }
   }

   /****************************************/
   /****************************************/

   void CRadioMedium::Update() {
      /* Update the positional indices of the radios */
      m_pcRadioEntityIndex->Update();
   }

   /****************************************/
   /****************************************/

   void CRadioMedium::AddEntity(CRadioEntity& c_entity) {
      m_pcRadioEntityIndex->AddEntity(c_entity);
   }

   /****************************************/
   /****************************************/

   void CRadioMedium::RemoveEntity(CRadioEntity& c_entity) {
      m_pcRadioEntityIndex->RemoveEntity(c_entity);
   }

   /****************************************/
   /****************************************/

   REGISTER_MEDIUM(CRadioMedium,
                   "radio",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "Enables communication between a radio actuator and radio sensor.",
                   "This medium indexes the radio entities in the space and allows\n"
                   "transmitting radios to find nearby receiving radios.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "<radio id=\"radios\" />\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None for the time being\n",
                   "Under development"
      );

   /****************************************/
   /****************************************/

}
