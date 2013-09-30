#include "led_medium.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   CLEDMedium::CLEDMedium() {
   }

   /****************************************/
   /****************************************/

   CLEDMedium::~CLEDMedium() {
   }

   /****************************************/
   /****************************************/

   void CLEDMedium::Init(TConfigurationNode& t_tree) {
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
            CGrid<CLEDEntity>* pcGrid = new CGrid<CLEDEntity>(
               cArenaCenter - cArenaSize * 0.5f, cArenaCenter + cArenaSize * 0.5f,
               punGridSize[0], punGridSize[1], punGridSize[2]);
            m_pcLEDEntityGridUpdateOperation = new CLEDEntityGridUpdater(*pcGrid);
            pcGrid->SetUpdateEntityOperation(m_pcLEDEntityGridUpdateOperation);
            m_pcLEDEntityIndex = pcGrid;
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

   void CLEDMedium::PostSpaceInit() {
      Update();
   }

   /****************************************/
   /****************************************/

   void CLEDMedium::Reset() {
      m_pcLEDEntityIndex->Reset();
   }

   /****************************************/
   /****************************************/

   void CLEDMedium::Destroy() {
      delete m_pcLEDEntityIndex;
      if(m_pcLEDEntityGridUpdateOperation != NULL) {
         delete m_pcLEDEntityGridUpdateOperation;
      }
   }

   /****************************************/
   /****************************************/

   void CLEDMedium::Update() {
      m_pcLEDEntityIndex->Update();
   }

   /****************************************/
   /****************************************/

   void CLEDMedium::AddEntity(CLEDEntity& c_entity) {
      m_pcLEDEntityIndex->AddEntity(c_entity);
   }

   /****************************************/
   /****************************************/

   void CLEDMedium::RemoveEntity(CLEDEntity& c_entity) {
      m_pcLEDEntityIndex->RemoveEntity(c_entity);
   }

   /****************************************/
   /****************************************/

   REGISTER_MEDIUM(CLEDMedium,
                   "led",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "Manages the LEDs.",
                   "This medium is required to manage the LED entities, thus allowing the\n"
                   "associated camera sensors to work properly. If you intend to use a camera\n"
                   "sensor that detects colored blobs, you must add this medium to the XML\n"
                   "configuration file.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "<led id=\"led\" />\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None for the time being\n",
                   "Under development"
      );

   /****************************************/
   /****************************************/

}
