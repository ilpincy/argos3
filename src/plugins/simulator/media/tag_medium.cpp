/**
 * @file <argos3/plugins/simulator/media/tag_medium.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "tag_medium.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CTagMedium::Init(TConfigurationNode& t_tree) {
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
         /* Create the positional index for tag entities */
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
            CGrid<CTagEntity>* pcGrid = new CGrid<CTagEntity>(
               cArenaCenter - cArenaSize * 0.5f, cArenaCenter + cArenaSize * 0.5f,
               punGridSize[0], punGridSize[1], punGridSize[2]);
            m_pcTagEntityGridUpdateOperation = new CTagEntityGridUpdater(*pcGrid);
            pcGrid->SetUpdateEntityOperation(m_pcTagEntityGridUpdateOperation);
            m_pcTagEntityIndex = pcGrid;
         }
         else {
            THROW_ARGOSEXCEPTION("Unknown method \"" << strPosIndexMethod << "\" for the positional index.");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error in initialization of the tag medium", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CTagMedium::PostSpaceInit() {
      Update();
   }

   /****************************************/
   /****************************************/

   void CTagMedium::Reset() {
      m_pcTagEntityIndex->Reset();
   }

   /****************************************/
   /****************************************/

   void CTagMedium::Destroy() {
      delete m_pcTagEntityIndex;
      if(m_pcTagEntityGridUpdateOperation != nullptr) {
         delete m_pcTagEntityGridUpdateOperation;
      }
   }

   /****************************************/
   /****************************************/

   void CTagMedium::Update() {
      m_pcTagEntityIndex->Update();
   }

   /****************************************/
   /****************************************/

   void CTagMedium::AddEntity(CTagEntity& c_entity) {
      m_pcTagEntityIndex->AddEntity(c_entity);
   }

   /****************************************/
   /****************************************/

   void CTagMedium::RemoveEntity(CTagEntity& c_entity) {
      m_pcTagEntityIndex->RemoveEntity(c_entity);
   }

   /****************************************/
   /****************************************/

   REGISTER_MEDIUM(CTagMedium,
                   "tag",
                   "Michael Allwright [allsey87@gmail.com]",
                   "1.0",
                   "Manages tag entities.",
                   "This medium is required to manage the tag entities, so that the\n"
                   "associated camera sensors can find them. If you use a camera sensor, you\n"
                   "must add this medium the sensors XML configuration.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "<tag id=\"qrcodes\" />\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None for the time being\n",
                   "Under development"
      );

   /****************************************/
   /****************************************/

}
