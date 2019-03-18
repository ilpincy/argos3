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
      std::vector<std::shared_ptr<CDynamics3DModel::CAbstractBody> >& vecBodies = c_model.GetBodies();
      m_vecTargets.insert(std::end(m_vecTargets),
                          std::begin(vecBodies),
                          std::end(vecBodies));
   }

   /****************************************/
   /****************************************/

   void CDynamics3DGravityPlugin::UnregisterModel(CDynamics3DModel& c_model) {
      std::vector<std::shared_ptr<CDynamics3DModel::CAbstractBody> >::iterator itRemove =
         std::remove_if(std::begin(m_vecTargets),
                        std::end(m_vecTargets),
                        [&c_model] (const std::shared_ptr<CDynamics3DModel::CAbstractBody>& ptr_body) {
                           return (&ptr_body->GetModel() == &c_model);
                        });
      m_vecTargets.erase(itRemove, std::end(m_vecTargets));
   }
   
   /****************************************/
   /****************************************/

   void CDynamics3DGravityPlugin::Update() {
      for(std::shared_ptr<CDynamics3DModel::CAbstractBody>& ptr_body : m_vecTargets) {
         btVector3 cGravity(0.0, -m_fAcceleration * ptr_body->GetData().Mass, 0.0);
         ptr_body->ApplyForce(cGravity);
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
