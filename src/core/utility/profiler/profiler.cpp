/**
 * @file <argos3/core/utility/profiler/profiler.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "profiler.h"
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/configuration/argos_configuration.h>

namespace argos {

   /****************************************/
   /****************************************/

   static double TV2Sec(const ::timeval& t_timeval) {
      return
         static_cast<double>(t_timeval.tv_sec) +
         static_cast<double>(t_timeval.tv_usec) * 0.000001;
   }

   /****************************************/
   /****************************************/

   static double CPUUsage(const ::rusage& t_rusage,
                          double f_elapsed_time) {
      return 100.0 *
         (TV2Sec(t_rusage.ru_utime) +
          TV2Sec(t_rusage.ru_stime)) /
         f_elapsed_time;
   }

   /****************************************/
   /****************************************/

#define DUMP_RUSAGE_FIELD_HUMAN_READABLE(MSG, FIELD) c_os << MSG ": " << t_resources.ru_ ## FIELD << std::endl;
   void DumpResourceUsageHumanReadable(std::ostream& c_os,
                                       const ::rusage& t_resources) {
      double fUserTime = TV2Sec(t_resources.ru_utime);
      c_os << "User time: " << fUserTime << std::endl;
      double fSysTime = TV2Sec(t_resources.ru_stime);
      c_os << "System time: " << fSysTime << std::endl;
      DUMP_RUSAGE_FIELD_HUMAN_READABLE("Maximum resident set size", maxrss);
      DUMP_RUSAGE_FIELD_HUMAN_READABLE("Integral shared memory size", ixrss);
      DUMP_RUSAGE_FIELD_HUMAN_READABLE("Integral unshared data size", idrss);
      DUMP_RUSAGE_FIELD_HUMAN_READABLE("Integral unshared stack size", isrss);
      DUMP_RUSAGE_FIELD_HUMAN_READABLE("Page reclaims", minflt);
      DUMP_RUSAGE_FIELD_HUMAN_READABLE("Page faults", majflt);
      DUMP_RUSAGE_FIELD_HUMAN_READABLE("Swaps", nswap);
      DUMP_RUSAGE_FIELD_HUMAN_READABLE("Block input operations", inblock);
      DUMP_RUSAGE_FIELD_HUMAN_READABLE("Block output operations", oublock);
      DUMP_RUSAGE_FIELD_HUMAN_READABLE("Messages sent", msgsnd);
      DUMP_RUSAGE_FIELD_HUMAN_READABLE("Messages received", msgrcv);
      DUMP_RUSAGE_FIELD_HUMAN_READABLE("Signals received", nsignals);
      DUMP_RUSAGE_FIELD_HUMAN_READABLE("Voluntary context switches", nvcsw);
      DUMP_RUSAGE_FIELD_HUMAN_READABLE("Involuntary context switches", nivcsw);
   }

   /****************************************/
   /****************************************/

#define DUMP_RUSAGE_FIELD_TABLE_ROW(FIELD) c_os << " " << t_resources.ru_ ## FIELD;
   void DumpResourceUsageAsTableRow(std::ostream& c_os,
                                    const ::rusage& t_resources) {
      double fUserTime = TV2Sec(t_resources.ru_utime);
      c_os << fUserTime;
      double fSysTime = TV2Sec(t_resources.ru_stime);
      c_os << " " << fSysTime;
      DUMP_RUSAGE_FIELD_TABLE_ROW(maxrss);
      DUMP_RUSAGE_FIELD_TABLE_ROW(ixrss);
      DUMP_RUSAGE_FIELD_TABLE_ROW(idrss);
      DUMP_RUSAGE_FIELD_TABLE_ROW(isrss);
      DUMP_RUSAGE_FIELD_TABLE_ROW(minflt);
      DUMP_RUSAGE_FIELD_TABLE_ROW(majflt);
      DUMP_RUSAGE_FIELD_TABLE_ROW(nswap);
      DUMP_RUSAGE_FIELD_TABLE_ROW(inblock);
      DUMP_RUSAGE_FIELD_TABLE_ROW(oublock);
      DUMP_RUSAGE_FIELD_TABLE_ROW(msgsnd);
      DUMP_RUSAGE_FIELD_TABLE_ROW(msgrcv);
      DUMP_RUSAGE_FIELD_TABLE_ROW(nsignals);
      DUMP_RUSAGE_FIELD_TABLE_ROW(nvcsw);
      DUMP_RUSAGE_FIELD_TABLE_ROW(nivcsw);
   }

   /****************************************/
   /****************************************/

#define DIFF_RUSAGE_FIELD(FIELD) tResult.ru_ ## FIELD = t_resource1.ru_ ## FIELD - t_resource2.ru_ ## FIELD;
   ::rusage operator-(const ::rusage& t_resource1,
                      const ::rusage& t_resource2) {
      ::rusage tResult;
      DIFF_RUSAGE_FIELD(utime.tv_sec);
      DIFF_RUSAGE_FIELD(utime.tv_usec);
      DIFF_RUSAGE_FIELD(stime.tv_sec);
      DIFF_RUSAGE_FIELD(stime.tv_usec);
      DIFF_RUSAGE_FIELD(maxrss);
      DIFF_RUSAGE_FIELD(ixrss);
      DIFF_RUSAGE_FIELD(idrss);
      DIFF_RUSAGE_FIELD(isrss);
      DIFF_RUSAGE_FIELD(minflt);
      DIFF_RUSAGE_FIELD(majflt);
      DIFF_RUSAGE_FIELD(nswap);
      DIFF_RUSAGE_FIELD(inblock);
      DIFF_RUSAGE_FIELD(oublock);
      DIFF_RUSAGE_FIELD(msgsnd);
      DIFF_RUSAGE_FIELD(msgrcv);
      DIFF_RUSAGE_FIELD(nsignals);
      DIFF_RUSAGE_FIELD(nvcsw);
      DIFF_RUSAGE_FIELD(nivcsw);
      return tResult;
   }

   /****************************************/
   /****************************************/

   CProfiler::CProfiler(const std::string& str_file_name,
                        bool b_trunc) {
      if(b_trunc) {
         m_cOutFile.open(str_file_name.c_str(),
                         std::ios::trunc | std::ios::out);
      }
      else {
         m_cOutFile.open(str_file_name.c_str(),
                         std::ios::app | std::ios::out);
      }
      LOG << "Opened file \"" << str_file_name << "\" for profiling." << std::endl;
      int nError = pthread_mutex_init(&m_tThreadResourceUsageMutex, NULL);
      if(nError) {
         THROW_ARGOSEXCEPTION("Error creating thread profiler mutex " << ::strerror(nError));
      }
   }

   /****************************************/
   /****************************************/

   CProfiler::~CProfiler() {
      m_cOutFile.close();
      pthread_mutex_destroy(&m_tThreadResourceUsageMutex);
   }

   /****************************************/
   /****************************************/

   void CProfiler::Start() {
      StartWallClock();
      StartCPUProfiler();
   }

   /****************************************/
   /****************************************/

   void CProfiler::Stop() {
      StopCPUProfiler();
      StopWallClock();
   }

   /****************************************/
   /****************************************/

   void CProfiler::Flush(bool b_human_readable) {
      if(b_human_readable) {
         FlushHumanReadable();
      }
      else {
         FlushAsTable();
      }
   }

   /****************************************/
   /****************************************/

   void CProfiler::CollectThreadResourceUsage() {
      pthread_mutex_lock(&m_tThreadResourceUsageMutex);
      ::rusage tResourceUsage;
#ifdef __APPLE__
      /* FIXME: This is badly broken, but I don't know how to obtain
	 the equivalent of RUSAGE_THREAD on MacOS. --tc */
      getrusage(RUSAGE_SELF, &tResourceUsage);
#else
      ::getrusage(::RUSAGE_THREAD, &tResourceUsage);
#endif
      m_vecThreadResourceUsage.push_back(tResourceUsage);
      pthread_mutex_unlock(&m_tThreadResourceUsageMutex);
   }

   /****************************************/
   /****************************************/

   void CProfiler::FlushHumanReadable() {
      m_cOutFile << "[profiled portion overall]" << std::endl << std::endl;
      double fStartTime = TV2Sec(m_tWallClockStart);
      double fEndTime = TV2Sec(m_tWallClockEnd);
      double fElapsedTime = fEndTime - fStartTime;
      m_cOutFile << "Wall clock time: " << fElapsedTime << std::endl;
      ::rusage tDiffResourceUsage = m_tResourceUsageEnd - m_tResourceUsageStart;
      m_cOutFile << "CPU usage: " << CPUUsage(tDiffResourceUsage, fElapsedTime) << "%" << std::endl;
      DumpResourceUsageHumanReadable(m_cOutFile, tDiffResourceUsage);
      m_cOutFile << std::endl << "[profiled portion start]" << std::endl << std::endl;
      DumpResourceUsageHumanReadable(m_cOutFile, m_tResourceUsageStart);
      m_cOutFile << std::endl << "[profiled portion end]" << std::endl << std::endl;
      DumpResourceUsageHumanReadable(m_cOutFile, m_tResourceUsageEnd);
      m_cOutFile << std::endl << "[process overall]" << std::endl << std::endl;
      ::rusage tResourceUsage;
#ifdef __APPLE__
      getrusage(RUSAGE_SELF, &tResourceUsage);
#else
      ::getrusage(::RUSAGE_SELF, &tResourceUsage);
#endif
      DumpResourceUsageHumanReadable(m_cOutFile, tResourceUsage);
      if(! m_vecThreadResourceUsage.empty()) {
         for(size_t i = 0; i < m_vecThreadResourceUsage.size(); ++i) {
            m_cOutFile << std::endl << "[thread #" << i << " overall]" << std::endl << std::endl;
            m_cOutFile << "CPU usage: " << CPUUsage(m_vecThreadResourceUsage[i], fElapsedTime) << "%" << std::endl;
            DumpResourceUsageHumanReadable(m_cOutFile, m_vecThreadResourceUsage[i]);
         }
      }
   }

   /****************************************/
   /****************************************/

   void CProfiler::FlushAsTable() {
      double fStartTime = TV2Sec(m_tWallClockStart);
      double fEndTime = TV2Sec(m_tWallClockEnd);
      double fElapsedTime = fEndTime - fStartTime;
      ::rusage tDiffResourceUsage = m_tResourceUsageEnd - m_tResourceUsageStart;
      m_cOutFile << "ProfPortion " << fElapsedTime << " "
                 << CPUUsage(tDiffResourceUsage, fElapsedTime) << " ";
      DumpResourceUsageAsTableRow(m_cOutFile, tDiffResourceUsage);
      m_cOutFile << std::endl << "PortionStart 0 0 ";
      DumpResourceUsageAsTableRow(m_cOutFile, m_tResourceUsageStart);
      m_cOutFile << std::endl << "PortionEnd 0 0 ";
      DumpResourceUsageAsTableRow(m_cOutFile, m_tResourceUsageEnd);
      m_cOutFile << std::endl << "Overall 0 0 ";
      ::rusage tResourceUsage;
#ifdef __APPLE__
      getrusage(RUSAGE_SELF, &tResourceUsage);
#else
      ::getrusage(::RUSAGE_SELF, &tResourceUsage);
#endif
      DumpResourceUsageAsTableRow(m_cOutFile, tResourceUsage);
      if(! m_vecThreadResourceUsage.empty()) {
         for(size_t i = 0; i < m_vecThreadResourceUsage.size(); ++i) {
            m_cOutFile << std::endl << "thread_" << i << " 0 "
                       << CPUUsage(m_vecThreadResourceUsage[i], fElapsedTime) << " ";
            DumpResourceUsageAsTableRow(m_cOutFile, m_vecThreadResourceUsage[i]);
         }
      }
      m_cOutFile << std::endl;
   }

   /****************************************/
   /****************************************/

   void CProfiler::StartWallClock() {
      ::gettimeofday(&m_tWallClockStart, NULL);
   }

   /****************************************/
   /****************************************/

   void CProfiler::StopWallClock() {
      ::gettimeofday(&m_tWallClockEnd, NULL);
   }

   /****************************************/
   /****************************************/

   void CProfiler::StartCPUProfiler() {
#ifdef __APPLE__
      getrusage(RUSAGE_SELF, &m_tResourceUsageStart);
#else
      ::getrusage(::RUSAGE_SELF, &m_tResourceUsageStart);
#endif
   }

   /****************************************/
   /****************************************/

   void CProfiler::StopCPUProfiler() {
#ifdef __APPLE__
      getrusage(RUSAGE_SELF, &m_tResourceUsageEnd);
#else
      ::getrusage(::RUSAGE_SELF, &m_tResourceUsageEnd);
#endif
   }

   /****************************************/
   /****************************************/

}
