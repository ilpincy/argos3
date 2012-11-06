/**
 * @file simulator/space/space_multi_thread_scatter_gather.h
 *
 * @brief This file provides the definition of the Swarmanoid 3D Space.
 *
 * This file provides the definition of the Swarmanoid 3D Space.
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef SPACE_MULTI_THREAD_SCATTER_GATHER_H
#define SPACE_MULTI_THREAD_SCATTER_GATHER_H

#include <argos3/core/simulator/space/space.h>
#include <pthread.h>

namespace argos {

   class CSpaceMultiThreadScatterGather : public CSpace {

      /****************************************/
      /****************************************/
      
   private:
      
      struct SUpdateThreadData {
         UInt32 ThreadId;
         CSpaceMultiThreadScatterGather* Space;
         
         SUpdateThreadData(UInt32 un_thread_id,
                           CSpaceMultiThreadScatterGather* pc_space) :
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

      /** Update thread conditional mutexes */
      pthread_mutex_t m_tSenseControlStepConditionalMutex;
      pthread_mutex_t m_tActConditionalMutex;
      pthread_mutex_t m_tPhysicsConditionalMutex;

      /** Update thread conditionals */
      pthread_cond_t m_tSenseControlStepConditional;
      pthread_cond_t m_tActConditional;
      pthread_cond_t m_tPhysicsConditional;

      /** Flag to know whether the assignment of controllable
          entities to threads must be recalculated */
      bool m_bIsControllableEntityAssignmentRecalculationNeeded;

   public:

      CSpaceMultiThreadScatterGather() :
         m_psUpdateThreadData(NULL),
         m_ptUpdateThreads(NULL),
         m_bIsControllableEntityAssignmentRecalculationNeeded(true) {}
      virtual ~CSpaceMultiThreadScatterGather() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Destroy();

      virtual void SetPhysicsEngines(CPhysicsEngine::TVector& t_engines);

      virtual void UpdateControllableEntities();
      virtual void UpdatePhysics();

   protected:

      virtual void AddControllableEntity(CControllableEntity& c_entity);
      virtual void RemoveControllableEntity(CControllableEntity& c_entity);

   private:

      void StartThreads();
      void UpdateThread(UInt32 un_id);
      friend void* LaunchUpdateThreadScatterGather(void* p_data);

   };

}

#endif
