#include "rab_medium.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <set>

namespace argos {

   /****************************************/
   /****************************************/

   CRABMedium::CRABMedium() {
   }

   /****************************************/
   /****************************************/

   CRABMedium::~CRABMedium() {
   }

   /****************************************/
   /****************************************/

   void CRABMedium::Init(TConfigurationNode& t_tree) {
      try {
         CMedium::Init(t_tree);
         /* Get the positional index method */
         std::string strPosIndexMethod("grid");
         GetNodeAttributeOrDefault(t_tree, "index", strPosIndexMethod, strPosIndexMethod);
         /* Create the positional index for embodied entities */
         if(strPosIndexMethod == "grid") {
            std::string strPosGridSize;
            GetNodeAttribute(t_tree, "grid_size", strPosGridSize);
            size_t punGridSize[3];
            ParseValues<size_t>(strPosGridSize, 3, punGridSize, ',');
            CVector3 cArenaCenter = CSimulator::GetInstance().GetSpace().GetArenaCenter();
            CVector3 cArenaHalfSize = CSimulator::GetInstance().GetSpace().GetArenaSize() * 0.5f;
            CGrid<CRABEquippedEntity>* pcGrid = new CGrid<CRABEquippedEntity>(
               cArenaCenter - cArenaHalfSize, cArenaCenter + cArenaHalfSize,
               punGridSize[0], punGridSize[1], punGridSize[2]);
            m_pcRABEquippedEntityGridUpdateOperation = new CRABEquippedEntityGridEntityUpdater(*pcGrid);
            pcGrid->SetUpdateEntityOperation(m_pcRABEquippedEntityGridUpdateOperation);
            m_pcRABEquippedEntityIndex = pcGrid;
         }
         else {
            THROW_ARGOSEXCEPTION("Unknown method \"" << strPosIndexMethod << "\" for the positional index.");
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error in initialization of the range-and-bearing medium", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CRABMedium::Reset() {
      LOGERR << "[DEBUG] CRABMedium::Reset()" << std::endl;
   }

   /****************************************/
   /****************************************/

   void CRABMedium::Destroy() {
      LOGERR << "[DEBUG] CRABMedium::Destroy()" << std::endl;
      delete m_pcRABEquippedEntityIndex;
      if(m_pcRABEquippedEntityGridUpdateOperation != NULL) {
         delete m_pcRABEquippedEntityGridUpdateOperation;
      }
   }

   /****************************************/
   /****************************************/

   void CRABMedium::Update() {
      LOGERR << "[DEBUG] CRABMedium::Update()" << std::endl;
      /* Update positional index of RAB entities */
      m_pcRABEquippedEntityIndex->Update();
      /* Delete routing table */
      for(TRoutingTable::iterator it = m_tRoutingTable.begin();
          it != m_tRoutingTable.end();
          ++it) {
         it->second.clear();
      }
      /* This map contains the pairs that have already been checked */
      std::set<std::pair<CRABEquippedEntity*, CRABEquippedEntity*> > setPairsAlreadyChecked;
      /* Used as test key */
      std::pair<CRABEquippedEntity*, CRABEquippedEntity*> cTestKey;
      CRay3 cOcclusionCheckRay;
      /* Buffer for the communicating entities */
      CSet<CRABEquippedEntity*> cOtherRABs;
      /* Buffer to store the intersection data */
      SEmbodiedEntityIntersectionItem sIntersectionItem;
      /* The distance between two RABs in line of sight */
      Real fDistance;
      /* Go through the RAB entities */
      for(TRoutingTable::iterator it = m_tRoutingTable.begin();
          it != m_tRoutingTable.end();
          ++it) {
         /* Get a reference to the current RAB entity */
         CRABEquippedEntity& cRAB = *(it->first);
         /* Initialize the occlusion check ray start to the position of the robot */
         cOcclusionCheckRay.SetStart(cRAB.GetPosition());
         /* For each RAB entity, get the list of RAB entities in range */
         cOtherRABs.clear();
         m_pcRABEquippedEntityIndex->GetEntitiesAt(cOtherRABs, cRAB.GetPosition());
         /* Go through the RAB entities in range */
         for(CSet<CRABEquippedEntity*>::iterator it2 = cOtherRABs.begin();
             it2 != cOtherRABs.end();
             ++it2) {
            /* Get a reference to the RAB entity */
            CRABEquippedEntity& cOtherRAB = **it2;
            /* First, make sure the entities are not the same */
            if(&cRAB != &cOtherRAB) {
               /* Proceed if the pair has not been checked already */
               if(&cRAB < &cOtherRAB) cTestKey = std::make_pair(&cRAB, &cOtherRAB);
               else cTestKey = std::make_pair(&cOtherRAB, &cRAB);
               if(setPairsAlreadyChecked.find(cTestKey) == setPairsAlreadyChecked.end()) {
                  /* Proceed if the message size is compatible */
                  if(cRAB.GetMsgSize() == cOtherRAB.GetMsgSize()) {
                     /* Proceed if the two entities are not obstructed by another object */
                     cOcclusionCheckRay.SetEnd(cOtherRAB.GetPosition());
                     if(GetClosestEmbodiedEntityIntersectedByRay(sIntersectionItem,
                                                                 CSimulator::GetInstance().GetSpace().GetEmbodiedEntityIndex(),
                                                                 cOcclusionCheckRay,
                                                                 cRAB.GetReference())  ||
                        (&cOtherRAB.GetParent() == &sIntersectionItem.IntersectedEntity->GetParent())) {
                        /* If we get here, the two RAB entities are in direct line of sight */
                        /* Mark this pair as already checked */
                        setPairsAlreadyChecked.insert(cTestKey);
                        /* cRAB can receive cOtherRAB's message if it is in range, and viceversa */
                        /* Calculate square distance */
                        fDistance = cOcclusionCheckRay.GetLength();
                        if(fDistance < cOtherRAB.GetRange()) {
                           /* cRAB receives cOtherRAB's message */
                           it->second.push_back(&cOtherRAB);
                        }
                        if(fDistance < cRAB.GetRange()) {
                           /* cOtherRAB receives cRAB's message */
                           m_tRoutingTable[&cOtherRAB].push_back(&cRAB);
                        }
                     }
                  }
               }
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   void CRABMedium::AddEntity(CRABEquippedEntity& c_entity) {
      LOGERR << "[DEBUG] Added entity \""
             << c_entity.GetId()
             << "\""
             << std::endl;
      m_tRoutingTable.insert(
         std::make_pair<CRABEquippedEntity*, std::vector<CRABEquippedEntity*> >(
            &c_entity, std::vector<CRABEquippedEntity*>()));
   }

   /****************************************/
   /****************************************/

   const std::vector<CRABEquippedEntity*>& CRABMedium::GetRABsCommunicatingWith(CRABEquippedEntity& c_entity) const {
      TRoutingTable::const_iterator it = m_tRoutingTable.find(&c_entity);
      if(it != m_tRoutingTable.end()) {
         return it->second;
      }
      else {
         THROW_ARGOSEXCEPTION("RAB entity \"" << c_entity.GetId() << "\" is not managed by the RAB medium \"" << GetId() << "\"");
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_MEDIUM(CRABMedium,
                   "range_and_bearing",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "It simulates the communication across range-and-bearing-equipped robots.",
                   "This entity is required to simulate communication across range-and-bearing-\n"
                   "equipped robots. You need to add it to the <media> section every time you add\n"
                   "a range-and-bearing-equipped entity whose controller has a range-and-bearing\n"
                   "device activated.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "<range_and_bearing id=\"rab\" />\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None for the time being\n",
                   "Under development"
      );

   /****************************************/
   /****************************************/

}
