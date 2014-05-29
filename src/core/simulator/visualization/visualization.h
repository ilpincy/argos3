/**
 * @file <argos3/core/simulator/visualization/visualization.h>
 *
 * @brief This file contains the definition of the CVisualization interface.
 *
 * This file contains the definition of the CVisualization interface. Such interface
 * is the base for all the visualisations in ARGoS.
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef VISUALIZATION_H
#define VISUALIZATION_H

namespace argos {
   class CVisualization;
}

#include <argos3/core/utility/configuration/base_configurable_resource.h>
#include <argos3/core/simulator/simulator.h>

namespace argos {

   class CVisualization : public CBaseConfigurableResource {

   public:

      CVisualization() :
         m_cSimulator(CSimulator::GetInstance()),
         m_cSpace(m_cSimulator.GetSpace()) {}

      virtual ~CVisualization() {}

      virtual void Init(TConfigurationNode& t_tree) = 0;

      virtual void Reset() = 0;

      virtual void Destroy() = 0;

      virtual void Execute() = 0;
      
   protected:

      /** A reference to the simulator */
      CSimulator& m_cSimulator;

      /** A reference to the space */
      CSpace& m_cSpace;

   };

#define REGISTER_VISUALIZATION(CLASSNAME,           \
                               LABEL,               \
                               AUTHOR,              \
                               VERSION,             \
                               BRIEF_DESCRIPTION,   \
                               LONG_DESCRIPTION,    \
                               STATUS)              \
   REGISTER_SYMBOL(CVisualization,                  \
                   CLASSNAME,                       \
                   LABEL,                           \
                   AUTHOR,                          \
                   VERSION,                         \
                   BRIEF_DESCRIPTION,               \
                   LONG_DESCRIPTION,                \
                   STATUS)

}

#endif
