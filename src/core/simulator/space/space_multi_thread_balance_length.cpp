/**
 * @file <argos3/core/simulator/space/space_multi_thread_balance_length.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "space_multi_thread_balance_length.h"
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/profiler/profiler.h>

namespace argos {

   /****************************************/
   /****************************************/

   struct SCleanupThreadData {
      pthread_mutex_t* StartSenseControlPhaseMutex;
      pthread_mutex_t* StartActPhaseMutex;
      pthread_mutex_t* StartPhysicsPhaseMutex;
      pthread_mutex_t* StartMediaPhaseMutex;
      pthread_mutex_t* FetchTaskMutex;
   };

   static void CleanupThread(void* p_data) {
      CSimulator& cSimulator = CSimulator::GetInstance();
      if(cSimulator.IsProfiling()) {
         cSimulator.GetProfiler().CollectThreadResourceUsage();
      }
      SCleanupThreadData& sData =
         *reinterpret_cast<SCleanupThreadData*>(p_data);
      pthread_mutex_unlock(sData.FetchTaskMutex);
      pthread_mutex_unlock(sData.StartSenseControlPhaseMutex);
      pthread_mutex_unlock(sData.StartActPhaseMutex);
      pthread_mutex_unlock(sData.StartPhysicsPhaseMutex);
      pthread_mutex_unlock(sData.StartMediaPhaseMutex);
   }

   void* LaunchThreadBalanceLength(void* p_data) {
      /* Set up thread-safe buffers for this new thread */
      LOG.AddThreadSafeBuffer();
      LOGERR.AddThreadSafeBuffer();
      /* Make this thread cancellable */
      pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
      pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
      /* Get a handle to the thread launch data */
      CSpaceMultiThreadBalanceLength::SThreadLaunchData* psData = reinterpret_cast<CSpaceMultiThreadBalanceLength::SThreadLaunchData*>(p_data);
      /* Create cancellation data */
      SCleanupThreadData sCancelData;
      sCancelData.StartSenseControlPhaseMutex = &(psData->Space->m_tStartSenseControlPhaseMutex);
      sCancelData.StartActPhaseMutex = &(psData->Space->m_tStartActPhaseMutex);
      sCancelData.StartPhysicsPhaseMutex = &(psData->Space->m_tStartPhysicsPhaseMutex);
      sCancelData.StartMediaPhaseMutex = &(psData->Space->m_tStartMediaPhaseMutex);
      sCancelData.FetchTaskMutex = &(psData->Space->m_tFetchTaskMutex);
      pthread_cleanup_push(CleanupThread, &sCancelData);
      psData->Space->SlaveThread();
      /* Dispose of cancellation data */
      pthread_cleanup_pop(1);
      return NULL;
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceLength::Init(TConfigurationNode& t_tree) {
      /* Initialize the space */
      CSpace::Init(t_tree);
      /* Initialize thread related structures */
      int nErrors;
      /* Init mutexes */
      if((nErrors = pthread_mutex_init(&m_tStartSenseControlPhaseMutex, NULL)) ||
         (nErrors = pthread_mutex_init(&m_tStartActPhaseMutex, NULL)) ||
         (nErrors = pthread_mutex_init(&m_tStartPhysicsPhaseMutex, NULL)) ||
         (nErrors = pthread_mutex_init(&m_tStartMediaPhaseMutex, NULL)) ||
         (nErrors = pthread_mutex_init(&m_tFetchTaskMutex, NULL))) {
         THROW_ARGOSEXCEPTION("Error creating thread mutexes " << ::strerror(nErrors));
      }
      /* Init conditionals */
      if((nErrors = pthread_cond_init(&m_tStartSenseControlPhaseCond, NULL)) ||
         (nErrors = pthread_cond_init(&m_tStartActPhaseCond, NULL)) ||
         (nErrors = pthread_cond_init(&m_tStartPhysicsPhaseCond, NULL)) ||
         (nErrors = pthread_cond_init(&m_tStartMediaPhaseCond, NULL)) ||
         (nErrors = pthread_cond_init(&m_tFetchTaskCond, NULL))) {
         THROW_ARGOSEXCEPTION("Error creating thread conditionals " << ::strerror(nErrors));
      }
      /* Reset the idle thread count */
      m_unSenseControlPhaseIdleCounter = CSimulator::GetInstance().GetNumThreads();
      m_unActPhaseIdleCounter = CSimulator::GetInstance().GetNumThreads();
      m_unPhysicsPhaseIdleCounter = CSimulator::GetInstance().GetNumThreads();
      m_unMediaPhaseIdleCounter = CSimulator::GetInstance().GetNumThreads();
      /* Start threads */
      StartThreads();
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceLength::Destroy() {
      /* Destroy the threads to update the controllable entities */
      int nErrors;
      if(m_ptThreads != NULL) {
         for(UInt32 i = 0; i < CSimulator::GetInstance().GetNumThreads(); ++i) {
            if((nErrors = pthread_cancel(m_ptThreads[i]))) {
               THROW_ARGOSEXCEPTION("Error canceling threads " << ::strerror(nErrors));
            }
         }
         void** ppJoinResult = new void*[CSimulator::GetInstance().GetNumThreads()];
         for(UInt32 i = 0; i < CSimulator::GetInstance().GetNumThreads(); ++i) {
            if((nErrors = pthread_join(m_ptThreads[i], ppJoinResult + i))) {
               THROW_ARGOSEXCEPTION("Error joining threads " << ::strerror(nErrors));
            }
            if(ppJoinResult[i] != PTHREAD_CANCELED) {
               LOGERR << "[WARNING] Thread #" << i<< " not canceled" << std::endl;
            }
         }
         delete[] ppJoinResult;
      }
      delete[] m_ptThreads;
      /* Destroy the thread launch info */
      if(m_psThreadData != NULL) {
         for(UInt32 i = 0; i < CSimulator::GetInstance().GetNumThreads(); ++i) {
            delete m_psThreadData[i];
         }
      }
      delete[] m_psThreadData;
      pthread_mutex_destroy(&m_tStartSenseControlPhaseMutex);
      pthread_mutex_destroy(&m_tStartActPhaseMutex);
      pthread_mutex_destroy(&m_tStartPhysicsPhaseMutex);
      pthread_mutex_destroy(&m_tStartMediaPhaseMutex);
      pthread_mutex_destroy(&m_tFetchTaskMutex);
      pthread_cond_destroy(&m_tStartSenseControlPhaseCond);
      pthread_cond_destroy(&m_tStartActPhaseCond);
      pthread_cond_destroy(&m_tStartPhysicsPhaseCond);
      pthread_cond_destroy(&m_tStartMediaPhaseCond);
      pthread_cond_destroy(&m_tFetchTaskCond);

      /* Destroy the base space */
      CSpace::Destroy();
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceLength::Update() {
      /* Reset the idle thread count */
      m_unSenseControlPhaseIdleCounter = CSimulator::GetInstance().GetNumThreads();
      m_unActPhaseIdleCounter = CSimulator::GetInstance().GetNumThreads();
      m_unPhysicsPhaseIdleCounter = CSimulator::GetInstance().GetNumThreads();
      m_unMediaPhaseIdleCounter = CSimulator::GetInstance().GetNumThreads();
      /* Update the space */
      CSpace::Update();
   }

   /****************************************/
   /****************************************/

#define MAIN_START_PHASE(PHASE)                             \
   pthread_mutex_lock(&m_tStart ## PHASE ## PhaseMutex);    \
   m_un ## PHASE ## PhaseIdleCounter = 0;                   \
   m_unTaskIndex = 0;                                       \
   pthread_cond_broadcast(&m_tStart ## PHASE ## PhaseCond); \
   pthread_mutex_unlock(&m_tStart ## PHASE ## PhaseMutex);

#define MAIN_WAIT_FOR_END_OF(PHASE)                                                         \
   pthread_mutex_lock(&m_tStart ## PHASE ## PhaseMutex);                                    \
   while(m_un ## PHASE ## PhaseIdleCounter < CSimulator::GetInstance().GetNumThreads()) {   \
      pthread_cond_wait(&m_tStart ## PHASE ## PhaseCond, &m_tStart ## PHASE ## PhaseMutex); \
   }                                                                                        \
   pthread_mutex_unlock(&m_tStart ## PHASE ## PhaseMutex);

   void CSpaceMultiThreadBalanceLength::UpdateControllableEntitiesAct() {
      /* Act phase */
      MAIN_START_PHASE(Act);
      MAIN_WAIT_FOR_END_OF(Act);
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceLength::UpdatePhysics() {
      /* Physics phase */
      MAIN_START_PHASE(Physics);
      MAIN_WAIT_FOR_END_OF(Physics);
      /* Perform entity transfer from engine to engine, if needed */
      for(size_t i = 0; i < m_ptPhysicsEngines->size(); ++i) {
         if((*m_ptPhysicsEngines)[i]->IsEntityTransferNeeded()) {
            (*m_ptPhysicsEngines)[i]->TransferEntities();
         }
      }
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceLength::UpdateMedia() {
      /* Media phase */
      MAIN_START_PHASE(Media);
      MAIN_WAIT_FOR_END_OF(Media);
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceLength::UpdateControllableEntitiesSenseStep() {
      /* Sense/control phase */
      MAIN_START_PHASE(SenseControl);
      MAIN_WAIT_FOR_END_OF(SenseControl);
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceLength::StartThreads() {
      int nErrors;
      /* Create the threads to update the controllable entities */
      m_ptThreads = new pthread_t[CSimulator::GetInstance().GetNumThreads()];
      m_psThreadData = new SThreadLaunchData*[CSimulator::GetInstance().GetNumThreads()];
      for(UInt32 i = 0; i < CSimulator::GetInstance().GetNumThreads(); ++i) {
         /* Create the struct with the info to launch the thread */
         m_psThreadData[i] = new SThreadLaunchData(i, this);
         /* Create the thread */
         if((nErrors = pthread_create(m_ptThreads + i,
                                      NULL,
                                      LaunchThreadBalanceLength,
                                      reinterpret_cast<void*>(m_psThreadData[i])))) {
            THROW_ARGOSEXCEPTION("Error creating thread: " << ::strerror(nErrors));
         }
      }
   }

   /****************************************/
   /****************************************/

#define THREAD_WAIT_FOR_START_OF(PHASE)                                                     \
   pthread_mutex_lock(&m_tStart ## PHASE ## PhaseMutex);                                    \
   while(m_un ## PHASE ## PhaseIdleCounter == CSimulator::GetInstance().GetNumThreads()) {  \
      pthread_cond_wait(&m_tStart ## PHASE ## PhaseCond, &m_tStart ## PHASE ## PhaseMutex); \
   }                                                                                        \
   pthread_mutex_unlock(&m_tStart ## PHASE ## PhaseMutex);                                  \
   pthread_testcancel();

#define THREAD_PERFORM_TASK(PHASE, TASKVEC, SNIPPET)                \
   while(1) {                                                       \
      pthread_mutex_lock(&m_tFetchTaskMutex);                       \
      if(m_unTaskIndex < (TASKVEC).size()) {                        \
         unTaskIndex = m_unTaskIndex;                               \
         ++m_unTaskIndex;                                           \
         pthread_mutex_unlock(&m_tFetchTaskMutex);                  \
         pthread_testcancel();                                      \
         {                                                          \
            SNIPPET;                                                \
         }                                                          \
         pthread_testcancel();                                      \
      }                                                             \
      else {                                                        \
         pthread_mutex_unlock(&m_tFetchTaskMutex);                  \
         pthread_testcancel();                                      \
         pthread_mutex_lock(&m_tStart ## PHASE ## PhaseMutex);      \
         ++m_un ## PHASE ## PhaseIdleCounter;                       \
         pthread_cond_broadcast(&m_tStart ## PHASE ## PhaseCond);   \
         pthread_mutex_unlock(&m_tStart ## PHASE ## PhaseMutex);    \
         pthread_testcancel();                                      \
         break;                                                     \
      }                                                             \
   }                                                                \
   pthread_testcancel();

   void CSpaceMultiThreadBalanceLength::SlaveThread() {
      /* Task index */
      size_t unTaskIndex;
      while(1) {
         THREAD_WAIT_FOR_START_OF(Act);
         THREAD_PERFORM_TASK(
            Act,
            m_vecControllableEntities,
            m_vecControllableEntities[unTaskIndex]->Act();
            );
         THREAD_WAIT_FOR_START_OF(Physics);
         THREAD_PERFORM_TASK(
            Physics,
            *m_ptPhysicsEngines,
            (*m_ptPhysicsEngines)[unTaskIndex]->Update();
            );
         THREAD_WAIT_FOR_START_OF(Media);
         THREAD_PERFORM_TASK(
            Media,
            *m_ptMedia,
            (*m_ptMedia)[unTaskIndex]->Update();
            );
         THREAD_WAIT_FOR_START_OF(SenseControl);
         THREAD_PERFORM_TASK(
            SenseControl,
            m_vecControllableEntities,
            m_vecControllableEntities[unTaskIndex]->Sense();
            m_vecControllableEntities[unTaskIndex]->ControlStep();
            );
      }
   }

   /****************************************/
   /****************************************/

}
