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
      pthread_mutex_t* EntityIterConditionalMutex;
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
      pthread_mutex_unlock(sData.EntityIterConditionalMutex);
   }

   void* LaunchUpdateThreadBalanceQuantity(void* p_data) {
      LOG.AddThreadSafeBuffer();
      LOGERR.AddThreadSafeBuffer();
      auto* psData = reinterpret_cast<CSpaceMultiThreadBalanceQuantity::SUpdateThreadData*>(p_data);
      psData->Space->UpdateThread(psData->ThreadId);
      return nullptr;
   }

   /****************************************/
   /****************************************/

   CSpaceMultiThreadBalanceQuantity::CSpaceMultiThreadBalanceQuantity(UInt32 un_n_threads,
                                                                      bool b_pin_threads_to_cores) :
         CSpaceMultiThread(un_n_threads, b_pin_threads_to_cores),
         m_psUpdateThreadData(nullptr),
         m_bIsControllableEntityAssignmentRecalculationNeeded(true) {
     LOG << "[INFO]   Chosen method \"balance_quantity\": threads will be assigned the same"
         << std::endl
         << "[INFO]   number of tasks, independently of the task length."
         << std::endl;
   }

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
      m_unEntityIterPhaseDoneCounter = CSimulator::GetInstance().GetNumThreads();

      /* Then the mutexes */
      if((nErrors = pthread_mutex_init(&m_tSenseControlStepConditionalMutex, nullptr)) ||
         (nErrors = pthread_mutex_init(&m_tActConditionalMutex, nullptr)) ||
         (nErrors = pthread_mutex_init(&m_tPhysicsConditionalMutex, nullptr)) ||
         (nErrors = pthread_mutex_init(&m_tMediaConditionalMutex, nullptr)) ||
         (nErrors = pthread_mutex_init(&m_tEntityIterConditionalMutex, nullptr))) {
         THROW_ARGOSEXCEPTION("Error creating thread mutexes " << ::strerror(nErrors));
      }
      /* Finally the conditionals */
      if((nErrors = pthread_cond_init(&m_tSenseControlStepConditional, nullptr)) ||
         (nErrors = pthread_cond_init(&m_tActConditional, nullptr)) ||
         (nErrors = pthread_cond_init(&m_tPhysicsConditional, nullptr)) ||
         (nErrors = pthread_cond_init(&m_tMediaConditional, nullptr)) ||
         (nErrors = pthread_cond_init(&m_tEntityIterConditional, nullptr))) {
         THROW_ARGOSEXCEPTION("Error creating thread conditionals " << ::strerror(nErrors));
      }
      /* Start threads */
      StartThreads();
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceQuantity::StartThreads() {
      m_psUpdateThreadData = new SUpdateThreadData*[GetNumThreads()];
      /* Create the threads */

      for(UInt32 i = 0; i < GetNumThreads(); ++i) {
         /* Create the struct with the info to launch the thread */
         m_psUpdateThreadData[i] = new SUpdateThreadData(i, this);
         /* Create the thread */
         CreateSingleThread(i,
                            LaunchUpdateThreadBalanceQuantity,
                            reinterpret_cast<void*>(m_psUpdateThreadData[i]));
      }
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceQuantity::Destroy() {
      /* Destroy the threads to update the controllable entities */
      DestroyAllThreads();
      /* Destroy the thread launch info */
      if(m_psUpdateThreadData != nullptr) {
         for(UInt32 i = 0; i < CSimulator::GetInstance().GetNumThreads(); ++i) {
            delete m_psUpdateThreadData[i];
         }
      }
      delete[] m_psUpdateThreadData;
      pthread_mutex_destroy(&m_tSenseControlStepConditionalMutex);
      pthread_mutex_destroy(&m_tActConditionalMutex);
      pthread_mutex_destroy(&m_tPhysicsConditionalMutex);
      pthread_mutex_destroy(&m_tMediaConditionalMutex);
      pthread_mutex_destroy(&m_tEntityIterConditionalMutex);

      pthread_cond_destroy(&m_tSenseControlStepConditional);
      pthread_cond_destroy(&m_tActConditional);
      pthread_cond_destroy(&m_tPhysicsConditional);
      pthread_cond_destroy(&m_tMediaConditional);
      pthread_cond_destroy(&m_tEntityIterConditional);

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

   void CSpaceMultiThreadBalanceQuantity::IterateOverControllableEntities(
       const TControllableEntityIterCBType &c_cb) {
     m_cbControllableEntityIter = c_cb;

     /* Iterate over all robots in the swarm */
     MAIN_SEND_GO_FOR_PHASE(EntityIter);
     MAIN_WAIT_FOR_PHASE_END(EntityIter);
   } /* IterateOverControllableEntities() */

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceQuantity::ControllableEntityIterationWaitAbort(void) {
     IterateOverControllableEntities(nullptr);
   } /* ControllableEntitiesIterationWaitAbort() */


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
      sCancelData.EntityIterConditionalMutex = &m_tEntityIterConditionalMutex;

      pthread_cleanup_push(CleanupUpdateThread, &sCancelData);

      /* Id range for the physics engines assigned to this thread */
      CRange<size_t> cPhysicsRange = CalculatePluginRangeForThread(unId,
                                                                   m_ptPhysicsEngines->size());
      /* Id range for the media assigned to this thread */
      CRange<size_t> cMediaRange = CalculatePluginRangeForThread(unId,
                                                                 m_ptMedia->size());

      /*
       * Id range for the entities to update assigned to this thread. Can change
       * as simulation progressesso periodic re-calculation may be necessary
       * before *ANY* phase which iterates over the entities, as the
       * CLoopFunctions::PreStep()/CLoopFunctions::PostStep() may have
       * added/removed entities.
       */
      CRange<size_t> cEntityRange;
      while (1) {
        /* Actuate entities assigned to this thread */
        UpdateThreadEntityAct(un_id, cEntityRange);

        /* Update physics engines assigned to this thread */
        UpdateThreadPhysics(cPhysicsRange);

        /* Update media assigned to this thread */
        UpdateThreadMedia(cMediaRange);

        /* loop functions PreStep() iteration (maybe) */
        UpdateThreadIterateOverEntities(un_id, cEntityRange);

        /* Update sensor readings/execute control step for entities */
        UpdateThreadEntitySenseControl(un_id, cEntityRange);

        /* loop functions PostStep() iteration (maybe) */
        UpdateThreadIterateOverEntities(un_id, cEntityRange);
      } /* while(1) */

      pthread_cleanup_pop(1);
   } /* UpdateThread */

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceQuantity::UpdateThreadEntityAct(UInt32 un_id,
                                                                CRange<size_t>& c_range) {
     THREAD_WAIT_FOR_GO_SIGNAL(Act);
     /* Calculate the portion of entities to update, if needed */
     if (m_bIsControllableEntityAssignmentRecalculationNeeded) {
       c_range = CalculatePluginRangeForThread(un_id,
                                               m_vecControllableEntities.size());
     }
     /* Cope with the fact that there may be less entities than threads */
     if (c_range.GetSpan() > 0) {
       /* This thread has entities */
       /* Actuate control choices */
       for(size_t i = c_range.GetMin(); i < c_range.GetMax(); ++i) {
         if(m_vecControllableEntities[i]->IsEnabled())
           m_vecControllableEntities[i]->Act();
       }
       pthread_testcancel();
       THREAD_SIGNAL_PHASE_DONE(Act);
     }
     else {
       /* This thread has no entities -> dummy computation */
       THREAD_SIGNAL_PHASE_DONE(Act);
     }
   } /* UpdateThreadEntityAct() */

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceQuantity::UpdateThreadPhysics(
       const CRange<size_t>& c_range) {
     /* Update physics engines, if this thread has been assigned to them */
     THREAD_WAIT_FOR_GO_SIGNAL(Physics);
     if (c_range.GetSpan() > 0) {
       /* This thread has engines, update them */
       for (size_t i = c_range.GetMin(); i < c_range.GetMax(); ++i) {
         (*m_ptPhysicsEngines)[i]->Update();
       }
       pthread_testcancel();
       THREAD_SIGNAL_PHASE_DONE(Physics);
     }
     else {
       /* This thread has no engines -> dummy computation */
       THREAD_SIGNAL_PHASE_DONE(Physics);
     }
   } /* UpdateThreadPhysics() */

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceQuantity::UpdateThreadMedia(
       const CRange<size_t>& c_range) {
     /* Update media, if this thread has been assigned to them */
     THREAD_WAIT_FOR_GO_SIGNAL(Media);
     if(c_range.GetSpan() > 0) {
       /* This thread has media, update them */
       for(size_t i = c_range.GetMin(); i < c_range.GetMax(); ++i) {
         (*m_ptMedia)[i]->Update();
       }
       pthread_testcancel();
       THREAD_SIGNAL_PHASE_DONE(Media);
     }
     else {
       /* This thread has no media -> dummy computation */
       THREAD_SIGNAL_PHASE_DONE(Media);
     }
   } /* UpdateThreadMedia() */

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceQuantity::UpdateThreadIterateOverEntities(UInt32 un_id,
                                                                          CRange<size_t>& c_range) {
     THREAD_WAIT_FOR_GO_SIGNAL(EntityIter);
     /* Calculate the portion of entities to update, if needed */
     if (m_bIsControllableEntityAssignmentRecalculationNeeded) {
       c_range = CalculatePluginRangeForThread(un_id,
                                               m_vecControllableEntities.size());
     }
     /* Cope with the fact that there may be less entities than threads */
     if (c_range.GetSpan() > 0 && ControllableEntityIterationEnabled()) {
       /* This thread has entities */
       for (size_t i = c_range.GetMin(); i < c_range.GetMax(); ++i) {
         m_cbControllableEntityIter(m_vecControllableEntities[i]);
       } /* for(i...) */
       pthread_testcancel();
       THREAD_SIGNAL_PHASE_DONE(EntityIter);
     }
     else {
       THREAD_SIGNAL_PHASE_DONE(EntityIter);
     }
   } /* UpdateThreadIterateOverEntities() */

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadBalanceQuantity::UpdateThreadEntitySenseControl(UInt32 un_id,
                                                                         CRange<size_t>& c_range) {
     /* Update sensor readings and call controllers */
     THREAD_WAIT_FOR_GO_SIGNAL(SenseControlStep);

     /* Calculate the portion of entities to update, if needed */
     if (m_bIsControllableEntityAssignmentRecalculationNeeded) {
       c_range = CalculatePluginRangeForThread(un_id,
                                               m_vecControllableEntities.size());
     }
     /* Cope with the fact that there may be less entities than threads */
     if (c_range.GetSpan() > 0) {
       /* This thread has entities */
       for (size_t i = c_range.GetMin(); i < c_range.GetMax(); ++i) {
         if (m_vecControllableEntities[i]->IsEnabled()) {
           m_vecControllableEntities[i]->Sense();
           m_vecControllableEntities[i]->ControlStep();
         }
       }
       pthread_testcancel();
       THREAD_SIGNAL_PHASE_DONE(SenseControlStep);
     }
     else {
       /* This thread has no entities -> dummy computation */
       THREAD_SIGNAL_PHASE_DONE(SenseControlStep);
     }
   } /* UpdateThreadEntitySenseControl() */

   /****************************************/
   /****************************************/
}
