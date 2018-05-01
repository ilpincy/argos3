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
      /* Get the floor height */
      Real fHeight = 0.0f;
      GetNodeAttributeOrDefault(t_tree, "height", fHeight, fHeight);
      /* Get the arena center and size from the arena node */
      CVector3 cArenaCenter(0.0f, 0.0f, 0.0f);
      CVector3 cArenaSize(0.0f, 0.0f, 0.0f);
      TConfigurationNode& tArena = GetNode(CSimulator::GetInstance().GetConfigurationRoot(), "arena");
      GetNodeAttribute(tArena, "size", cArenaSize);
      GetNodeAttributeOrDefault(tArena, "center", cArenaCenter, cArenaCenter);
      /* Configure the floor */
      m_cFloorExtents = btVector3(cArenaSize.GetX(), fHeight, cArenaSize.GetY());
      m_cFloorOrigin = btVector3(cArenaCenter.GetX(), -fHeight * 0.5f, -cArenaCenter.GetY());
      /* Call the destructors for the floor components */ 
      m_cFloor.~btRigidBody();
      m_cFloorShape.~btBoxShape();
      /* Call the constructors for the floor components */
      new (&m_cFloorShape) btBoxShape(m_cFloorExtents * 0.5f);
      new (&m_cFloor) btRigidBody(0, nullptr, &m_cFloorShape);
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
      /* Call the destructors for the floor components */ 
      m_cFloor.~btRigidBody();
      m_cFloorShape.~btBoxShape();
      /* Call the constructors for the floor components */
      new (&m_cFloorShape) btBoxShape(m_cFloorExtents * 0.5f);
      new (&m_cFloor) btRigidBody(0, nullptr, &m_cFloorShape);
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
