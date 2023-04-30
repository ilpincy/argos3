/**
 * @file <argos3/core/simulator/space/space_multi_thread.h>
 *
 * @brief This file provides the definition of the Swarmanoid 3D Space with
 *        threads.
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#ifndef INCLUDE_SPACE_MULTI_THREAD_H_
#define INCLUDE_SPACE_MULTI_THREAD_H_

#include <vector>

namespace argos {
  class CSpace;
}

#include <argos3/core/simulator/space/space.h>

namespace argos {

   /**
    * @brief Base class for common threading functionality used by @ref
    * CSpaceMultithreadBalanceLength, @ref CSpaceMultithreadBalanceQuantity.
    */
   class CSpaceMultiThread : public CSpace {
    public:

     CSpaceMultiThread(UInt32 un_n_threads, bool b_pin_threads);

     /**
      * @brief Return the # of configured threads. If threads were not
      * specified, returns 0.
      */
     inline UInt32 GetNumThreads() const { return m_vecThreads.size(); }

    protected:

     /**
      * @brief After ARGoS finishes running the experiment, dispose of all
      * threads, which are no longer needed.
      */
     void DestroyAllThreads();

     /**
      * @brief During initialization, create a single thread.
      *
      * @param unThreadId The index/ID of the thread.
      * @param start_routine Pointer to the routine that the thread should start
      *                      execution in.
      * @param arg The single argument which will be passed to @p
      *            start_routine. Can be NULL.
      */

     void CreateSingleThread(UInt32 unThreadId,
                             void *(*start_routine) (void *),
                             void *arg);

    private:

     /** Should threads be pinned to cores ? */
     bool m_bPinThreadsToCores;

     /** The slave thread array */
     std::vector<pthread_t> m_vecThreads;
   };

}

#endif /* INCLUDE_SPACE_MULTI_THREAD_H_ */
