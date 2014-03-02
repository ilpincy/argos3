/**
 * @file <argos3/core/simulator/visualization/visualization.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include <argos3/core/simulator/visualization/default_visualization.h>
#include <argos3/core/simulator/space/space.h>

#include <unistd.h>

namespace argos {

   /****************************************/
   /****************************************/

   static Real TVTimeToHumanReadable(::timeval& t_time) {
      return
         static_cast<Real>(t_time.tv_sec) +
         static_cast<Real>(t_time.tv_usec * 10e-6);
   }

   /****************************************/
   /****************************************/

   CDefaultVisualization::CDefaultVisualization() {
      fprintf(stderr, "[DEBUG] CDefaultVisualization() - START\n");
      /* Set the pointer to the step function */
      if(m_cSimulator.IsRealTimeClock()) {
         /* Use real-time clock and set time structures */
         m_tStepFunction = &CDefaultVisualization::RealTimeStep;
         timerclear(&m_tStepClockTime);
         m_tStepClockTime.tv_usec = 1e6 * CPhysicsEngine::GetSimulationClockTick();
         ::gettimeofday(&m_tStepStartTime, NULL);
      }
      else {
         /* Use normal clock */
         m_tStepFunction = &CDefaultVisualization::NormalStep;
      }
      fprintf(stderr, "[DEBUG] CDefaultVisualization() - END\n");
   }

   /****************************************/
   /****************************************/

   void CDefaultVisualization::Execute() {
      /* Main cycle */
      fprintf(stderr, "[DEBUG] Execute() - START\n");
      while(!m_cSimulator.IsExperimentFinished()) {
         (this->*m_tStepFunction)();
      }
      fprintf(stderr, "[DEBUG] Execute() - END\n");
   }

   /****************************************/
   /****************************************/

   void CDefaultVisualization::NormalStep() {
      m_cSimulator.UpdateSpace();
   }

   /****************************************/
   /****************************************/

   void CDefaultVisualization::RealTimeStep() {
      fprintf(stderr, "[DEBUG] RealTimeStep() - START\n");
      /* m_tStepStartTime has already been set */
      m_cSimulator.UpdateSpace();
      fprintf(stderr, "[DEBUG] RealTimeStep() - UpdateSpace() done\n");
      /* Take the time now */
      ::gettimeofday(&m_tStepEndTime, NULL);
      /* Calculate the elapsed time */
      timersub(&m_tStepEndTime, &m_tStepStartTime, &m_tStepElapsedTime);
      /* If the elapsed time is lower than the tick length, wait */
      if(!timercmp(&m_tStepElapsedTime, &m_tStepClockTime, >)) {
         /* Calculate the waiting time */
         timersub(&m_tStepClockTime, &m_tStepElapsedTime, &m_tStepWaitTime);
         fprintf(stderr, "[DEBUG] m_tStepElapsedTime.tv_sec = %ld\n", m_tStepElapsedTime.tv_sec);
         fprintf(stderr, "[DEBUG] m_tStepElapsedTime.tv_usec = %ld\n", m_tStepElapsedTime.tv_usec);
         fprintf(stderr, "[DEBUG] m_tStepWaitTime.tv_sec = %ld\n", m_tStepWaitTime.tv_sec);
         fprintf(stderr, "[DEBUG] m_tStepWaitTime.tv_usec = %ld\n", m_tStepWaitTime.tv_usec);
         /* Wait */
         ::usleep(m_tStepWaitTime.tv_sec * 1e6 + m_tStepWaitTime.tv_usec);
         /* Get the new step end */
         ::gettimeofday(&m_tStepEndTime, NULL);
      }
      else {
         LOGERR << "[WARNING] Clock tick took "
                << TVTimeToHumanReadable(m_tStepElapsedTime)
                << " sec, more than the expected "
                << TVTimeToHumanReadable(m_tStepClockTime)
                << " sec."
                << std::endl;
      }
      /* Set the step start time to whatever the step end time is */
      m_tStepStartTime.tv_sec = m_tStepEndTime.tv_sec;
      m_tStepStartTime.tv_usec = m_tStepEndTime.tv_usec;
      fprintf(stderr, "[DEBUG] RealTimeStep() - END\n\n");
   }

   /****************************************/
   /****************************************/

}
