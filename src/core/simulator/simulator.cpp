/**
 * @file argos3/core/simulator/simulator.cpp
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "simulator.h"

#include <iostream>
#include <string>
#include <sys/time.h>
#include <argos3/core/simulator/space/space_no_threads.h>
#include <argos3/core/simulator/space/space_multi_thread_scatter_gather.h>
#include <argos3/core/simulator/space/space_multi_thread_h_dispatch.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/profiler/profiler.h>
#include <argos3/core/simulator/visualization/visualization.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/core/utility/string_utilities.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/simulator/loop_functions.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/physics_engine/physics_engine_entity.h>

const std::string CONFIGURATION_CLOCKTICK = "clocktick";
const std::string CONFIGURATION_MAXCLOCK = "maxclock";
const std::string CONFIGURATION_RANDOMSEED = "random_seed";
const std::string CONFIGURATION_CONTROLLER_PATH = "controller_path";
const std::string CONFIGURATION_MAPPING_ENGINE_ID = "id";
const std::string CONFIGURATION_MAPPING_ENTITY_ID = "id";

namespace argos {

   /****************************************/
   /****************************************/

   CSimulator::CSimulator() :
      m_pcVisualization(NULL),
      m_pcSpace(NULL),
      m_pcLoopFunctions(NULL),
      m_unMaxSimulationClock(0),
      m_bWasRandomSeedSet(false),
      m_unThreads(0),
      m_pcProfiler(NULL),
      m_bHumanReadableProfile(true) {}

   /****************************************/
   /****************************************/

   CSimulator::~CSimulator() {
      if(IsProfiling()) {
         delete m_pcProfiler;
      }
      /* Delete the visualization */
      if(m_pcVisualization != NULL) delete m_pcVisualization;
      /* Delete all the physics engines */
      for(CPhysicsEngine::TMap::iterator it = m_mapPhysicsEngines.begin(); it
             != m_mapPhysicsEngines.end(); ++it)
         delete it->second;
      m_mapPhysicsEngines.clear();
      m_vecPhysicsEngines.clear();
      /* Delete the space and the dynamic linking manager */
      if(m_pcSpace != NULL) {
         delete m_pcSpace;
      }
   }

   /****************************************/
   /****************************************/

   CSimulator& CSimulator::GetInstance() {
      static std::auto_ptr<CSimulator> pcSimulatorInstance(new CSimulator());
      return *(pcSimulatorInstance.get());
   }

   /****************************************/
   /****************************************/

   void CSimulator::LoadExperiment() {
      /* Build configuration tree */
      m_tConfiguration.LoadFile(m_strExperimentConfigFileName);
      m_tConfigurationRoot = *m_tConfiguration.FirstChildElement();
      /* Init the experiment */
      Init();
      LOG.Flush();
      LOGERR.Flush();
   }

   /****************************************/
   /****************************************/

   void CSimulator::Init() {
      /* General configuration */
      InitFramework(GetNode(m_tConfigurationRoot, "framework"));

      /* TODO: MANAGE CREATION OF LOOP FUNCTIONS */

      /* Space */
      InitSpace(GetNode(m_tConfigurationRoot, "arena"));

      /* Physics engines */
      InitPhysics(GetNode(m_tConfigurationRoot, "physics_engines"));

      /* Initialise the mapping between physics engines and entities */
      InitPhysicsEntitiesMapping(m_tConfigurationRoot);

      /* Call user init function */
      m_pcLoopFunctions->Init(GetNode(m_tConfigurationRoot, "loop_functions"));

      /* Initialise visualization */
      InitVisualization(GetNode(m_tConfigurationRoot, "visualization"));

      /* Start profiling, if needed */
      if(IsProfiling()) {
         m_pcProfiler->Start();
      }
   }

   /****************************************/
   /****************************************/

   void CSimulator::Reset() {
      /* if random seed is 0 or is not specified, init with the current timeval */
      if(m_bWasRandomSeedSet) {
         CRandom::SetSeedOf("argos", m_unRandomSeed);
      }
      else {
         /* Prepare the default value based on the current clock time */
         struct timeval sTimeValue;
         ::gettimeofday(&sTimeValue, NULL);
         UInt32 unSeed = static_cast<UInt32>(sTimeValue.tv_usec);
         CRandom::SetSeedOf("argos", unSeed);
         m_unRandomSeed = unSeed;
         LOG << "[INFO] Using random seed = " << m_unRandomSeed << std::endl;
      }
      CRandom::GetCategory("argos").ResetRNGs();

      /* Reset the space */
      m_pcSpace->Reset();

      /* Reset the physics engines */
      for(CPhysicsEngine::TMap::iterator it = m_mapPhysicsEngines.begin();
          it != m_mapPhysicsEngines.end(); ++it) {
         it->second->Reset();
      }

      /* Reset the loop functions */
      m_pcLoopFunctions->Reset();

      LOG.Flush();
      LOGERR.Flush();
   }

   /****************************************/
   /****************************************/

   void CSimulator::Destroy() {
      /* Call user destroy function */
      if (m_pcLoopFunctions != NULL) {
         m_pcLoopFunctions->Destroy();
         delete m_pcLoopFunctions;
         m_pcLoopFunctions = NULL;
      }

      /* Destroy the visualization */
      if(m_pcVisualization != NULL) m_pcVisualization->Destroy();

      /* Close physics engines */
      for(CPhysicsEngine::TMap::iterator it = m_mapPhysicsEngines.begin();
          it != m_mapPhysicsEngines.end(); ++it) {
         it->second->Destroy();
         delete it->second;
      }
      m_mapPhysicsEngines.clear();
      m_vecPhysicsEngines.clear();

      /* Destroy simulated space */
      if(m_pcSpace != NULL) {
         m_pcSpace->Destroy();
      }

      /* Get rid of ARGoS category */
      if(CRandom::ExistsCategory("argos")) {
         CRandom::RemoveCategory("argos");
      }

      /* Stop profiling and flush the data */
      if(IsProfiling()) {
         m_pcProfiler->Stop();
         m_pcProfiler->Flush(m_bHumanReadableProfile);
      }

      LOG.Flush();
      LOGERR.Flush();
   }

   /****************************************/
   /****************************************/

   void CSimulator::Execute() {
      m_pcVisualization->Execute();
   }

   /****************************************/
   /****************************************/

   void CSimulator::UpdateSpace() {
      /* Increase the simulation clock */
      m_pcSpace->IncreaseSimulationClock();
      /* Call loop functions */
      m_pcLoopFunctions->PrePhysicsEngineStep();
      LOG.Flush();
      LOGERR.Flush();
      /* Update the space */
      m_pcSpace->Update();
      LOG.Flush();
      LOGERR.Flush();
      /* Call loop functions */
      m_pcLoopFunctions->PostPhysicsEngineStep();
      LOG.Flush();
      LOGERR.Flush();
   }

   /****************************************/
   /****************************************/

   bool CSimulator::IsExperimentFinished() const {
      /*
        The experiment is considered finished when the simulation clock exceeds
        the maximum value set in the XML, or when one of the visualisations asks
        to terminate.
      */

      /* Check simulation clock */
      if (m_unMaxSimulationClock > 0 &&
          m_pcSpace->GetSimulationClock() >= m_unMaxSimulationClock) {
         return true;
      }

      /* Call loop function */
      return m_pcLoopFunctions->IsExperimentFinished();
   }

   /****************************************/
   /****************************************/

   void CSimulator::InitFramework(TConfigurationNode& t_tree) {
      try {
         /* Parse the 'system' node */
         if(NodeExists(t_tree, "system")) {
            TConfigurationNode tSystem;
            tSystem = GetNode(t_tree, "system");
            GetNodeAttributeOrDefault(tSystem, "threads", m_unThreads, m_unThreads);
            if(m_unThreads == 0) {
               LOG << "[INFO] Not using threads" << std::endl;
               m_pcSpace = new CSpaceNoThreads;
            }
            else {
               LOG << "[INFO] Using " << m_unThreads << " parallel threads" << std::endl;
               std::string strThreadingMethod = "scatter-gather";
               GetNodeAttributeOrDefault(tSystem, "method", strThreadingMethod, strThreadingMethod);
               if(strThreadingMethod == "scatter-gather") {
                  LOG << "[INFO] Using threading method \"scatter-gather\"" << std::endl;
                  m_pcSpace = new CSpaceMultiThreadScatterGather;
               }
               else if(strThreadingMethod == "h-dispatch") {
                  LOG << "[INFO] Using threading method \"h-dispatch\"" << std::endl;
                  m_pcSpace = new CSpaceMultiThreadHDispatch;
               }
               else {
                  THROW_ARGOSEXCEPTION("Error parsing the <system> tag. Unknown threading method \"" << strThreadingMethod << "\". Available methods: \"scatter-gather\" and \"h-dispatch\".");
               }
            }
         }
         else {
            LOG << "[INFO] Not using threads" << std::endl;
            m_pcSpace = new CSpaceNoThreads;
         }

         /* Get 'experiment' node */
         TConfigurationNode tExperiment;
         tExperiment = GetNode(t_tree, "experiment");

         /* Parse random seed */
         /* Buffer to hold the random seed */
         GetNodeAttributeOrDefault(tExperiment,
                                   "random_seed",
                                   m_unRandomSeed,
                                   static_cast<UInt32>(0));

         /* if random seed is 0 or is not specified, init with the current timeval */
         if(m_unRandomSeed != 0) {
            CRandom::CreateCategory("argos", m_unRandomSeed);
            LOG << "[INFO] Using random seed = " << m_unRandomSeed << std::endl;
            m_bWasRandomSeedSet = true;
         }
         else {
            /* Prepare the default value based on the current clock time */
            m_bWasRandomSeedSet = false;
            struct timeval sTimeValue;
            ::gettimeofday(&sTimeValue, NULL);
            UInt32 unSeed = static_cast<UInt32>(sTimeValue.tv_usec);
            m_unRandomSeed = unSeed;
            CRandom::CreateCategory("argos", unSeed);
            LOG << "[INFO] Using random seed = " << unSeed << std::endl;
         }
         m_pcRNG = CRandom::CreateRNG("argos");

         /* Set the simulation clock tick length */
         UInt32 unTicksPerSec;
         GetNodeAttribute(tExperiment,
                          "ticks_per_second",
                          unTicksPerSec);
         CPhysicsEngine::SetSimulationClockTick(1.0 / static_cast<Real>(unTicksPerSec));

         /* Set the maximum simulation ticks (in ticks) */
         Real fExpLength;
         GetNodeAttributeOrDefault<Real>(tExperiment,
                                         "length",
                                         fExpLength,
                                         0.0f);
         m_unMaxSimulationClock = fExpLength * unTicksPerSec;
         LOG << "[INFO] Total experiment length in clock ticks = "
             << (m_unMaxSimulationClock ? ToString(m_unMaxSimulationClock) : "unlimited")
             << std::endl;

         /* Get the profiling tag, if present */
         if(NodeExists(t_tree, "profiling")) {
            TConfigurationNode& tProfiling = GetNode(t_tree, "profiling");
            std::string strFile;
            GetNodeAttribute(tProfiling, "file", strFile);
            std::string strFormat;
            GetNodeAttribute(tProfiling, "format", strFormat);
            if(strFormat == "human_readable") {
               m_bHumanReadableProfile = true;
            }
            else if(strFormat == "table") {
               m_bHumanReadableProfile = false;
            }
            else {
               THROW_ARGOSEXCEPTION("Unrecognized profile format \"" << strFormat << "\". Accepted values are \"human_readable\" and \"table\".");
            }
            bool bTrunc = true;
            GetNodeAttributeOrDefault(tProfiling, "truncate_file", bTrunc, bTrunc);
            m_pcProfiler = new CProfiler(strFile, bTrunc);
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize the simulator. Parse error inside the <framework> tag.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CSimulator::InitSpace(TConfigurationNode& t_tree) {
      try {
         m_pcSpace->Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize the space. Error in the <arena> subtree.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CSimulator::InitPhysics(TConfigurationNode& t_tree) {
      try {
         /* Cycle through the physics engines */
         TConfigurationNodeIterator itEngines;
         for(itEngines = itEngines.begin(&t_tree);
             itEngines != itEngines.end();
             ++itEngines) {
            /* Create the physics engine */
            CPhysicsEngine* pcEngine = CFactory<CPhysicsEngine>::New(itEngines->Value());
            try {
               /* Initialize the engine */
               pcEngine->Init(*itEngines);
               /* Check that an engine with that ID does not exist yet */
               if(m_mapPhysicsEngines.find(pcEngine->GetId()) == m_mapPhysicsEngines.end()) {
                  /* Add it to the lists */
                  m_mapPhysicsEngines[pcEngine->GetId()] = pcEngine;
                  m_vecPhysicsEngines.push_back(pcEngine);
               }
               else {
                  /* Duplicate id -> error */
                  THROW_ARGOSEXCEPTION("A physics engine with id \"" << pcEngine->GetId() << "\" exists already. The ids must be unique!");
               }
            }
            catch(CARGoSException& ex) {
               /* Error while executing engine init, destroy what done to prevent memory leaks */
               pcEngine->Destroy();
               delete pcEngine;
               THROW_ARGOSEXCEPTION_NESTED("Error initializing physics engine type \"" << itEngines->Value() << "\"", ex);
            }
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize the physics engines. Parse error in the <physics_engines> subtree.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CSimulator::InitVisualization(TConfigurationNode& t_tree) {
      try {
         /* Consider only the first visualization */
         TConfigurationNodeIterator itVisualization;
         itVisualization = itVisualization.begin(&t_tree);
         if(itVisualization != NULL) {
            /* Create the visualization */
            m_pcVisualization = CFactory<CVisualization>::New(itVisualization->Value());
            /* Initialize the visualization */
            m_pcVisualization->Init(*itVisualization);
         }
         else {
            LOGERR << "[WARNING] No visualization selected." << std::endl;
            m_pcVisualization = new CVisualization();
         }
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize the visualization. Parse error in the <visualization> subtree.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CSimulator::InitPhysicsEntitiesMapping(TConfigurationNode& t_tree) {
      /* Get the base node for the mappings */
      TConfigurationNode tArenaPhysics;
      tArenaPhysics = GetNode(t_tree, "arena_physics");
      /* Cycle through the engines */
      TConfigurationNodeIterator itEngines;
      std::string strEngineId;
      for(itEngines = itEngines.begin(&tArenaPhysics);
          itEngines != itEngines.end();
          ++itEngines) {
         /* Get the id of the engine */
         GetNodeAttribute(*itEngines, "id", strEngineId);
         /* Get a reference to the respective physics engine object */
         CPhysicsEngine::TMap::iterator itEngineMap = m_mapPhysicsEngines.find(strEngineId);
         if (itEngineMap == m_mapPhysicsEngines.end()) {
            THROW_ARGOSEXCEPTION("Unknown physics engine id \"" << strEngineId << "\" specified in the arena_physics section.");
         }
         CPhysicsEngine* pcEngine = itEngineMap->second;
         /* Cycle through the entities associated to this engine and add them to it */
         TConfigurationNodeIterator itEntities;
         std::string strEntityId;
         TEntityVector tMatchingEntities;
         for(itEntities = itEntities.begin(&(*itEngines));
             itEntities != itEntities.end();
             ++itEntities) {
            /* Clear the matching entities */
            tMatchingEntities.clear();
            /* Get the id of the entity */
            GetNodeAttribute(*itEntities, "id", strEntityId);
            /* Get a reference to the entities to add */
            m_pcSpace->GetEntitiesMatching(tMatchingEntities, "^" + strEntityId + "$");
            if(tMatchingEntities.size() == 0) {
               THROW_ARGOSEXCEPTION("No entity matches the regular expression \"" << strEntityId << "\"");
            }
            /* Go through the matching entities */
            for(TEntityVector::iterator itMatchingEntities = tMatchingEntities.begin();
                itMatchingEntities != tMatchingEntities.end(); ++itMatchingEntities) {
               CEntity& cEntity = **itMatchingEntities;
               /* Add the entity to the engine */
               /*
               LOG << "[INFO] Adding entity \""
                   << cEntity.GetId()
                   << "\" to engine \""
                   << pcEngine->GetId()
                   << "\""
                   << std::endl;
               */
               pcEngine->AddEntity(cEntity);
               /* Is the entity composable with a controllable component? */
               CComposableEntity* pcComposableEntity = dynamic_cast<CComposableEntity*>(&cEntity);
               if(pcComposableEntity != NULL &&
                  pcComposableEntity->HasComponent("controllable_entity")) {
                  /* The entity is controllable, there's more to do: associating a controller */
                  CControllableEntity& cControllableEntity = pcComposableEntity->GetComponent<CControllableEntity>("controllable_entity");
                  TConfigurationNode tControllerParameters;
                  /* Try to get the <parameters> section in the robot entity */
                  /* If there's none, get the one set in the controller */
                  if(NodeExists(*itEntities, "parameters")) {
                     /* Look in the <parameters> section in the entity */
                     tControllerParameters = GetNode(*itEntities, "parameters");
                  }
                  else {
                     /* The previous check failed, so look in the controller now */
                     /* Look for the controller with the right id in the XML */
                     TConfigurationNode tControllersTree;
                     tControllersTree = GetNode(t_tree, "controllers");
                     bool bFound = false;
                     TConfigurationNodeIterator itControllers;
                     std::string strControllerId;
                     itControllers = itControllers.begin(&tControllersTree);
                     while(!bFound && itControllers != itControllers.end()) {
                        GetNodeAttribute(*itControllers, "id", strControllerId);
                        if(strControllerId == cControllableEntity.GetControllerId()) {
                           bFound = true;
                        }
                        else {
                           ++itControllers;
                        }
                     }
                     /* Did we find the controller? */
                     if(! bFound) {
                        THROW_ARGOSEXCEPTION(
                           "[FATAL] The entity \"" <<
                           cControllableEntity.GetId() << "\" has been associated with a controller with id \"" <<
                           cControllableEntity.GetControllerId() <<
                           "\", but a controller with this id wasn't found in the <controllers> section of the XML file.");
                     }
                     /* Now itControllers points to the right controller subtree */
                     /* Get the parameters subtree */
                     tControllerParameters = GetNode(*itControllers, "parameters");
                  }
                  /* TODO */
                  /* Create the controller */
                  // CCI_Controller* pcController =
                  //    GetDynamicLinkingManager().
                  //    NewController(cEntity,
                  //                  cControllableEntity.GetControllerId(),
                  //                  tControllerParameters);
                  
                  // /* Set the controller to the entity */
                  // cControllableEntity.SetController(*pcController);
               }
            }
         }
      }
      m_pcSpace->SetPhysicsEngines(m_vecPhysicsEngines);
   }
}
