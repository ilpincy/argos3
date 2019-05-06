/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_floor_plugin.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 */

#ifndef DYNAMICS3D_FLOOR_PLUGIN_H
#define DYNAMICS3D_FLOOR_PLUGIN_H

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_plugin.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_model.h>
#include <argos3/core/utility/datatypes/datatypes.h>

namespace argos {
   
   /****************************************/
   /****************************************/
   
   class CDynamics3DFloorPlugin : public CDynamics3DPlugin {
   public:
      CDynamics3DFloorPlugin() :
         m_cFloorOrigin(0.0f, 0.0f, 0.0f),
         m_cFloorShape(btVector3(0.0f, 1.0f, 0.0f), 0.0f),
         m_cFloor(0.0f, nullptr, nullptr) {}
      
      ~CDynamics3DFloorPlugin() {}
      
      virtual void Init(TConfigurationNode& t_tree);
      
      virtual void Reset();

      virtual void Destroy();

      virtual void RegisterModel(CDynamics3DModel& c_model) {}

      virtual void UnregisterModel(CDynamics3DModel& c_model) {}
      
      virtual void Update() {}

   private:

      btScalar m_fFriction;
      btVector3 m_cFloorOrigin;
      btStaticPlaneShape m_cFloorShape;
      btRigidBody m_cFloor;
   };
   
   /****************************************/
   /****************************************/
   
}

#endif
