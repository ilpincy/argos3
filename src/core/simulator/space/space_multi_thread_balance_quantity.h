/**
 * @file <argos3/core/simulator/space/space_multi_thread_balance_quantity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef SPACE_MULTI_THREAD_BALANCE_QUANTITY_H
#define SPACE_MULTI_THREAD_BALANCE_QUANTITY_H

#include <argos3/core/simulator/space/space.h>
#include <pthread.h>

namespace argos {

   class CSpaceMultiThreadBalanceQuantity : public CSpace {

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

      /** The update threads */
      pthread_t* m_ptUpdateThreads;

      /** Update thread related variables */
      UInt32 m_unSenseControlStepPhaseDoneCounter;
      UInt32 m_unActPhaseDoneCounter;
      UInt32 m_unPhysicsPhaseDoneCounter;
      UInt32 m_unMediaPhaseDoneCounter;

      /** Update thread conditional mutexes */
      pthread_mutex_t m_tSenseControlStepConditionalMutex;
      pthread_mutex_t m_tActConditionalMutex;
      pthread_mutex_t m_tPhysicsConditionalMutex;
      pthread_mutex_t m_tMediaConditionalMutex;

      /** Update thread conditionals */
      pthread_cond_t m_tSenseControlStepConditional;
      pthread_cond_t m_tActConditional;
      pthread_cond_t m_tPhysicsConditional;
      pthread_cond_t m_tMediaConditional;

      /** Flag to know whether the assignment of controllable
          entities to threads must be recalculated */
      bool m_bIsControllableEntityAssignmentRecalculationNeeded;

   public:

      CSpaceMultiThreadBalanceQuantity();
      virtual ~CSpaceMultiThreadBalanceQuantity() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Destroy();

      virtual void UpdateControllableEntitiesAct();
      virtual void UpdatePhysics();
      virtual void UpdateMedia();
      virtual void UpdateControllableEntitiesSenseStep();

   protected:

      virtual void AddControllableEntity(CControllableEntity& c_entity);
      virtual void RemoveControllableEntity(CControllableEntity& c_entity);

   private:

      void StartThreads();
      void UpdateThread(UInt32 un_id);
      friend void* LaunchUpdateThreadBalanceQuantity(void* p_data);

   };

}

#endif
