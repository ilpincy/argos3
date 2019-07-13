/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_floor_plugin.cpp>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#include "dynamics3d_floor_plugin.h"

namespace argos {
   
   /****************************************/
   /****************************************/
   
   void CDynamics3DFloorPlugin::Init(TConfigurationNode& t_tree) {
      /* Get the friction of the floor */
      m_fFriction = m_pcEngine->GetDefaultFriction();
      GetNodeAttributeOrDefault(t_tree, "friction", m_fFriction, m_fFriction);
      /* Call the destructor for the floor body */
      m_cFloor.~btRigidBody();
      /* Call the constructor for the floor body */
      btRigidBody::btRigidBodyConstructionInfo sConstructionInfo(0, nullptr, &m_cFloorShape);
      sConstructionInfo.m_friction = m_fFriction;
      /* Create the floor */
      new (&m_cFloor) btRigidBody(sConstructionInfo);
      m_cFloor.setUserPointer(nullptr);
      m_cFloor.getWorldTransform().setOrigin(m_cFloorOrigin);
      /* Add floor to world */
      m_pcEngine->GetWorld().addRigidBody(&m_cFloor);
   } 

   /****************************************/
   /****************************************/

   void CDynamics3DFloorPlugin::Reset() {
      /* Remove floor from world */
      m_pcEngine->GetWorld().removeRigidBody(&m_cFloor);
      /* Call the destructor for the floor body */ 
      m_cFloor.~btRigidBody();
      /* Call the constructor for the floor body */
      btRigidBody::btRigidBodyConstructionInfo sConstructionInfo(0, nullptr, &m_cFloorShape);
      sConstructionInfo.m_friction = m_fFriction;
      /* Create the floor */
      new (&m_cFloor) btRigidBody(sConstructionInfo);
      m_cFloor.setUserPointer(nullptr);
      m_cFloor.getWorldTransform().setOrigin(m_cFloorOrigin);
      /* Add floor to world */
      m_pcEngine->GetWorld().addRigidBody(&m_cFloor);
   }
   
   /****************************************/
   /****************************************/

   void CDynamics3DFloorPlugin::Destroy() {
      /* Remove floor from world */
      m_pcEngine->GetWorld().removeRigidBody(&m_cFloor);
   }
   
   /****************************************/
   /****************************************/
   
   REGISTER_DYNAMICS3D_PLUGIN(CDynamics3DFloorPlugin,
                              "floor",
                              "Michael Allwright [allsey87@gmail.com]",
                              "1.0",
                              "Inserts a floor into the 3D dynamics engine",
                              "For a description on how to use this plugin, please consult the documentation\n"
                              "for the dynamics3d physics engine plugin",
                              "Usable");

   /****************************************/
   /****************************************/

}
