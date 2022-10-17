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
      pthread_mutex_t* StartEntityIterPhaseMutex;
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
      pthread_mutex_unlock(sData.StartEntityIterPhaseMutex);
   }

   void* LaunchThreadBalanceLength(void* p_data) {
      /* Set up thread-safe buffers for this new thread */
      LOG.AddThreadSafeBuffer();
      LOGERR.AddThreadSafeBuffer();
      /* Make this thread cancellable */
      pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
      pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, nullptr);
      /* Get a handle to the thread launch data */
      auto* psData = reinterpret_cast<CSpaceMultiThreadBalanceLength::SThreadLaunchData*>(p_data);
      /* Create cancellation data */
      SCleanupThreadData sCancelData;
      sCancelData.StartSenseControlPhaseMutex = &(psData->Space->m_tStartSenseControlPhaseMutex);
      sCancelData.StartActPhaseMutex = &(psData->Space->m_tStartActPhaseMutex);
      sCancelData.StartPhysicsPhaseMutex = &(psData->Space->m_tStartPhysicsPhaseMutex);
      sCancelData.StartMediaPhaseMutex = &(psData->Space->m_tStartMediaPhaseMutex);
      sCancelData.StartEntityIterPhaseMutex = &(psData->Space->m_tStartEntityIterPhaseMutex);
      sCancelData.FetchTaskMutex = &(psData->Space->m_tFetchTaskMutex);
      pthread_cleanup_push(CleanupThread, &sCancelData);
      psData->Space->SlaveThread();
      /* Dispose of cancellation data */
      pthread_cleanup_pop(1);
      return nullptr;
   }

   /****************************************/
   /****************************************/

   CSpaceMultiThreadBalanceLength::CSpaceMultiThreadBalanceLength(UInt32 un_n_threads,
                                                                  bool b_pin_threads_to_cores) :
       CSpaceMultiThread(un_n_threads, b_pin_threads_to_cores) {
     LOG << "[INFO]   Chosen method \"balance_length\": threads will be assigned different"
         << std::endl
         << "[INFO]   numbers of tasks, depending on the task length."
         << std::endl;
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceLength::Init(TConfigurationNode& t_tree) {
      /* Initialize the space */
      CSpace::Init(t_tree);
      /* Initialize thread related structures */
      int nErrors;
      /* Init mutexes */
      if((nErrors = pthread_mutex_init(&m_tStartSenseControlPhaseMutex, nullptr)) ||
         (nErrors = pthread_mutex_init(&m_tStartActPhaseMutex, nullptr)) ||
         (nErrors = pthread_mutex_init(&m_tStartPhysicsPhaseMutex, nullptr)) ||
         (nErrors = pthread_mutex_init(&m_tStartMediaPhaseMutex, nullptr)) ||
         (nErrors = pthread_mutex_init(&m_tStartEntityIterPhaseMutex, nullptr)) ||
         (nErrors = pthread_mutex_init(&m_tFetchTaskMutex, nullptr))) {
         THROW_ARGOSEXCEPTION("Error creating thread mutexes " << ::strerror(nErrors));
      }
      /* Init conditionals */
      if((nErrors = pthread_cond_init(&m_tStartSenseControlPhaseCond, nullptr)) ||
         (nErrors = pthread_cond_init(&m_tStartActPhaseCond, nullptr)) ||
         (nErrors = pthread_cond_init(&m_tStartPhysicsPhaseCond, nullptr)) ||
         (nErrors = pthread_cond_init(&m_tStartMediaPhaseCond, nullptr)) ||
         (nErrors = pthread_cond_init(&m_tStartEntityIterPhaseCond, nullptr)) ||
         (nErrors = pthread_cond_init(&m_tFetchTaskCond, nullptr))) {
         THROW_ARGOSEXCEPTION("Error creating thread conditionals " << ::strerror(nErrors));
      }
      /* Reset the idle thread count */
      m_unSenseControlPhaseIdleCounter = GetNumThreads();
      m_unActPhaseIdleCounter = GetNumThreads();
      m_unPhysicsPhaseIdleCounter = GetNumThreads();
      m_unMediaPhaseIdleCounter = GetNumThreads();
      m_unEntityIterPhaseIdleCounter = GetNumThreads();
      /* Start threads */
      StartThreads();
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceLength::Destroy() {
      /* Destroy the threads */
      DestroyAllThreads();

      /* Destroy the thread launch info */
      if(m_psThreadData != nullptr) {
         for(UInt32 i = 0; i < GetNumThreads(); ++i) {
            delete m_psThreadData[i];
         }
      }
      delete[] m_psThreadData;
      pthread_mutex_destroy(&m_tStartSenseControlPhaseMutex);
      pthread_mutex_destroy(&m_tStartActPhaseMutex);
      pthread_mutex_destroy(&m_tStartPhysicsPhaseMutex);
      pthread_mutex_destroy(&m_tStartMediaPhaseMutex);
      pthread_mutex_destroy(&m_tStartEntityIterPhaseMutex);
      pthread_mutex_destroy(&m_tFetchTaskMutex);

      pthread_cond_destroy(&m_tStartSenseControlPhaseCond);
      pthread_cond_destroy(&m_tStartActPhaseCond);
      pthread_cond_destroy(&m_tStartPhysicsPhaseCond);
      pthread_cond_destroy(&m_tStartMediaPhaseCond);
      pthread_cond_destroy(&m_tStartEntityIterPhaseCond);
      pthread_cond_destroy(&m_tFetchTaskCond);

      /* Destroy the base space */
      CSpace::Destroy();
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceLength::Update() {
      /* Reset the idle thread count */
      m_unSenseControlPhaseIdleCounter = GetNumThreads();
      m_unActPhaseIdleCounter = GetNumThreads();
      m_unPhysicsPhaseIdleCounter = GetNumThreads();
      m_unMediaPhaseIdleCounter = GetNumThreads();
      m_unEntityIterPhaseIdleCounter = GetNumThreads();
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
   while(m_un ## PHASE ## PhaseIdleCounter < GetNumThreads()) {   \
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

   void CSpaceMultiThreadBalanceLength::IterateOverControllableEntities(
       const TControllableEntityIterCBType &c_cb) {
     m_cbControllableEntityIter = c_cb;
     /* Iterate over all robots in the swarm */
     MAIN_START_PHASE(EntityIter);
     MAIN_WAIT_FOR_END_OF(EntityIter);
   } /* IterateOverControllableEntities() */


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
      m_psThreadData = new SThreadLaunchData*[GetNumThreads()];
      /* Create the threads */
      for(UInt32 i = 0; i < GetNumThreads(); ++i) {
         /* Create the struct with the info to launch the thread */
         m_psThreadData[i] = new SThreadLaunchData(i, this);
         /* Create the thread */
         CreateSingleThread(i,
                            LaunchThreadBalanceLength,
                            reinterpret_cast<void*>(m_psThreadData[i]));
      }
   }

   /****************************************/
   /****************************************/

#define THREAD_WAIT_FOR_START_OF(PHASE)                                                     \
   pthread_mutex_lock(&m_tStart ## PHASE ## PhaseMutex);                                    \
   while(m_un ## PHASE ## PhaseIdleCounter == GetNumThreads()) {  \
      pthread_cond_wait(&m_tStart ## PHASE ## PhaseCond, &m_tStart ## PHASE ## PhaseMutex); \
   }                                                                                        \
   pthread_mutex_unlock(&m_tStart ## PHASE ## PhaseMutex);                                  \
   pthread_testcancel();

#define THREAD_PERFORM_TASK(PHASE, TASKVEC, CONDITION, SNIPPET)     \
   while(1) {                                                       \
      pthread_mutex_lock(&m_tFetchTaskMutex);                       \
      if((CONDITION) && m_unTaskIndex < (TASKVEC).size()) {         \
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
            true,
            if(m_vecControllableEntities[unTaskIndex]->IsEnabled()) m_vecControllableEntities[unTaskIndex]->Act();
            );
         THREAD_WAIT_FOR_START_OF(Physics);
         THREAD_PERFORM_TASK(
            Physics,
            *m_ptPhysicsEngines,
            true,
            (*m_ptPhysicsEngines)[unTaskIndex]->Update();
            );
         THREAD_WAIT_FOR_START_OF(Media);
         THREAD_PERFORM_TASK(
            Media,
            *m_ptMedia,
            true,
            (*m_ptMedia)[unTaskIndex]->Update();
            );
         /* loop functions PreStep() */
         THREAD_WAIT_FOR_START_OF(EntityIter);
         THREAD_PERFORM_TASK(
             EntityIter,
             m_vecControllableEntities,
             ControllableEntityIterationEnabled(),
             m_cbControllableEntityIter(m_vecControllableEntities[unTaskIndex]));
         THREAD_WAIT_FOR_START_OF(SenseControl);
         THREAD_PERFORM_TASK(
            SenseControl,
            m_vecControllableEntities,
            true,
            if(m_vecControllableEntities[unTaskIndex]->IsEnabled()) {
               m_vecControllableEntities[unTaskIndex]->Sense();
               m_vecControllableEntities[unTaskIndex]->ControlStep();
            }
            );
         /* loop functions PostStep() */
         THREAD_WAIT_FOR_START_OF(EntityIter);
         THREAD_PERFORM_TASK(
             EntityIter,
             m_vecControllableEntities,
             ControllableEntityIterationEnabled(),
             m_cbControllableEntityIter(m_vecControllableEntities[unTaskIndex]));
      } /* while(1) */
   }

   /****************************************/
   /****************************************/

}
