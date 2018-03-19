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
      GetNodeAttributeOrDefault(t_tree, "height", m_fHeight, m_fHeight);
      /* Get the arena center and size */
      TConfigurationNode& tArena = GetNode(CSimulator::GetInstance().GetConfigurationRoot(), "arena");
      GetNodeAttribute(tArena, "size", m_cArenaSize);
      GetNodeAttributeOrDefault(tArena, "center", m_cArenaCenter, m_cArenaCenter);
   } 

   /****************************************/
   /****************************************/

   void CDynamics3DFloorPlugin::Reset() {
      /* Create the floor */
      btVector3 cFloorExtents(m_cArenaSize.GetX(), m_fHeight, m_cArenaSize.GetY());
      btVector3 cFloorOrigin(m_cArenaCenter.GetX(), -m_fHeight * 0.5f, -m_cArenaCenter.GetY());
      new (&m_cFloorShape) btBoxShape(cFloorExtents * 0.5f);
      new (&m_cFloor) btRigidBody(0, nullptr, &m_cFloorShape);
      m_cFloor.setUserPointer(nullptr);
      m_cFloor.getWorldTransform().setOrigin(cFloorOrigin);
      m_pcEngine->GetWorld().addRigidBody(&m_cFloor);
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
