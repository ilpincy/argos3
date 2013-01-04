/**
 * @file <argos3/core/utility/profiler/profiler.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */
#ifndef PROFILER_H
#define PROFILER_H

#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace argos {

   class CProfiler {

   public:

      CProfiler(const std::string& str_file_name,
                bool b_trunc=true);
      ~CProfiler();

      void Start();
      void Stop();
      void Flush(bool b_human_readable);
      void CollectThreadResourceUsage();

   private:

      void StartWallClock();
      void StopWallClock();

      void StartCPUProfiler();
      void StopCPUProfiler();

      void FlushHumanReadable();
      void FlushAsTable();

   private:

      std::ofstream m_cOutFile;
      ::timeval m_tWallClockStart;
      ::timeval m_tWallClockEnd;
      ::rusage m_tResourceUsageStart;
      ::rusage m_tResourceUsageEnd;
      std::vector< ::rusage > m_vecThreadResourceUsage;
      pthread_mutex_t m_tThreadResourceUsageMutex;

   };

}

#endif
