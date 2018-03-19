/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_gravity_plugin.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_gravity_plugin.h"

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_model.h>
#include <algorithm>
#include <vector>

namespace argos {
   
   /****************************************/
   /****************************************/
   
   void CDynamics3DGravityPlugin::Init(TConfigurationNode& t_tree) {
      GetNodeAttributeOrDefault(t_tree, "g", m_fAcceleration, m_fAcceleration);
   } 

   /****************************************/
   /****************************************/

   void CDynamics3DGravityPlugin::RegisterModel(CDynamics3DModel& c_model) {
      for(CDynamics3DModel::CAbstractBody* pc_body : c_model.GetBodies()) {
         m_vecTargets.push_back(pc_body);
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics3DGravityPlugin::UnregisterModel(CDynamics3DModel& c_model) {
      std::vector<CDynamics3DModel::CAbstractBody*>::iterator itRemove =
         std::remove_if(std::begin(m_vecTargets),
                        std::end(m_vecTargets),
                        [&c_model] (CDynamics3DModel::CAbstractBody* pc_body) {
                           return(&(pc_body->GetModel()) == &c_model);
                        });
      m_vecTargets.erase(itRemove, std::end(m_vecTargets));
   }
   
   /****************************************/
   /****************************************/

   void CDynamics3DGravityPlugin::Update() {
      for(CDynamics3DModel::CAbstractBody* pc_body : m_vecTargets) {
         btVector3 cGravity(0.0, -m_fAcceleration * pc_body->GetData().Mass, 0.0);
         pc_body->ApplyForce(cGravity);
      }
   }
   
   /****************************************/
   /****************************************/
   
   REGISTER_DYNAMICS3D_PLUGIN(CDynamics3DGravityPlugin,
                              "gravity",
                              "Michael Allwright [allsey87@gmail.com]",
                              "1.0",
                              "Applies a gravity force to all bodies in the simulation",
                              "For a description on how to use this plugin, please consult the documentation\n"
                              "for the dynamics3d physics engine plugin",
                              "Usable");

   /****************************************/
   /****************************************/

}
