/**
 * @file <argos3/core/simulator/space/space_multi_thread_balance_quantity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <unistd.h>
#include <cstring>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/utility/profiler/profiler.h>
#include "space_multi_thread_balance_quantity.h"

namespace argos {

   /****************************************/
   /****************************************/

   struct SCleanupUpdateThreadData {
      pthread_mutex_t* SenseControlStepConditionalMutex;
      pthread_mutex_t* ActConditionalMutex;
      pthread_mutex_t* PhysicsConditionalMutex;
      pthread_mutex_t* MediaConditionalMutex;
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
      pthread_mutex_unlock(sData.MediaConditionalMutex);
   }

   void* LaunchUpdateThreadBalanceQuantity(void* p_data) {
      LOG.AddThreadSafeBuffer();
      LOGERR.AddThreadSafeBuffer();
      CSpaceMultiThreadBalanceQuantity::SUpdateThreadData* psData = reinterpret_cast<CSpaceMultiThreadBalanceQuantity::SUpdateThreadData*>(p_data);
      psData->Space->UpdateThread(psData->ThreadId);
      return NULL;
   }

   /****************************************/
   /****************************************/

   CSpaceMultiThreadBalanceQuantity::CSpaceMultiThreadBalanceQuantity() :
      m_psUpdateThreadData(NULL),
      m_ptUpdateThreads(NULL),
      m_bIsControllableEntityAssignmentRecalculationNeeded(true) {}

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceQuantity::Init(TConfigurationNode& t_tree) {
      /* Initialize the space */
      CSpace::Init(t_tree);
      /* Initialize thread related structures */
      int nErrors;
      /* First the counters */
      m_unSenseControlStepPhaseDoneCounter = CSimulator::GetInstance().GetNumThreads();
      m_unActPhaseDoneCounter = CSimulator::GetInstance().GetNumThreads();
      m_unPhysicsPhaseDoneCounter = CSimulator::GetInstance().GetNumThreads();
      m_unMediaPhaseDoneCounter = CSimulator::GetInstance().GetNumThreads();
      /* Then the mutexes */
      if((nErrors = pthread_mutex_init(&m_tSenseControlStepConditionalMutex, NULL)) ||
         (nErrors = pthread_mutex_init(&m_tActConditionalMutex, NULL)) ||
         (nErrors = pthread_mutex_init(&m_tPhysicsConditionalMutex, NULL)) ||
         (nErrors = pthread_mutex_init(&m_tMediaConditionalMutex, NULL))) {
         THROW_ARGOSEXCEPTION("Error creating thread mutexes " << ::strerror(nErrors));
      }
      /* Finally the conditionals */
      if((nErrors = pthread_cond_init(&m_tSenseControlStepConditional, NULL)) ||
         (nErrors = pthread_cond_init(&m_tActConditional, NULL)) ||
         (nErrors = pthread_cond_init(&m_tPhysicsConditional, NULL)) ||
         (nErrors = pthread_cond_init(&m_tMediaConditional, NULL))) {
         THROW_ARGOSEXCEPTION("Error creating thread conditionals " << ::strerror(nErrors));
      }
      /* Start threads */
      StartThreads();
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceQuantity::StartThreads() {
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
                                      LaunchUpdateThreadBalanceQuantity,
                                      reinterpret_cast<void*>(m_psUpdateThreadData[i])))) {
            THROW_ARGOSEXCEPTION("Error creating thread: " << ::strerror(nErrors));
         }
      }
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceQuantity::Destroy() {
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
      pthread_mutex_destroy(&m_tPhysicsConditionalMutex);
      pthread_mutex_destroy(&m_tMediaConditionalMutex);
      pthread_cond_destroy(&m_tSenseControlStepConditional);
      pthread_cond_destroy(&m_tActConditional);
      pthread_cond_destroy(&m_tPhysicsConditional);
      pthread_cond_destroy(&m_tMediaConditional);
      /* Destroy the base space */
      CSpace::Destroy();
   }

   /****************************************/
   /****************************************/
   
   void CSpaceMultiThreadBalanceQuantity::AddControllableEntity(CControllableEntity& c_entity) {
      m_bIsControllableEntityAssignmentRecalculationNeeded = true;
      CSpace::AddControllableEntity(c_entity);
   }

   /****************************************/
   /****************************************/
   
   void CSpaceMultiThreadBalanceQuantity::RemoveControllableEntity(CControllableEntity& c_entity) {
      m_bIsControllableEntityAssignmentRecalculationNeeded = true;
      CSpace::RemoveControllableEntity(c_entity);
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
   
   void CSpaceMultiThreadBalanceQuantity::UpdateControllableEntitiesAct() {
      MAIN_SEND_GO_FOR_PHASE(Act);
      MAIN_WAIT_FOR_PHASE_END(Act);
      /* Avoid recalculation at the next time step */
      m_bIsControllableEntityAssignmentRecalculationNeeded = false;
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceQuantity::UpdatePhysics() {
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

   void CSpaceMultiThreadBalanceQuantity::UpdateMedia() {
      /* Update the media */
      MAIN_SEND_GO_FOR_PHASE(Media);
      MAIN_WAIT_FOR_PHASE_END(Media);
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceQuantity::UpdateControllableEntitiesSenseStep() {
      MAIN_SEND_GO_FOR_PHASE(SenseControlStep);
      MAIN_WAIT_FOR_PHASE_END(SenseControlStep);
      /* Avoid recalculation at the next time step */
      m_bIsControllableEntityAssignmentRecalculationNeeded = false;
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

   CRange<size_t> CalculatePluginRangeForThread(size_t un_id,
                                                size_t un_tot_plugins) {
      /* This is the minimum number of plugins assigned to a thread */
      size_t unMinPortion = un_tot_plugins / CSimulator::GetInstance().GetNumThreads();
      /* If the division has a remainder, the extra plugins must be assigned too */
      size_t unExtraPortion = un_tot_plugins % CSimulator::GetInstance().GetNumThreads();
      /* Calculate the range */
      if(unMinPortion == 0) {
         /* Not all threads get a plugin */
         if(un_id < unExtraPortion) {
            /* This thread does */
            return CRange<size_t>(un_id, un_id+1);
         }
         else {
            /* This thread does not */
            return CRange<size_t>();
         }
      }
      else {
         /* For sure this thread will get unMinPortion plugins, does it get an extra too? */
         if(un_id < unExtraPortion) {
            /* Yes, it gets an extra */
            return CRange<size_t>( un_id    * (unMinPortion+1),
                                  (un_id+1) * (unMinPortion+1));
         }
         else {
            /* No, it doesn't get an extra */
            return CRange<size_t>(unExtraPortion * (unMinPortion+1) + (un_id-unExtraPortion)   * unMinPortion,
                                  unExtraPortion * (unMinPortion+1) + (un_id-unExtraPortion+1) * unMinPortion);
         }
      }
   }

   void CSpaceMultiThreadBalanceQuantity::UpdateThread(UInt32 un_id) {
      /* Copy the id */
      UInt32 unId = un_id;
      /* Create cancellation data */
      SCleanupUpdateThreadData sCancelData;
      sCancelData.SenseControlStepConditionalMutex = &m_tSenseControlStepConditionalMutex;
      sCancelData.ActConditionalMutex = &m_tActConditionalMutex;
      sCancelData.PhysicsConditionalMutex = &m_tPhysicsConditionalMutex;
      sCancelData.MediaConditionalMutex = &m_tMediaConditionalMutex;
      pthread_cleanup_push(CleanupUpdateThread, &sCancelData);
      /* Id range for the physics engines assigned to this thread */
      CRange<size_t> cPhysicsRange = CalculatePluginRangeForThread(unId, m_ptPhysicsEngines->size());
      /* Id range for the physics engines assigned to this thread */
      CRange<size_t> cMediaRange = CalculatePluginRangeForThread(unId, m_ptMedia->size());
      /* Variables storing the portion of entities to update */
      CRange<size_t> cEntityRange;
      while(1) {
         THREAD_WAIT_FOR_GO_SIGNAL(Act);
         /* Calculate the portion of entities to update, if needed */
         if(m_bIsControllableEntityAssignmentRecalculationNeeded) {
            cEntityRange = CalculatePluginRangeForThread(unId, m_vecControllableEntities.size());
         }
         /* Cope with the fact that there may be less entities than threads */
         if(cEntityRange.GetSpan() > 0) {
            /* This thread has entities */
            /* Actuate control choices */
            for(size_t i = cEntityRange.GetMin(); i < cEntityRange.GetMax(); ++i) {
               m_vecControllableEntities[i]->Act();
            }
            pthread_testcancel();
            THREAD_SIGNAL_PHASE_DONE(Act);
         }
         else {
            /* This thread has no entities -> dummy computation */
            THREAD_SIGNAL_PHASE_DONE(Act);
         }
         /* Update physics engines, if this thread has been assigned to them */
         THREAD_WAIT_FOR_GO_SIGNAL(Physics);
         if(cPhysicsRange.GetSpan() > 0) {
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
         /* Update media, if this thread has been assigned to them */
         THREAD_WAIT_FOR_GO_SIGNAL(Media);
         if(cMediaRange.GetSpan() > 0) {
            /* This thread has media, update them */
            for(size_t i = cMediaRange.GetMin(); i < cMediaRange.GetMax(); ++i) {
               (*m_ptMedia)[i]->Update();
            }
            pthread_testcancel();
            THREAD_SIGNAL_PHASE_DONE(Media);
         }
         else {
            /* This thread has no media -> dummy computation */
            THREAD_SIGNAL_PHASE_DONE(Media);
         }
         /* Update sensor readings and call controllers */
         THREAD_WAIT_FOR_GO_SIGNAL(SenseControlStep);
         /* Cope with the fact that there may be less entities than threads */
         if(cEntityRange.GetSpan() > 0) {
            /* This thread has entities */
            for(size_t i = cEntityRange.GetMin(); i < cEntityRange.GetMax(); ++i) {
               m_vecControllableEntities[i]->Sense();
               m_vecControllableEntities[i]->ControlStep();
            }
            pthread_testcancel();
            THREAD_SIGNAL_PHASE_DONE(SenseControlStep);
         }
         else {
            /* This thread has no entities -> dummy computation */
            THREAD_SIGNAL_PHASE_DONE(SenseControlStep);
         }
      }
      pthread_cleanup_pop(1);
   }

   /****************************************/
   /****************************************/

}
