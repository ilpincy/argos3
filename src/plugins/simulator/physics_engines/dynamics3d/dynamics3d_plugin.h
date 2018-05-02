/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_plugin.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_PLUGIN_H
#define DYNAMICS3D_PLUGIN_H

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/plugins/factory.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_engine.h>

namespace argos {

   /****************************************/
   /****************************************/

   class CDynamics3DPlugin {

   public:

      using TMap = std::map<std::string, CDynamics3DPlugin*>;

   public:
      CDynamics3DPlugin() :
         m_pcEngine(nullptr) {}

      virtual ~CDynamics3DPlugin() {}
      
      virtual void Init(TConfigurationNode& t_tree) {}

      virtual void Reset() {}

      virtual void Destroy() {}

      virtual void SetEngine(CDynamics3DEngine& c_engine) {
         m_pcEngine = &c_engine;
      }

      virtual void RegisterModel(CDynamics3DModel& c_model) = 0;

      virtual void UnregisterModel(CDynamics3DModel& c_model) = 0;

      virtual void Update() = 0;

   protected:
      CDynamics3DEngine* m_pcEngine;
   };

   /****************************************/
   /****************************************/
   
}

#define REGISTER_DYNAMICS3D_PLUGIN(CLASSNAME,         \
                                   LABEL,             \
                                   AUTHOR,            \
                                   VERSION,           \
                                   BRIEF_DESCRIPTION, \
                                   LONG_DESCRIPTION,  \
                                   STATUS)            \
   REGISTER_SYMBOL(CDynamics3DPlugin,                 \
                   CLASSNAME,                         \
                   LABEL,                             \
                   AUTHOR,                            \
                   VERSION,                           \
                   BRIEF_DESCRIPTION,                 \
                   LONG_DESCRIPTION,                  \
                   STATUS)

#endif
