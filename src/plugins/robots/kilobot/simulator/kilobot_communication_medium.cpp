#include "kilobot_communication_medium.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/space/positional_indices/grid.h>
#include <argos3/core/utility/configuration/argos_exception.h>
#include <argos3/core/utility/logging/argos_log.h>

namespace argos {

   /****************************************/
   /****************************************/

   CKilobotCommunicationMedium::CKilobotCommunicationMedium() {
   }

   /****************************************/
   /****************************************/

   CKilobotCommunicationMedium::~CKilobotCommunicationMedium() {
   }

   /****************************************/
   /****************************************/

   void CKilobotCommunicationMedium::Init(TConfigurationNode& t_tree) {
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
         /* Create the positional index for embodied entities */
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
            CGrid<CKilobotCommunicationEntity>* pcGrid = new CGrid<CKilobotCommunicationEntity>(
               cArenaCenter - cArenaSize * 0.5f, cArenaCenter + cArenaSize * 0.5f,
               punGridSize[0], punGridSize[1], punGridSize[2]);
            m_pcGridUpdateOperation = new CKilobotCommunicationEntityGridEntityUpdater(*pcGrid);
            pcGrid->SetUpdateEntityOperation(m_pcGridUpdateOperation);
            m_pcKilobotIndex = pcGrid;
         }
         else {
            THROW_ARGOSEXCEPTION("Unknown method \"" << strPosIndexMethod << "\" for the positional index.");
         }
         /* Set probability of receiving a message */
         m_fRxProb = 0.0;
         GetNodeAttributeOrDefault(t_tree, "message_drop_prob", m_fRxProb, m_fRxProb);
         m_fRxProb = 1.0 - m_fRxProb;
         /* Create random number generator */
         m_pcRNG = CRandom::CreateRNG("argos");
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error in initialization of the range-and-bearing medium", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CKilobotCommunicationMedium::PostSpaceInit() {
      Update();
   }

   /****************************************/
   /****************************************/

   void CKilobotCommunicationMedium::Reset() {
      /* Reset positional index of Kilobot entities */
      m_pcKilobotIndex->Reset();
      /* Delete adjacency matrix */
      for(TAdjacencyMatrix::iterator it = m_tCommMatrix.begin();
          it != m_tCommMatrix.end();
          ++it) {
         it->second.clear();
      }
   }

   /****************************************/
   /****************************************/

   void CKilobotCommunicationMedium::Destroy() {
      delete m_pcKilobotIndex;
      if(m_pcGridUpdateOperation != NULL)
         delete m_pcGridUpdateOperation;
   }

   /****************************************/
   /****************************************/

   static UInt64 HashKilobotPair(const std::pair<CKilobotCommunicationEntity*, CKilobotCommunicationEntity*>& c_pair) {
      UInt64 unA = *reinterpret_cast<unsigned long long*>(c_pair.first) & 0xFFFFFFFF;
      UInt64 unB = *reinterpret_cast<unsigned long long*>(c_pair.second) & 0xFFFFFFFF;
      return (unA << 32) | unB;
   }

   void CKilobotCommunicationMedium::Update() {
      /*
       * Update positional index of Kilobot entities
       */
      m_pcKilobotIndex->Update();
      /*
       * Delete obsolete adjacency matrices
       */
      for(TAdjacencyMatrix::iterator it = m_tCommMatrix.begin();
          it != m_tCommMatrix.end();
          ++it) {
         it->second.clear();
      }
      m_tTxNeighbors.clear();
      /*
       * Construct the adjacency matrix of transmitting robots
       */
      /* Buffer for the communicating entities */
      CSet<CKilobotCommunicationEntity*> cOtherKilobots;
      /* This map contains the pairs that have already been checked */
      std::map<UInt64, std::pair<CKilobotCommunicationEntity*, CKilobotCommunicationEntity*> > mapPairsAlreadyChecked;
      /* Iterator for the above structure */
      std::map<UInt64, std::pair<CKilobotCommunicationEntity*, CKilobotCommunicationEntity*> >::iterator itPair;
      /* Used as test key */
      std::pair<CKilobotCommunicationEntity*, CKilobotCommunicationEntity*> cTestKey;
      /* Used as hash for the test key */
      UInt64 unTestHash;
      /* The distance between two Kilobots in line of sight */
      Real fDistance;
      /* Go through the Kilobot entities */
      for(TAdjacencyMatrix::iterator it = m_tCommMatrix.begin();
          it != m_tCommMatrix.end();
          ++it) {
         /* Get a reference to the current Kilobot entity */
         CKilobotCommunicationEntity& cKilobot = *(it->first);
         /* Is this robot trying to transmit? */
         if(cKilobot.GetTxStatus() == CKilobotCommunicationEntity::TX_ATTEMPT) {
            /* Yes, add it to the list of transmitting robots */
            m_tTxNeighbors[&cKilobot];
            /* Get the list of Kilobots in range */
            cOtherKilobots.clear();
            m_pcKilobotIndex->GetEntitiesAt(cOtherKilobots, cKilobot.GetPosition());
            /* Go through the Kilobots in range */
            for(CSet<CKilobotCommunicationEntity*>::iterator it2 = cOtherKilobots.begin();
                it2 != cOtherKilobots.end();
                ++it2) {
               /* Get a reference to the neighboring Kilobot */
               CKilobotCommunicationEntity& cOtherKilobot = **it2;
               /* First, make sure the entities are not the same and
                  that they are both transmitting */
               if(&cKilobot != &cOtherKilobot &&
                  cOtherKilobot.GetTxStatus() == CKilobotCommunicationEntity::TX_ATTEMPT) {
                  /* Proceed if the pair has not been checked already */
                  if(&cKilobot < &cOtherKilobot) {
                     cTestKey.first = &cKilobot;
                     cTestKey.second = &cOtherKilobot;
                  }
                  else {
                     cTestKey.first = &cOtherKilobot;
                     cTestKey.second = &cKilobot;
                  }
                  unTestHash = HashKilobotPair(cTestKey);
                  itPair = mapPairsAlreadyChecked.find(unTestHash);
                  if(itPair == mapPairsAlreadyChecked.end() ||   /* Pair does not exist */
                     itPair->second.first != cTestKey.first ||   /* Pair exists, but first Kilobot involved is different */
                     itPair->second.second != cTestKey.second) { /* Pair exists, but second Kilobot involved is different */
                     /* Mark this pair as already checked */
                     mapPairsAlreadyChecked[unTestHash] = cTestKey;
                     /* Calculate distance */
                     fDistance = Distance(cKilobot.GetPosition(),
                                          cOtherKilobot.GetPosition());
                     if(fDistance < cOtherKilobot.GetTxRange())
                        /* cKilobot receives cOtherKilobot's message */
                        m_tTxNeighbors[&cKilobot].insert(&cOtherKilobot);
                     if(fDistance < cKilobot.GetTxRange())
                        /* cOtherKilobot receives cKilobot's message */
                        m_tTxNeighbors[&cOtherKilobot].insert(&cKilobot);
                  } /* pair check */
               } /* entity identity + transmit check */
            } /* neighbors loop */
         } /* transmission check */
      } /* robot loop */
      /*
       * Go through transmitting robots and broadcast messages
       */
      /* The ray to use for occlusion checking */
      CRay3 cOcclusionCheckRay;
      /* Buffer to store the intersection data */
      SEmbodiedEntityIntersectionItem sIntersectionItem;
      /* Loop over transmitting robots */
      for(TAdjacencyMatrix::iterator it = m_tTxNeighbors.begin();
          it != m_tTxNeighbors.end();
          ++it) {
         /* Is this robot conflicting? */
         if(it->second.empty() ||
            m_pcRNG->Uniform(CRange<UInt32>(0, it->second.size() + 1)) == 0) {
            /* The robot can transmit */
            /* Get a reference to the current Kilobot entity */
            CKilobotCommunicationEntity& cKilobot = *(it->first);
            /* Change its transmission status */
            cKilobot.SetTxStatus(CKilobotCommunicationEntity::TX_SUCCESS);
            /* Go through its neighbors */
            cOtherKilobots.clear();
            m_pcKilobotIndex->GetEntitiesAt(cOtherKilobots, cKilobot.GetPosition());
            for(CSet<CKilobotCommunicationEntity*>::iterator it2 = cOtherKilobots.begin();
                it2 != cOtherKilobots.end();
                ++it2) {
               /* Get a reference to the neighboring Kilobot entity */
               CKilobotCommunicationEntity& cOtherKilobot = **it2;
               /* Make sure the robots are different */
               if(&cKilobot != &cOtherKilobot) {
                  /* Initialize the occlusion check ray start to the position of the robot */
                  cOcclusionCheckRay.SetStart(cKilobot.GetPosition());
                  /* Proceed if the two entities are not obstructed by another object */
                  cOcclusionCheckRay.SetEnd(cOtherKilobot.GetPosition());
                  if((!GetClosestEmbodiedEntityIntersectedByRay(sIntersectionItem,
                                                                cOcclusionCheckRay,
                                                                cKilobot.GetEntityBody())) ||
                     (&cOtherKilobot.GetEntityBody() == sIntersectionItem.IntersectedEntity)) {
                     /* If we get here, the two Kilobot entities are in direct line of sight */
                     /* Calculate distance */
                     fDistance = cOcclusionCheckRay.GetLength();
                     /* If robots are within transmission range and transmission succeeds... */
                     if(fDistance < cKilobot.GetTxRange() &&
                        m_pcRNG->Bernoulli(m_fRxProb)) {
                        /* cOtherKilobot receives cKilobot's message */
                        m_tCommMatrix[&cOtherKilobot].insert(&cKilobot);
                     }
                  } /* occlusion check */
               } /* identity check */
            } /* neighbor loop */
         } /* conflict check */
      } /* transmitters loop */
   }

   /****************************************/
   /****************************************/

   void CKilobotCommunicationMedium::AddEntity(CKilobotCommunicationEntity& c_entity) {
      m_tCommMatrix.insert(
         std::make_pair<CKilobotCommunicationEntity*, CSet<CKilobotCommunicationEntity*> >(
            &c_entity, CSet<CKilobotCommunicationEntity*>()));
      m_pcKilobotIndex->AddEntity(c_entity);
   }

   /****************************************/
   /****************************************/

   void CKilobotCommunicationMedium::RemoveEntity(CKilobotCommunicationEntity& c_entity) {
      m_pcKilobotIndex->RemoveEntity(c_entity);
      TAdjacencyMatrix::iterator it = m_tCommMatrix.find(&c_entity);
      if(it != m_tCommMatrix.end())
         m_tCommMatrix.erase(it);
      else
         THROW_ARGOSEXCEPTION("Can't erase entity \"" << c_entity.GetId() << "\" from Kilobot medium \"" << GetId() << "\"");
   }

   /****************************************/
   /****************************************/

   const CSet<CKilobotCommunicationEntity*>& CKilobotCommunicationMedium::GetKilobotsCommunicatingWith(CKilobotCommunicationEntity& c_entity) const {
      TAdjacencyMatrix::const_iterator it = m_tCommMatrix.find(&c_entity);
      if(it != m_tCommMatrix.end()) {
         return it->second;
      }
      else {
         THROW_ARGOSEXCEPTION("Kilobot entity \"" << c_entity.GetId() << "\" is not managed by the Kilobot medium \"" << GetId() << "\"");
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_MEDIUM(CKilobotCommunicationMedium,
                   "kilobot_communication",
                   "Carlo Pinciroli [ilpincy@gmail.com]",
                   "1.0",
                   "It simulates communication across Kilobot robots.",
                   "This medium is required to simulate communication across Kilobots. It works as\n"
                   "follows:\n"
                   "1. The medium calculates which robots can transmit at each time step. Every\n"
                   "   robot is assigned a non-transmission period. At the end of this period, the\n"
                   "   robot attempts transmission. It is successful with a probability that is\n"
                   "   inversely proportional to the number of transmitting robots in range. If\n"
                   "   successful, robot waits until the next period to transmit again. Otherwise\n"
                   "   it tries at the next time step.\n"
                   "2. It broadcasts the messages of the robots that can transmit. It is possible\n"
                   "   to specify the probability of message dropping by a robot as an optional\n"
                   "   parameter (see below).\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "<kilobot_communication id=\"kbc\" />\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "It is possible to specify the probability with which a robot a drops an incoming\n"
                   "message. This is done by setting the attribute \"message_drop_prob\". When set\n"
                   "to 0, no message is ever dropped; when set to 1, every message is dropped.\n\n"
                   "<kilobot_communication id=\"kbc\" message_drop_prob=\"0.25\" />\n"
                   ,
                   "Under development"
      );

   /****************************************/
   /****************************************/

}
