/**
 * @file core/simulator/visualization/visualization.h
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

#include <string>
#include <map>

namespace argos {

   class CVisualization : public CBaseConfigurableResource {

   public:

      CVisualization() :
         m_cSimulator(CSimulator::GetInstance()),
         m_cSpace(m_cSimulator.GetSpace()) {}

      virtual ~CVisualization() {}

      virtual void Init(TConfigurationNode& t_tree);

      virtual void Reset() {}

      virtual void Destroy() {}

      virtual void Execute();
      
      inline const std::string& GetId() const {
         return m_strId;
      }

      inline void SetId(const std::string& str_id) {
         m_strId = str_id;
      }

   protected:

      /** A reference to the simulator */
      CSimulator& m_cSimulator;

      /** A reference to the space */
      CSpace& m_cSpace;

      /** The id of the visualizationer */
      std::string m_strId;

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
