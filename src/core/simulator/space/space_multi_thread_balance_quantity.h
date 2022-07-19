/**
 * @file <argos3/core/simulator/space/space_multi_thread_balance_quantity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef SPACE_MULTI_THREAD_BALANCE_QUANTITY_H
#define SPACE_MULTI_THREAD_BALANCE_QUANTITY_H

#include <argos3/core/simulator/space/space_multi_thread.h>
#include <pthread.h>

namespace argos {

   class CSpaceMultiThreadBalanceQuantity : public CSpaceMultiThread {

      /****************************************/
      /****************************************/

   private:

      struct SUpdateThreadData {
         UInt32 ThreadId;
         CSpaceMultiThreadBalanceQuantity* Space;

         SUpdateThreadData(UInt32 un_thread_id,
                           CSpaceMultiThreadBalanceQuantity* pc_space) :
            ThreadId(un_thread_id),
            Space(pc_space) {}
      };

      /****************************************/
      /****************************************/

   private:

      /** Data structure needed to launch the threads */
      SUpdateThreadData** m_psUpdateThreadData;

      /** Update thread related variables */
      UInt32 m_unSenseControlStepPhaseDoneCounter;
      UInt32 m_unActPhaseDoneCounter;
      UInt32 m_unPhysicsPhaseDoneCounter;
      UInt32 m_unMediaPhaseDoneCounter;
      UInt32 m_unEntityIterPhaseDoneCounter;

      /** Update thread conditional mutexes */
      pthread_mutex_t m_tSenseControlStepConditionalMutex;
      pthread_mutex_t m_tActConditionalMutex;
      pthread_mutex_t m_tPhysicsConditionalMutex;
      pthread_mutex_t m_tMediaConditionalMutex;
      pthread_mutex_t m_tEntityIterConditionalMutex;

      /** Update thread conditionals */
      pthread_cond_t m_tSenseControlStepConditional;
      pthread_cond_t m_tActConditional;
      pthread_cond_t m_tPhysicsConditional;
      pthread_cond_t m_tMediaConditional;
      pthread_cond_t m_tEntityIterConditional;

      /** Flag to know whether the assignment of controllable
          entities to threads must be recalculated */
      bool m_bIsControllableEntityAssignmentRecalculationNeeded;

   public:

      CSpaceMultiThreadBalanceQuantity(UInt32 un_n_threads,
                                       bool b_pin_threads_to_cores);
      virtual ~CSpaceMultiThreadBalanceQuantity() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Destroy();

      virtual void UpdateControllableEntitiesAct();
      virtual void UpdatePhysics();
      virtual void UpdateMedia();
      virtual void UpdateControllableEntitiesSenseStep();
      virtual void IterateOverControllableEntities(
          const TControllableEntityIterCBType& c_cb);

   protected:

      virtual void AddControllableEntity(CControllableEntity& c_entity);
      virtual void RemoveControllableEntity(CControllableEntity& c_entity);

   private:

      void StartThreads();
      void UpdateThread(UInt32 un_id);

     /**
      * \brief Actuate entities assigned to this thread, possibly updating the
      * entity range assigned to this thread before doing so.
      */
      void UpdateThreadEntityAct(UInt32 un_id, CRange<size_t>& c_range);

     /**
      * \brief Update the physics engines assigned to this thread (static
      * assignment throughout simulation).
      */
      void UpdateThreadPhysics(const CRange<size_t>& c_range);

     /**
      * \brief Update the media engines assigned to this thread (static
      * assignment throughout simulation).
      */
     void UpdateThreadMedia(const CRange<size_t>& c_range);

     /**
      * \brief (Maybe) iterate over entities as called from
      * CLoopFunctions::PreStep()/CLoopFunctions::PostStep(), possibly
      * recalculating entity range assigned to this thread before doing so.
      *
      * @see CLoopFunctions::PreStep()
      * @see CLoopFunctions::PostStep()
      */
      void UpdateThreadIterateOverEntities(UInt32 un_id, CRange<size_t>& c_range);

     /**
      * \brief Update sensor readings and call controllers assigned to a
      * particular thread, possibly recalculating entity range assigned to this
      * thread before doing so.
      */
      void UpdateThreadEntitySenseControl(UInt32 un_id, CRange<size_t>& c_range);

      virtual void ControllableEntityIterationWaitAbort();

      friend void* LaunchUpdateThreadBalanceQuantity(void* p_data);

   };

}

#endif
