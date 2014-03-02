/**
 * @file <argos3/core/simulator/visualization/default_visualization.h>
 *
 * @brief This file contains the definition of the default (non-interactive) ARGoS visualization.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DEFAULT_VISUALIZATION_H
#define DEFAULT_VISUALIZATION_H

namespace argos {
   class CDefaultVisualization;
}

#include <argos3/core/simulator/visualization/visualization.h>
#include <sys/time.h>

namespace argos {

   class CDefaultVisualization : public CVisualization {

   public:

      CDefaultVisualization();

      virtual ~CDefaultVisualization() {}

      virtual void Init(TConfigurationNode& t_tree) {}

      virtual void Reset() {}

      virtual void Destroy() {}

      virtual void Execute();
      
   private:

      /** Performs a simulation step the normal way */
      void NormalStep();

      /** Performs a simulation step respecting the real-time constraint */
      void RealTimeStep();

   private:

      typedef void (CDefaultVisualization::*TStepFunction)();

   private:

      /** Pointer to step function */
      TStepFunction m_tStepFunction;

      /** The length of a clock tick */
      ::timeval m_tStepClockTime;

      /** The starting time of a clock tick */
      ::timeval m_tStepStartTime;

      /** The end time of a clock tick*/
      ::timeval m_tStepEndTime;

      /** The elapsed time between start and end of a clock tick */
      ::timeval m_tStepElapsedTime;

      /** The time to wait to complete a clock tick */
      ::timeval m_tStepWaitTime;

   };

}

#endif
