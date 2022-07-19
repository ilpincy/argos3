/**
 * @file <argos3/core/simulator/space/space_multi_thread.cpp>
 *
 * @author John Harwell - <john.r.harwell@gmail.com>
 */

#include <argos3/core/simulator/space/space_multi_thread.h>

namespace argos {

   /****************************************/
   /****************************************/

   CSpaceMultiThread::CSpaceMultiThread(UInt32 un_n_threads,
                                        bool b_pin_threads_to_cores) :
       m_bPinThreadsToCores(b_pin_threads_to_cores),
       m_vecThreads(un_n_threads) {
     LOG << "[INFO] Using " << GetNumThreads() << " parallel threads" << std::endl;
     if (m_bPinThreadsToCores) {
       LOG << "[INFO]   Pinning threads to cores by thread ID" << std::endl;
     }
   }

   /****************************************/
   /****************************************/
   void CSpaceMultiThread::DestroyAllThreads(void) {
     int nErrors;
     if (!m_vecThreads.empty()) {
       for(UInt32 i = 0; i < GetNumThreads(); ++i) {
         if((nErrors = pthread_cancel(m_vecThreads[i]))) {
           THROW_ARGOSEXCEPTION("Error canceling threads " << ::strerror(nErrors));
         }
       }
       auto** ppJoinResult = new void*[GetNumThreads()];
       for(UInt32 i = 0; i < GetNumThreads(); ++i) {
         if((nErrors = pthread_join(m_vecThreads[i], ppJoinResult + i))) {
           THROW_ARGOSEXCEPTION("Error joining threads " << ::strerror(nErrors));
         }
         if(ppJoinResult[i] != PTHREAD_CANCELED) {
           LOGERR << "[WARNING] Thread #" << i<< " not canceled" << std::endl;
         }
       }
       delete[] ppJoinResult;
     }
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThread::CreateSingleThread(UInt32 unThreadId,
                                              void *(*start_routine) (void *),
                                              void *arg) {
     if (unThreadId > GetNumThreads()) {
       THROW_ARGOSEXCEPTION("Cannot create thread " << unThreadId << ": Id out of range");
     }
     /* Create the thread */
     int nErrors = pthread_create(&m_vecThreads[unThreadId],
                                  nullptr,
                                  start_routine,
                                  arg);
     if(nErrors != 0) {
       THROW_ARGOSEXCEPTION("Error creating thread: " << ::strerror(nErrors));
     }
     /* pin thread i to core i */
     if (m_bPinThreadsToCores) {
#if !defined(__APPLE__)
       cpu_set_t cpuset;
       CPU_ZERO(&cpuset);
       CPU_SET(unThreadId, &cpuset);
       int rc = pthread_setaffinity_np(m_vecThreads[unThreadId],
                                       sizeof(cpu_set_t),
                                       &cpuset);
       if (rc != 0) {
         THROW_ARGOSEXCEPTION("Error setting thread " << unThreadId << " affinity: " << ::strerror(rc));
       }
#else
       LOGERR << "[WARNING] Thread pinning not available on OSX!" << std::endl;
#endif /* __APPLE__ */
     }
   }

}
