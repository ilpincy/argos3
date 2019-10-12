/**
 * @file <argos3/core/simulator/space/space.cpp>
 *
 * @brief This file provides the implementation of the space.
 *
 * This file provides the implementation of the space.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <argos3/core/utility/string_utilities.h>
#include <argos3/core/utility/math/range.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/positional_entity.h>
#include <argos3/core/simulator/loop_functions.h>
#include <cstring>
#include "space.h"

namespace argos {

   /****************************************/
   /****************************************/

   CSpace::CSpace() :
      m_cSimulator(CSimulator::GetInstance()),
      m_unSimulationClock(0),
      m_pcFloorEntity(NULL),
      m_ptPhysicsEngines(NULL),
      m_ptMedia(NULL) {}

   /****************************************/
   /****************************************/

   void CSpace::Init(TConfigurationNode& t_tree) {
      /* Get reference to physics engine and media vectors */
      m_ptPhysicsEngines = &(m_cSimulator.GetPhysicsEngines());
      m_ptMedia = &(m_cSimulator.GetMedia());
      /* Get the arena center and size */
      GetNodeAttributeOrDefault(t_tree, "center", m_cArenaCenter, m_cArenaCenter);
      GetNodeAttribute(t_tree, "size", m_cArenaSize);
      m_cArenaLimits.Set(m_cArenaCenter - m_cArenaSize / 2.0f,
                         m_cArenaCenter + m_cArenaSize / 2.0f);
      /*
       * Add and initialize all entities in XML
       */
      /* Start from the entities placed manually */
      TConfigurationNodeIterator itArenaItem;
      for(itArenaItem = itArenaItem.begin(&t_tree);
          itArenaItem != itArenaItem.end();
          ++itArenaItem) {
         if(itArenaItem->Value() != "distribute") {
            CEntity* pcEntity = CFactory<CEntity>::New(itArenaItem->Value());
            pcEntity->Init(*itArenaItem);
            CallEntityOperation<CSpaceOperationAddEntity, CSpace, void>(*this, *pcEntity);
         }
      }
      /* Place the entities to distribute automatically */
      for(itArenaItem = itArenaItem.begin(&t_tree);
          itArenaItem != itArenaItem.end();
          ++itArenaItem) {
         if(itArenaItem->Value() == "distribute") {
            Distribute(*itArenaItem);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CSpace::Reset() {
      /* Reset the simulation clock */
      m_unSimulationClock = 0;
      /* Reset the entities */
      for(UInt32 i = 0; i < m_vecEntities.size(); ++i) {
         m_vecEntities[i]->Reset();
      }
   }

   /****************************************/
   /****************************************/

   void CSpace::Destroy() {
      /* Remove all entities */
      while(!m_vecRootEntities.empty()) {
         CallEntityOperation<CSpaceOperationRemoveEntity, CSpace, void>(*this, *m_vecRootEntities.back());
      }
   }

   /****************************************/
   /****************************************/

   void CSpace::GetEntitiesMatching(CEntity::TVector& t_buffer,
                                    const std::string& str_pattern) {
      for(CEntity::TVector::iterator it = m_vecEntities.begin();
          it != m_vecEntities.end(); ++it) {
         if(MatchPattern((*it)->GetId(), str_pattern)) {
            t_buffer.push_back(*it);
         }
      }
   }

   /****************************************/
   /****************************************/

   CSpace::TMapPerType& CSpace::GetEntitiesByTypeImpl(const std::string& str_type) const {
     TMapPerTypePerId::const_iterator itEntities = m_mapEntitiesPerTypePerId.find(str_type);
     if(itEntities != m_mapEntitiesPerTypePerId.end()){
       return const_cast<CSpace::TMapPerType&>(itEntities->second);
     } else {
       THROW_ARGOSEXCEPTION("Entity map for type \"" << str_type << "\" not found.");
     }
   }
   /****************************************/
   /****************************************/

   void CSpace::Update() {
      /* Increase the simulation clock */
      IncreaseSimulationClock();
      /* Perform the 'act' phase for controllable entities */
      UpdateControllableEntitiesAct();
      /* Update the physics engines */
      UpdatePhysics();
      /* Update media */
      UpdateMedia();
      /* Call loop functions */
      m_cSimulator.GetLoopFunctions().PreStep();
      /* Perform the 'sense+step' phase for controllable entities */
      UpdateControllableEntitiesSenseStep();
      /* Call loop functions */
      m_cSimulator.GetLoopFunctions().PostStep();
      /* Flush logs */
      LOG.Flush();
      LOGERR.Flush();
   }

   /****************************************/
   /****************************************/

   void CSpace::AddControllableEntity(CControllableEntity& c_entity) {
      m_vecControllableEntities.push_back(&c_entity);
   }

   /****************************************/
   /****************************************/

   void CSpace::RemoveControllableEntity(CControllableEntity& c_entity) {
      CControllableEntity::TVector::iterator it = find(m_vecControllableEntities.begin(),
                                                       m_vecControllableEntities.end(),
                                                       &c_entity);
      if(it != m_vecControllableEntities.end()) {
         m_vecControllableEntities.erase(it);
      }
   }

   /****************************************/
   /****************************************/

   void CSpace::AddEntityToPhysicsEngine(CEmbodiedEntity& c_entity) {
      /* Get a reference to the root entity */
      CEntity* pcToAdd = &c_entity.GetRootEntity();
      /* Get a reference to the position of the entity */
      const CVector3& cPos = c_entity.GetOriginAnchor().Position;
      /* Go through engines and check which ones could house the entity */
      CPhysicsEngine::TVector vecPotentialEngines;
      for(size_t i = 0; i < m_ptPhysicsEngines->size(); ++i) {
         if((*m_ptPhysicsEngines)[i]->IsPointContained(cPos)) {
            vecPotentialEngines.push_back((*m_ptPhysicsEngines)[i]);
         }
      }
      /* If no engine can house the entity, bomb out */
      if(vecPotentialEngines.empty()) {
         THROW_ARGOSEXCEPTION("No physics engines available to house entity \"" << pcToAdd->GetId() << "\"@(" << cPos << ").");
      }
      /* If the entity is not movable, add the entity to all the matching engines */
      if(! c_entity.IsMovable()) {
         bool bAdded = false;
         for(size_t i = 0; i < vecPotentialEngines.size(); ++i) {
            bAdded |= vecPotentialEngines[i]->AddEntity(*pcToAdd);
         }
         if(!bAdded) {
            std::ostringstream ossMsg;
            ossMsg << "None of the matching physics engines (";
            ossMsg << "\"" << vecPotentialEngines[0]->GetId() << "\"";
            for(size_t i = 1; i < vecPotentialEngines.size(); ++i) {
               ossMsg << ",\"" << vecPotentialEngines[i]->GetId() << "\"";
            }
            ossMsg << ") can house non-movable entity \"" << pcToAdd->GetId() << "\"@(" << cPos << ").";
            THROW_ARGOSEXCEPTION(ossMsg.str());
         }
      }
      /* If the entity is movable, only one engine can be associated to the embodied entity */
      else if(vecPotentialEngines.size() == 1) {
         /* Only one engine matches, bingo! */
         if(!vecPotentialEngines[0]->AddEntity(*pcToAdd)) {
            THROW_ARGOSEXCEPTION("The matching physics engine (\"" << vecPotentialEngines[0]->GetId() << "\"), cannot house movable entity \"" << pcToAdd->GetId()  << "\"@(" << cPos << ").");
         }
      }
      else {
         /* More than one engine matches, pick the first that can manage the entity */
         for(size_t i = 0; i < vecPotentialEngines.size(); ++i) {
            if(vecPotentialEngines[i]->AddEntity(*pcToAdd)) return;
         }
         /* No engine can house the entity */
         std::ostringstream ossMsg;
         ossMsg << "None of the matching physics engines (";
         ossMsg << "\"" << vecPotentialEngines[0]->GetId() << "\"";
         for(size_t i = 1; i < vecPotentialEngines.size(); ++i) {
            ossMsg << ",\"" << vecPotentialEngines[i]->GetId() << "\"";
         }
         ossMsg << ") can house movable entity \"" << pcToAdd->GetId() << "\"@(" << cPos << ").";
         THROW_ARGOSEXCEPTION(ossMsg.str());
      }
   }

   /****************************************/
   /****************************************/

   class RealNumberGenerator {
   public:
      virtual ~RealNumberGenerator() {}
      virtual CVector3 operator()(bool b_is_retry) = 0;
   };

   class ConstantGenerator : public RealNumberGenerator {
   public:
      ConstantGenerator(const CVector3& c_value) :
         m_cValue(c_value) {}

      inline virtual CVector3 operator()(bool b_is_retry) {
         return m_cValue;
      }
   private:
      CVector3 m_cValue;

   };

   class UniformGenerator : public RealNumberGenerator {
   public:
      UniformGenerator(const CVector3& c_min,
                       const CVector3& c_max) :
         m_cMin(c_min),
         m_cMax(c_max) {}
      inline virtual CVector3 operator()(bool b_is_retry) {
         Real fRandX =
            m_cMax.GetX() > m_cMin.GetX() ?
            CSimulator::GetInstance().GetRNG()->Uniform(CRange<Real>(m_cMin.GetX(), m_cMax.GetX())) :
            m_cMax.GetX();
         Real fRandY =
            m_cMax.GetY() > m_cMin.GetY() ?
            CSimulator::GetInstance().GetRNG()->Uniform(CRange<Real>(m_cMin.GetY(), m_cMax.GetY())) :
            m_cMax.GetY();
         Real fRandZ =
            m_cMax.GetZ() > m_cMin.GetZ() ?
            CSimulator::GetInstance().GetRNG()->Uniform(CRange<Real>(m_cMin.GetZ(), m_cMax.GetZ())) :
            m_cMax.GetZ();
         return CVector3(fRandX, fRandY, fRandZ);
      }
   private:
      CVector3 m_cMin;
      CVector3 m_cMax;
   };

   class GaussianGenerator : public RealNumberGenerator {
   public:
      GaussianGenerator(const CVector3& c_mean,
                        const CVector3& c_std_dev) :
         m_cMean(c_mean),
         m_cStdDev(c_std_dev) {}
      inline virtual CVector3 operator()(bool b_is_retry) {
         return CVector3(CSimulator::GetInstance().GetRNG()->Gaussian(m_cStdDev.GetX(), m_cMean.GetX()),
                         CSimulator::GetInstance().GetRNG()->Gaussian(m_cStdDev.GetY(), m_cMean.GetY()),
                         CSimulator::GetInstance().GetRNG()->Gaussian(m_cStdDev.GetZ(), m_cMean.GetZ()));
      }
   private:
      CVector3 m_cMean;
      CVector3 m_cStdDev;
   };

   class GridGenerator : public RealNumberGenerator {
   public:
      GridGenerator(const CVector3 c_center,
                    const UInt32 un_layout[],
                    const CVector3 c_distances):
         m_cCenter(c_center),
         m_cDistances(c_distances),
         m_unNumEntityPlaced(0) {
         m_unLayout[0] = un_layout[0];
         m_unLayout[1] = un_layout[1];
         m_unLayout[2] = un_layout[2];
         /* Check if layout is sane */
         if( m_unLayout[0] == 0 || m_unLayout[1] == 0 || m_unLayout[2] == 0 ) {
            THROW_ARGOSEXCEPTION("'layout' values (distribute position, method 'grid') must all be different than 0");
         }
      }

      virtual CVector3 operator()(bool b_is_retry) {
         if(b_is_retry) {
            THROW_ARGOSEXCEPTION("Impossible to place entity #" << m_unNumEntityPlaced << " in grid");
         }
         CVector3 cReturn;
         if(m_unNumEntityPlaced < m_unLayout[0] * m_unLayout[1] * m_unLayout[2]) {
            cReturn.SetX(m_cCenter.GetX() + ( m_unLayout[0] - 1 ) * m_cDistances.GetX() * 0.5 - ( m_unNumEntityPlaced  % m_unLayout[0] ) * m_cDistances.GetX());
            cReturn.SetY(m_cCenter.GetY() + ( m_unLayout[1] - 1 ) * m_cDistances.GetY() * 0.5 - ( m_unNumEntityPlaced  / m_unLayout[0] ) % m_unLayout[1] * m_cDistances.GetY());
            cReturn.SetZ(m_cCenter.GetZ() + ( m_unLayout[2] - 1 ) * m_cDistances.GetZ() * 0.5 - ( m_unNumEntityPlaced / ( m_unLayout[0] * m_unLayout[1] ) ) * m_cDistances.GetZ());
            ++m_unNumEntityPlaced;
         }
         else {
            THROW_ARGOSEXCEPTION("Distribute position, method 'grid': trying to place more entities than allowed "
                                 "by the 'layout', check your 'quantity' tag");
         }
         return cReturn;
      }

   private:
      CVector3 m_cCenter;
      UInt32 m_unLayout[3];
      CVector3 m_cDistances;
      UInt32 m_unNumEntityPlaced;
   };

   /****************************************/
   /****************************************/

   RealNumberGenerator* CreateGenerator(TConfigurationNode& t_tree) {
      std::string strMethod;
      GetNodeAttribute(t_tree, "method", strMethod);
      if(strMethod == "uniform") {
         CVector3 cMin, cMax;
         GetNodeAttribute(t_tree, "min", cMin);
         GetNodeAttribute(t_tree, "max", cMax);
         if(! (cMin <= cMax)) {
            THROW_ARGOSEXCEPTION("Uniform generator: the min is not less than or equal to max: " << cMin << " / " << cMax);
         }
         return new UniformGenerator(cMin, cMax);
      }
      else if(strMethod == "gaussian") {
         CVector3 cMean, cStdDev;
         GetNodeAttribute(t_tree, "mean", cMean);
         GetNodeAttribute(t_tree, "std_dev", cStdDev);
         return new GaussianGenerator(cMean, cStdDev);
      }
      else if(strMethod == "constant") {
         CVector3 cValues;
         GetNodeAttribute(t_tree, "values", cValues);
         return new ConstantGenerator(cValues);
      }
      else if(strMethod == "grid") {
         CVector3 cCenter,cDistances;
         GetNodeAttribute(t_tree, "center", cCenter);
         GetNodeAttribute(t_tree, "distances", cDistances);
         UInt32 unLayout[3];
         std::string strLayout;
         GetNodeAttribute(t_tree, "layout", strLayout);
         ParseValues<UInt32> (strLayout, 3, unLayout, ',');
         return new GridGenerator(cCenter, unLayout, cDistances);
      }
      else {
         THROW_ARGOSEXCEPTION("Unknown distribution method \"" << strMethod << "\"");
      }
   }

   /****************************************/
   /****************************************/

   static CEmbodiedEntity* GetEmbodiedEntity(CEntity* pc_entity) {
      /* Is the entity embodied itself? */
      CEmbodiedEntity* pcEmbodiedTest = dynamic_cast<CEmbodiedEntity*>(pc_entity);
      if(pcEmbodiedTest != NULL) {
         return pcEmbodiedTest;
      }
      /* Is the entity composable with an embodied component? */
      CComposableEntity* pcComposableTest = dynamic_cast<CComposableEntity*>(pc_entity);
      if(pcComposableTest != NULL) {
         if(pcComposableTest->HasComponent("body")) {
            return &(pcComposableTest->GetComponent<CEmbodiedEntity>("body"));
         }
      }
      /* No embodied entity found */
      return NULL;
   }

   /****************************************/
   /****************************************/

   static CPositionalEntity* GetPositionalEntity(CEntity* pc_entity) {
      /* Is the entity positional itself? */
      CPositionalEntity* pcPositionalTest = dynamic_cast<CPositionalEntity*>(pc_entity);
      if(pcPositionalTest != NULL) {
         return pcPositionalTest;
      }
      /* Is the entity composable with a positional component? */
      CComposableEntity* pcComposableTest = dynamic_cast<CComposableEntity*>(pc_entity);
      if(pcComposableTest != NULL) {
         if(pcComposableTest->HasComponent("position")) {
            return &(pcComposableTest->GetComponent<CPositionalEntity>("position"));
         }
      }
      /* No positional entity found */
      return NULL;
   }

   /****************************************/
   /****************************************/

   void CSpace::Distribute(TConfigurationNode& t_tree) {
      try {
         /* Get the needed nodes */
         TConfigurationNode cPositionNode;
         cPositionNode = GetNode(t_tree, "position");
         TConfigurationNode cOrientationNode;
         cOrientationNode = GetNode(t_tree, "orientation");
         TConfigurationNode cEntityNode;
         cEntityNode = GetNode(t_tree, "entity");
         /* Create the real number generators */
         RealNumberGenerator* pcPositionGenerator = CreateGenerator(cPositionNode);
         RealNumberGenerator* pcOrientationGenerator = CreateGenerator(cOrientationNode);
         /* How many entities? */
         UInt32 unQuantity;
         GetNodeAttribute(cEntityNode, "quantity", unQuantity);
         /* How many trials before failing? */
         UInt32 unMaxTrials;
         GetNodeAttribute(cEntityNode, "max_trials", unMaxTrials);
         /* Get the (optional) entity base numbering */
         UInt64 unBaseNum = 0;
         GetNodeAttributeOrDefault(cEntityNode, "base_num", unBaseNum, unBaseNum);
         /* Get the entity type to add (take only the first, ignore additional if any) */
         TConfigurationNodeIterator itEntity;
         itEntity = itEntity.begin(&cEntityNode);
         if(itEntity == itEntity.end()) {
            THROW_ARGOSEXCEPTION("No entity to distribute specified.");
         }
         /* Get the entity base ID */
         std::string strBaseId;
         GetNodeAttribute(*itEntity, "id", strBaseId);
         /* Add the requested entities */
         for(UInt32 i = 0; i < unQuantity; ++i) {
            /* Copy the entity XML tree */
            TConfigurationNode& tEntityTree = *itEntity;
            /* Set progressive ID */
            SetNodeAttribute(tEntityTree, "id", strBaseId + ToString(i+unBaseNum));
            /* Go on until the entity is placed with no collisions or
               the max number of trials has been exceeded */
            UInt32 unTrials = 0;
            bool bDone = false;
            bool bRetry = false;
            CEntity* pcEntity;
            do {
               /* Create entity */
               pcEntity = CFactory<CEntity>::New(tEntityTree.Value());
               /*
                * Now that you have the entity, check whether the
                * entity is positional or embodied or has one such
                * component.
                *
                * In case the entity is positional but not embodied,
                * there's no need to check for collisions.
                *
                * In case the entity is embodied, we must check for
                * collisions.
                *
                * To check for collisions, we add the entity in the
                * place where it's supposed to be, then we ask the
                * engine if that entity is colliding with something.
                *
                * In case of collision, we remove the entity and try a
                * different position/orientation.
                */
               /* Check whether the entity is positional */
               CPositionalEntity* pcPositionalEntity = GetPositionalEntity(pcEntity);
               if(pcPositionalEntity != NULL) {
                  /* Set the position */
                  SetNodeAttribute(tEntityTree, "position", (*pcPositionGenerator)(bRetry));
                  /* Set the orientation */
                  SetNodeAttribute(tEntityTree, "orientation", (*pcOrientationGenerator)(bRetry));
                  /* Init the entity (this also creates the components, if pcEntity is a composable) */
                  pcEntity->Init(tEntityTree);
                  /* Wherever we want to put the entity, it's OK, add it */
                  CallEntityOperation<CSpaceOperationAddEntity, CSpace, void>(*this, *pcEntity);
                  bDone = true;
               }
               else {
                  /* Assume the entity is embodied */
                  /* If the tree does not have a 'body' node, create a new one */
                  if(!NodeExists(tEntityTree, "body")) {
                     TConfigurationNode tBodyNode("body");
                     AddChildNode(tEntityTree, tBodyNode);
                  }
                  /* Get 'body' node */
                  TConfigurationNode& tBodyNode = GetNode(tEntityTree, "body");
                  /* Set the position */
                  SetNodeAttribute(tBodyNode, "position", (*pcPositionGenerator)(bRetry));
                  /* Set the orientation */
                  SetNodeAttribute(tBodyNode, "orientation", (*pcOrientationGenerator)(bRetry));
                  /* Init the entity (this also creates the components, if pcEntity is a composable) */
                  pcEntity->Init(tEntityTree);
                  /* Check whether the entity is indeed embodied */
                  CEmbodiedEntity* pcEmbodiedEntity = GetEmbodiedEntity(pcEntity);
                  if(pcEmbodiedEntity != NULL) {
                     /* Yes, the entity is embodied */
                     /* Add it to the space and to the designated physics engine */
                     CallEntityOperation<CSpaceOperationAddEntity, CSpace, void>(*this, *pcEntity);
                     /* Check if it's colliding with anything else */
                     if(pcEmbodiedEntity->IsCollidingWithSomething()) {
                        /* Set retry to true */
                        bRetry = true;
                        /* Get rid of the entity */
                        CallEntityOperation<CSpaceOperationRemoveEntity, CSpace, void>(*this, *pcEntity);
                        /* Increase the trial count */
                        ++unTrials;
                        /* Too many trials? */
                        if(unTrials > unMaxTrials) {
                           /* Yes, bomb out */
                           THROW_ARGOSEXCEPTION("Exceeded max trials when trying to distribute objects of type " <<
                                                tEntityTree.Value() << " with base id \"" <<
                                                strBaseId << "\". I managed to place only " << i << " objects.");
                        }
                        /* Retry with a new position */
                     }
                     else {
                        /* No collision, we're done with this entity */
                        bDone = true;
                     }
                  }
                  else {
                     THROW_ARGOSEXCEPTION("Cannot distribute entities that are not positional nor embodied, and \"" << tEntityTree.Value() << "\" is neither.");
                  }
               }
            }
            while(!bDone);
         }
         /* Delete the generators, now unneeded */
         delete pcPositionGenerator;
         delete pcOrientationGenerator;
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error while trying to distribute entities", ex);
      }
   }

   /****************************************/
   /****************************************/

}
