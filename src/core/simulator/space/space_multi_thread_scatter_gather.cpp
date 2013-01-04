/**
 * @file <argos3/core/simulator/space/space_multi_thread_scatter_gather.cpp>
 *
 * @brief This file provides the implementation of the Swarmanoid 3D Space.
 *
 * This file provides the implementation of the Swarmanoid 3D Space.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <unistd.h>
#include <cstring>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/profiler/profiler.h>
#include "space_multi_thread_scatter_gather.h"

namespace argos {

   /****************************************/
   /****************************************/

   struct SCleanupUpdateThreadData {
      pthread_mutex_t* SenseControlStepConditionalMutex;
      pthread_mutex_t* ActConditionalMutex;
      pthread_mutex_t* PhysicsConditionalMutex;
   };

   static void CleanupUpdateThread(void* p_data) {
      CSimulator& cSimulator = CSimulator::GetInstance();
      if(cSimulator.IsProfiling()) {
         cSimulator.GetProfiler().CollectThreadResourceUsage();
      }
      SCleanupUpdateThreadData& sData =
         *reinterpret_cast<SCleanupUpdateThreadData*>(p_data);
      pthread_mutex_unlock(sData.SenseControlStepConditionalMutex);
      pthread_mutex_unlock(sData.ActConditionalMutex);
      pthread_mutex_unlock(sData.PhysicsConditionalMutex);
   }

   void* LaunchUpdateThreadScatterGather(void* p_data) {
      LOG.AddThreadSafeBuffer();
      LOGERR.AddThreadSafeBuffer();
      CSpaceMultiThreadScatterGather::SUpdateThreadData* psData = reinterpret_cast<CSpaceMultiThreadScatterGather::SUpdateThreadData*>(p_data);
      psData->Space->UpdateThread(psData->ThreadId);
      return NULL;
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadScatterGather::Init(TConfigurationNode& t_tree)
   {
      /* Initialize the space */
      CSpace::Init(t_tree);

      /* Initialize thread related structures */
      int nErrors;
      /* First the counters */
      m_unSenseControlStepPhaseDoneCounter = CSimulator::GetInstance().GetNumThreads();
      m_unActPhaseDoneCounter = CSimulator::GetInstance().GetNumThreads();
      m_unPhysicsPhaseDoneCounter = CSimulator::GetInstance().GetNumThreads();
      /* Then the mutexes */
      if((nErrors = pthread_mutex_init(&m_tSenseControlStepConditionalMutex, NULL)) ||
         (nErrors = pthread_mutex_init(&m_tPhysicsConditionalMutex, NULL)) ||
         (nErrors = pthread_mutex_init(&m_tActConditionalMutex, NULL))) {
         THROW_ARGOSEXCEPTION("Error creating thread mutexes " << ::strerror(nErrors));
      }
      /* Finally the conditionals */
      if((nErrors = pthread_cond_init(&m_tSenseControlStepConditional, NULL)) ||
         (nErrors = pthread_cond_init(&m_tPhysicsConditional, NULL)) ||
         (nErrors = pthread_cond_init(&m_tActConditional, NULL))) {
         THROW_ARGOSEXCEPTION("Error creating thread conditionals " << ::strerror(nErrors));
      }
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadScatterGather::StartThreads() {
      int nErrors;
      /* Create the threads to update the controllable entities */
      m_ptUpdateThreads = new pthread_t[CSimulator::GetInstance().GetNumThreads()];
      m_psUpdateThreadData = new SUpdateThreadData*[CSimulator::GetInstance().GetNumThreads()];
      for(UInt32 i = 0; i < CSimulator::GetInstance().GetNumThreads(); ++i) {
         /* Create the struct with the info to launch the thread */
         m_psUpdateThreadData[i] = new SUpdateThreadData(i, this);
         /* Create the thread */
         if((nErrors = pthread_create(m_ptUpdateThreads + i,
                                      NULL,
                                      LaunchUpdateThreadScatterGather,
                                      reinterpret_cast<void*>(m_psUpdateThreadData[i])))) {
            THROW_ARGOSEXCEPTION("Error creating thread: " << ::strerror(nErrors));
         }
      }
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadScatterGather::Destroy()
   {
      /* Destroy the threads to update the controllable entities */
      int nErrors;
      if(m_ptUpdateThreads != NULL) {
         for(UInt32 i = 0; i < CSimulator::GetInstance().GetNumThreads(); ++i) {
            if((nErrors = pthread_cancel(m_ptUpdateThreads[i]))) {
               THROW_ARGOSEXCEPTION("Error canceling controllable entities update threads " << ::strerror(nErrors));
            }
         }
         void** ppJoinResult = new void*[CSimulator::GetInstance().GetNumThreads()];
         for(UInt32 i = 0; i < CSimulator::GetInstance().GetNumThreads(); ++i) {
            if((nErrors = pthread_join(m_ptUpdateThreads[i], ppJoinResult + i))) {
               THROW_ARGOSEXCEPTION("Error joining controllable entities update threads " << ::strerror(nErrors));
            }
            if(ppJoinResult[i] != PTHREAD_CANCELED) {
               LOGERR << "[WARNING] Controllable entities update thread #" << i<< " not canceled" << std::endl;
            }
         }
         delete[] ppJoinResult;
      }
      delete[] m_ptUpdateThreads;
      /* Destroy the thread launch info */
      if(m_psUpdateThreadData != NULL) {
         for(UInt32 i = 0; i < CSimulator::GetInstance().GetNumThreads(); ++i) {
            delete m_psUpdateThreadData[i];
         }
      }
      delete[] m_psUpdateThreadData;
      pthread_mutex_destroy(&m_tSenseControlStepConditionalMutex);
      pthread_mutex_destroy(&m_tActConditionalMutex);
      pthread_cond_destroy(&m_tSenseControlStepConditional);
      pthread_cond_destroy(&m_tActConditional);

      /* Destroy the base space */
      CSpace::Destroy();
   }

   /****************************************/
   /****************************************/
   
   void CSpaceMultiThreadScatterGather::AddControllableEntity(CControllableEntity& c_entity) {
      m_bIsControllableEntityAssignmentRecalculationNeeded = true;
      CSpace::AddControllableEntity(c_entity);
   }

   /****************************************/
   /****************************************/
   
   void CSpaceMultiThreadScatterGather::RemoveControllableEntity(CControllableEntity& c_entity) {
      m_bIsControllableEntityAssignmentRecalculationNeeded = true;
      CSpace::RemoveControllableEntity(c_entity);
   }

   /****************************************/
   /****************************************/
   
   void CSpaceMultiThreadScatterGather::SetPhysicsEngines(CPhysicsEngine::TVector& t_engines) {
      CSpace::SetPhysicsEngines(t_engines);
      StartThreads();
   }

   /****************************************/
   /****************************************/
   
#define MAIN_SEND_GO_FOR_PHASE(PHASE)                       \
   LOG.Flush();                                             \
   LOGERR.Flush();                                          \
   pthread_mutex_lock(&m_t ## PHASE ## ConditionalMutex);   \
   m_un ## PHASE ## PhaseDoneCounter = 0;                   \
   pthread_cond_broadcast(&m_t ## PHASE ## Conditional);    \
   pthread_mutex_unlock(&m_t ## PHASE ## ConditionalMutex);

#define MAIN_WAIT_FOR_PHASE_END(PHASE)                                  \
   pthread_mutex_lock(&m_t ## PHASE ## ConditionalMutex);               \
   while(m_un ## PHASE ## PhaseDoneCounter < CSimulator::GetInstance().GetNumThreads()) { \
      pthread_cond_wait(&m_t ## PHASE ## Conditional, &m_t ## PHASE ## ConditionalMutex); \
   }                                                                    \
   pthread_mutex_unlock(&m_t ## PHASE ## ConditionalMutex);
   
   void CSpaceMultiThreadScatterGather::UpdateControllableEntities() {
      MAIN_SEND_GO_FOR_PHASE(SenseControlStep);
      MAIN_WAIT_FOR_PHASE_END(SenseControlStep);
      MAIN_SEND_GO_FOR_PHASE(Act);
      MAIN_WAIT_FOR_PHASE_END(Act);
      /* Avoid recalculation at the next time step */
      m_bIsControllableEntityAssignmentRecalculationNeeded = false;
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadScatterGather::UpdatePhysics() {
      /* Update the physics engines */
      MAIN_SEND_GO_FOR_PHASE(Physics);
      MAIN_WAIT_FOR_PHASE_END(Physics);
      /* Perform entity transfer from engine to engine, if needed */
      for(size_t i = 0; i < m_ptPhysicsEngines->size(); ++i) {
         if((*m_ptPhysicsEngines)[i]->IsEntityTransferNeeded()) {
            (*m_ptPhysicsEngines)[i]->TransferEntities();
         }
      }
   }

   /****************************************/
   /****************************************/

#define THREAD_WAIT_FOR_GO_SIGNAL(PHASE)                                                   \
   pthread_mutex_lock(&m_t ## PHASE ## ConditionalMutex);                                  \
   while(m_un ## PHASE ## PhaseDoneCounter == CSimulator::GetInstance().GetNumThreads()) { \
      pthread_cond_wait(&m_t ## PHASE ## Conditional, &m_t ## PHASE ## ConditionalMutex);  \
   }                                                                                       \
   pthread_mutex_unlock(&m_t ## PHASE ## ConditionalMutex);                                \
   pthread_testcancel();
   
#define THREAD_SIGNAL_PHASE_DONE(PHASE)                     \
   pthread_mutex_lock(&m_t ## PHASE ## ConditionalMutex);   \
   ++m_un ## PHASE ## PhaseDoneCounter;                     \
   pthread_cond_broadcast(&m_t ## PHASE ## Conditional);    \
   pthread_mutex_unlock(&m_t ## PHASE ## ConditionalMutex); \
   pthread_testcancel();

   void CSpaceMultiThreadScatterGather::UpdateThread(UInt32 un_id) {
      /* Copy the id */
      UInt32 unId = un_id;
      /* Create cancellation data */
      SCleanupUpdateThreadData sCancelData;
      sCancelData.SenseControlStepConditionalMutex = &m_tSenseControlStepConditionalMutex;
      sCancelData.ActConditionalMutex = &m_tActConditionalMutex;
      sCancelData.PhysicsConditionalMutex = &m_tPhysicsConditionalMutex;
      pthread_cleanup_push(CleanupUpdateThread, &sCancelData);
      /* Calculate whether this thread is in charge of physics engines and how many */
      size_t tPhysicsPortion = (m_ptPhysicsEngines->size() / CSimulator::GetInstance().GetNumThreads());
      if((m_ptPhysicsEngines->size() % CSimulator::GetInstance().GetNumThreads()) != 0) {
         ++tPhysicsPortion;
      }
      CRange<size_t> cPhysicsRange(0, 1);
      bool bUpdateEngines = false;
      if(unId * tPhysicsPortion < m_ptPhysicsEngines->size()) {
         bUpdateEngines = true;
         cPhysicsRange.Set(unId * tPhysicsPortion,
                           Min<size_t>(unId * tPhysicsPortion + tPhysicsPortion,
                                       m_ptPhysicsEngines->size()));
      }
      /* Variables storing the portion of entities to update */
      size_t tEntityPortion;
      CRange<size_t> cEntityRange(0,1);
      bool bUpdateEntities = false;
      while(1) {
         THREAD_WAIT_FOR_GO_SIGNAL(SenseControlStep);
         /* Calculate the portion of entities to update, if needed */
         if(m_bIsControllableEntityAssignmentRecalculationNeeded) {
            tEntityPortion = (m_vecControllableEntities.size() / CSimulator::GetInstance().GetNumThreads());
            if((m_vecControllableEntities.size() % CSimulator::GetInstance().GetNumThreads()) != 0) {
               ++tEntityPortion;
            }
            /* Cope with the fact that there may be less entities than threads */
            if(unId * tEntityPortion < m_vecControllableEntities.size()) {
               /* This thread has entities */
               bUpdateEntities = true;
               cEntityRange.Set(unId * tEntityPortion,
                                Min<size_t>(unId * tEntityPortion + tEntityPortion,
                                            m_vecControllableEntities.size()));
            }
         }
         /* Cope with the fact that there may be less entities than threads */
         if(bUpdateEntities) {
            /* This thread has entities */
            /* Update sensor readings and call controllers */
            for(size_t i = cEntityRange.GetMin(); i < cEntityRange.GetMax(); ++i) {
               m_vecControllableEntities[i]->Sense();
               m_vecControllableEntities[i]->ControlStep();
            }
            pthread_testcancel();
            THREAD_SIGNAL_PHASE_DONE(SenseControlStep);
            /* Actuate control choices */
            THREAD_WAIT_FOR_GO_SIGNAL(Act);
            for(size_t i = cEntityRange.GetMin(); i < cEntityRange.GetMax(); ++i) {
               m_vecControllableEntities[i]->Act();
            }
            pthread_testcancel();
            THREAD_SIGNAL_PHASE_DONE(Act);
         }
         else {
            /* This thread has no entities -> dummy computation */
            /* Update sensor readings */
            /* Call controllers */
            THREAD_WAIT_FOR_GO_SIGNAL(SenseControlStep);
            THREAD_SIGNAL_PHASE_DONE(SenseControlStep);
            /* Actuate control choices */
            THREAD_WAIT_FOR_GO_SIGNAL(Act);
            THREAD_SIGNAL_PHASE_DONE(Act);
         }
         /* Update physics engines, if this thread has been assigned to them */
         THREAD_WAIT_FOR_GO_SIGNAL(Physics);
         if(bUpdateEngines) {
            /* This thread has engines, update them */
            for(size_t i = cPhysicsRange.GetMin(); i < cPhysicsRange.GetMax(); ++i) {
               (*m_ptPhysicsEngines)[i]->Update();
            }
            pthread_testcancel();
            THREAD_SIGNAL_PHASE_DONE(Physics);
         }
         else {
            /* This thread has no engines -> dummy computation */
            THREAD_SIGNAL_PHASE_DONE(Physics);
         }
      }
      pthread_cleanup_pop(1);
   }

   /****************************************/
   /****************************************/

}
