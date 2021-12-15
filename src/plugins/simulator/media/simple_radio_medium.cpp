/**
 * @file <argos3/plugins/simulator/media/simple_radio_medium.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "simple_radio_medium.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CSimpleRadioMedium::Init(TConfigurationNode& t_tree) {
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
               punGridSize[0] = static_cast<size_t>(cArenaSize.GetX());
               punGridSize[1] = static_cast<size_t>(cArenaSize.GetY());
               punGridSize[2] = static_cast<size_t>(cArenaSize.GetZ());
            }
            else {
               std::string strPosGridSize;
               GetNodeAttribute(t_tree, "grid_size", strPosGridSize);
               ParseValues<size_t>(strPosGridSize, 3, punGridSize, ',');
            }
            CGrid<CSimpleRadioEntity>* pcGrid = new CGrid<CSimpleRadioEntity>(
               cArenaCenter - cArenaSize * 0.5f, cArenaCenter + cArenaSize * 0.5f,
               punGridSize[0], punGridSize[1], punGridSize[2]);
            m_pcEntityGridUpdateOperation = new CSimpleRadioEntityGridUpdater(*pcGrid);
            pcGrid->SetUpdateEntityOperation(m_pcEntityGridUpdateOperation);
            m_pcEntityIndex = pcGrid;
         }
         else {
            THROW_ARGOSEXCEPTION("Unknown method \"" << strPosIndexMethod << "\" for the positional index.");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error in initialization of the simple radio medium", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioMedium::PostSpaceInit() {
      Update();
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioMedium::Reset() {
      m_pcEntityIndex->Reset();
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioMedium::Destroy() {
      delete m_pcEntityIndex;
      if(m_pcEntityGridUpdateOperation != nullptr) {
         delete m_pcEntityGridUpdateOperation;
      }
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioMedium::Update() {
      /* Update the positional indices of the radios */
      m_pcEntityIndex->Update();
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioMedium::AddEntity(CSimpleRadioEntity& c_entity) {
      m_pcEntityIndex->AddEntity(c_entity);
      m_pcEntityIndex->Update();
   }

   /****************************************/
   /****************************************/

   void CSimpleRadioMedium::RemoveEntity(CSimpleRadioEntity& c_entity) {
      m_pcEntityIndex->RemoveEntity(c_entity);
      m_pcEntityIndex->Update();
   }

   /****************************************/
   /****************************************/

   REGISTER_MEDIUM(CSimpleRadioMedium,
                   "simple_radio",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "Enables communication between a simple radio actuator and a simple\n"
                   "radio sensor.",
                   "This medium indexes the simple radio entities in the space and allows\n"
                   "transmitting radios to find nearby receiving radios.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "<simple_radio id=\"simple_radios\" />\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None\n",
                   "Under development"
      );

   /****************************************/
   /****************************************/

}
