#include "rab_medium.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   template<class ENTITY>
   class CDummyIndex : public CPositionalIndex<ENTITY> {
   public:
      typedef typename CPositionalIndex<ENTITY>::COperation CEntityOperation;
   public:
      CDummyIndex() {}
      
      virtual ~CDummyIndex() {}
      
      virtual void Init(TConfigurationNode& t_tree) {}
      
      virtual void Reset() {}
      
      virtual void Destroy() {}

      virtual void AddEntity(ENTITY& c_entity) {
         m_cEntities.insert(&c_entity);
      }

      virtual void RemoveEntity(ENTITY& c_entity) {
         m_cEntities.erase(&c_entity);
      }

      virtual void Update() {}

      virtual void GetEntitiesAt(CSet<ENTITY*,SEntityComparator>& c_entities,
                                 const CVector3& c_position) const {
         c_entities = m_cEntities;
      }

      virtual void ForAllEntities(CEntityOperation& c_operation) {
         for(typename CSet<ENTITY*,SEntityComparator>::iterator it = m_cEntities.begin();
             it != m_cEntities.end() && c_operation(**it);
             ++it);
      }

      virtual void ForEntitiesInSphereRange(const CVector3& c_center,
                                            Real f_radius,
                                            CEntityOperation& c_operation) {}

      virtual void ForEntitiesInBoxRange(const CVector3& c_center,
                                         const CVector3& c_half_size,
                                         CEntityOperation& c_operation) {}

      virtual void ForEntitiesInCircleRange(const CVector3& c_center,
                                            Real f_radius,
                                            CEntityOperation& c_operation) {}

      virtual void ForEntitiesInRectangleRange(const CVector3& c_center,
                                               const CVector2& c_half_size,
                                               CEntityOperation& c_operation) {}

      virtual void ForEntitiesAlongRay(const CRay3& c_ray,
                                       CEntityOperation& c_operation,
                                       bool b_stop_at_closest_match = false) {}

   protected:

      CSet<ENTITY*,SEntityComparator> m_cEntities;
   };

/****************************************/
/****************************************/

   CRABMedium::CRABMedium() :
      m_bCheckOcclusions(true) {
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
         /* Check occlusions? */
         GetNodeAttributeOrDefault(t_tree, "check_occlusions", m_bCheckOcclusions, m_bCheckOcclusions);
         /* Get the positional index method */
         std::string strPosIndexMethod("grid");
         GetNodeAttributeOrDefault(t_tree, "index", strPosIndexMethod, strPosIndexMethod);
         /* Get the arena center and size */
         CVector3 cArenaCenter;
         CVector3 cArenaSize;
         TConfigurationNode& tArena = GetNode(CSimulator::GetInstance().GetConfigurationRoot(), "arena");
         GetNodeAttribute(tArena, "size", cArenaSize);
         GetNodeAttributeOrDefault(tArena, "center", cArenaCenter, cArenaCenter);
         /* Create the positional index for embodied entities */
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
            CGrid<CRABEquippedEntity>* pcGrid = new CGrid<CRABEquippedEntity>(
               cArenaCenter - cArenaSize * 0.5f, cArenaCenter + cArenaSize * 0.5f,
               punGridSize[0], punGridSize[1], punGridSize[2]);
            m_pcRABEquippedEntityGridUpdateOperation = new CRABEquippedEntityGridEntityUpdater(*pcGrid);
            pcGrid->SetUpdateEntityOperation(m_pcRABEquippedEntityGridUpdateOperation);
            m_pcRABEquippedEntityIndex = pcGrid;
            LOG << "[INFO] RAB medium \""
                << GetId()
                << "\" using the grid index with size <"
                << punGridSize[0] << "," << punGridSize[1] << "," << punGridSize[2] << ">"
                << std::endl;
         }
         else if(strPosIndexMethod == "dummy") {
            m_pcRABEquippedEntityGridUpdateOperation = nullptr;
            m_pcRABEquippedEntityIndex = new CDummyIndex<CRABEquippedEntity>();
            LOG << "[INFO] RAB medium \""
                << GetId()
                << "\" using the dummy index"
                << std::endl;
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

   void CRABMedium::PostSpaceInit() {
      Update();
   }

/****************************************/
/****************************************/

   void CRABMedium::Reset() {
      /* Reset positional index of RAB entities */
      m_pcRABEquippedEntityIndex->Reset();
      /* Delete routing table */
      for(TRoutingTable::iterator it = m_tRoutingTable.begin();
          it != m_tRoutingTable.end();
          ++it) {
         it->second.clear();
      }
   }

/****************************************/
/****************************************/

   void CRABMedium::Destroy() {
      delete m_pcRABEquippedEntityIndex;
      if(m_pcRABEquippedEntityGridUpdateOperation != nullptr) {
         delete m_pcRABEquippedEntityGridUpdateOperation;
      }
   }

/****************************************/
/****************************************/

   static size_t HashRABPair(const std::pair<CRABEquippedEntity*, CRABEquippedEntity*>& c_pair) {
      return
         reinterpret_cast<size_t>(c_pair.first) ^
         reinterpret_cast<size_t>(c_pair.second);
   }

   void CRABMedium::Update() {
      /* Update positional index of RAB entities */
      m_pcRABEquippedEntityIndex->Update();
      /* Delete routing table */
      for(TRoutingTable::iterator it = m_tRoutingTable.begin();
          it != m_tRoutingTable.end();
          ++it) {
         it->second.clear();
      }
      /* This map contains the pairs that have already been checked */
      unordered_map<ssize_t, std::pair<CRABEquippedEntity*, CRABEquippedEntity*> > mapPairsAlreadyChecked;
      /* Iterator for the above structure */
      unordered_map<ssize_t, std::pair<CRABEquippedEntity*, CRABEquippedEntity*> >::iterator itPair;
      /* Used as test key */
      std::pair<CRABEquippedEntity*, CRABEquippedEntity*> cTestKey;
      /* Used as hash for the test key */
      UInt64 unTestHash;
      /* The ray to use for occlusion checking */
      CRay3 cOcclusionCheckRay;
      /* Buffer for the communicating entities */
      CSet<CRABEquippedEntity*,SEntityComparator> cOtherRABs;
      /* Buffer to store the intersection data */
      SEmbodiedEntityIntersectionItem sIntersectionItem;
      /* The distance between two RABs in line of sight */
      Real fDistance;
      /* Go through the RAB entities */
      for(TRoutingTable::iterator it = m_tRoutingTable.begin();
          it != m_tRoutingTable.end();
          ++it) {
         /* Get a reference to the current RAB entity */
         CRABEquippedEntity& cRAB = *reinterpret_cast<CRABEquippedEntity*>(GetSpace().GetEntityVector()[it->first]);
         /* Initialize the occlusion check ray start to the position of the robot */
         cOcclusionCheckRay.SetStart(cRAB.GetPosition());
         /* For each RAB entity, get the list of RAB entities in range */
         cOtherRABs.clear();
         try {
            m_pcRABEquippedEntityIndex->GetEntitiesAt(cOtherRABs, cRAB.GetPosition());
         }
         catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("When checking RAB \"" << cRAB.GetContext() << cRAB.GetId() << "\"", ex);
         }
         /* Go through the RAB entities in range */
         for(CSet<CRABEquippedEntity*>::iterator it2 = cOtherRABs.begin();
             it2 != cOtherRABs.end();
             ++it2) {
            /* Get a reference to the RAB entity */
            CRABEquippedEntity& cOtherRAB = **it2;
            /* First, make sure the entities are not the same */
            if(&cRAB != &cOtherRAB) {
               /* Proceed if the pair has not been checked already */
               if(&cRAB < &cOtherRAB) {
                  cTestKey.first = &cRAB;
                  cTestKey.second = &cOtherRAB;
               }
               else {
                  cTestKey.first = &cOtherRAB;
                  cTestKey.second = &cRAB;
               }
               unTestHash = HashRABPair(cTestKey);
               itPair = mapPairsAlreadyChecked.find(unTestHash);
               if(itPair == mapPairsAlreadyChecked.end() ||   /* Pair does not exist */
                  itPair->second.first != cTestKey.first ||   /* Pair exists, but first RAB involved is different */
                  itPair->second.second != cTestKey.second) { /* Pair exists, but second RAB involved is different */
                  /* Mark this pair as already checked */
                  mapPairsAlreadyChecked[unTestHash] = cTestKey;
                  /* Proceed if the message size is compatible */
                  if(cRAB.GetMsgSize() == cOtherRAB.GetMsgSize()) {
                     /* Proceed if the two entities are not obstructed by another object */
                     cOcclusionCheckRay.SetEnd(cOtherRAB.GetPosition());
                     if((!m_bCheckOcclusions) ||
                        (!GetClosestEmbodiedEntityIntersectedByRay(sIntersectionItem,
                                                                   cOcclusionCheckRay,
                                                                   cRAB.GetEntityBody())) ||
                        (&cOtherRAB.GetEntityBody() == sIntersectionItem.IntersectedEntity)) {
                        /* If we get here, the two RAB entities are in direct line of sight */
                        /* cRAB can receive cOtherRAB's message if it is in range, and viceversa */
                        /* Calculate square distance */
                        fDistance = cOcclusionCheckRay.GetLength();
                        if(fDistance < cOtherRAB.GetRange()) {
                           /* cRAB receives cOtherRAB's message */
                           m_tRoutingTable[cRAB.GetIndex()].insert(&cOtherRAB);
                        }
                        if(fDistance < cRAB.GetRange()) {
                           /* cOtherRAB receives cRAB's message */
                           m_tRoutingTable[cOtherRAB.GetIndex()].insert(&cRAB);
                        }
                     } // occlusion found?
                  } // is msg size the same?
               } // is check necessary?
            } // are entities the same?
         } // for entities in range
      } // for routing table
   }

/****************************************/
/****************************************/

   void CRABMedium::AddEntity(CRABEquippedEntity& c_entity) {
      m_tRoutingTable.insert(
         std::make_pair<ssize_t, CSet<CRABEquippedEntity*,SEntityComparator> >(
            c_entity.GetIndex(), CSet<CRABEquippedEntity*,SEntityComparator>()));
      m_pcRABEquippedEntityIndex->AddEntity(c_entity);
      m_pcRABEquippedEntityIndex->Update();
   }

/****************************************/
/****************************************/

   void CRABMedium::RemoveEntity(CRABEquippedEntity& c_entity) {
      m_pcRABEquippedEntityIndex->RemoveEntity(c_entity);
      m_pcRABEquippedEntityIndex->Update();
      TRoutingTable::iterator it = m_tRoutingTable.find(c_entity.GetIndex());
      if(it != m_tRoutingTable.end())
         m_tRoutingTable.erase(it);
   }

/****************************************/
/****************************************/

   const CSet<CRABEquippedEntity*,SEntityComparator>& CRABMedium::GetRABsCommunicatingWith(CRABEquippedEntity& c_entity) const {
      TRoutingTable::const_iterator it = m_tRoutingTable.find(c_entity.GetIndex());
      if(it != m_tRoutingTable.end()) {
         return it->second;
      }
      else {
         THROW_ARGOSEXCEPTION("RAB entity \"" << c_entity.GetContext() << c_entity.GetId() << "\" is not managed by the RAB medium \"" << GetId() << "\"");
      }
   }

/****************************************/
/****************************************/

   REGISTER_MEDIUM(CRABMedium,
                   "range_and_bearing",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "It simulates the communication across range-and-bearing-equipped robots.",
                   "This medium is required to simulate communication across range-and-bearing-\n"
                   "equipped robots. You need to add it to the <media> section every time you add\n"
                   "a range-and-bearing-equipped entity whose controller has a range-and-bearing\n"
                   "device activated.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "<range_and_bearing id=\"rab\" />\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "By default, the RAB medium requires two robots to be in direct line-of-sight in\n"
                   "order to be able to exchange messages. You can toggle this behavior on or off\n"
                   "through the 'check_occlusions' attribute:\n\n"
                   "<range_and_bearing id=\"rab\" check_occlusions=\"false\" />\n\n"
                   "An important factor in simulation efficiency is how the robots are indexed in\n"
                   "space, because that impacts how fast neighbor queries can be completed. You can\n"
                   "control this aspect with the \"index\" attribute. The default is to use the\n"
                   "\"grid\" index, which superimposes a grid onto the arena. The size of the grid\n"
                   "cells is calculated automatically from the size of the arena, defaulting at\n"
                   "1m per side. The optimal choice for your experiment is usually to set this size\n"
                   "to the diameter of the most common robot in your simulation. You can override\n"
                   "the defaults by using this configuration:\n\n"
                   "<range_and_bearing id=\"rab\" index=\"grid\" grid_size=\"20, 10, 5\" />\n\n"
                   "The example shows how to set a 20x10x5 grid. Imagine that the arena size is\n"
                   "<10,10,1>: then, the size of a cell would be <10/20, 10/10, 1/5> = <.5, 1, .2>.\n\n"
                   "It makes sense to pay the cost of a grid when you have a lot of robots. If you\n"
                   "only have a handful of robots, you might want to avoid grid management\n"
                   "altogether. In that case, give the \"dummy\" index a try:\n\n"
                   "<range_and_bearing id=\"rab\" index=\"dummy\" />\n\n"
                   "This index has O(N^2) complexity (where N is the number of robots), but for\n"
                   "small experiments it's actually faster than using the grid.\n",
                   "Usable"
      );

/****************************************/
/****************************************/

}
