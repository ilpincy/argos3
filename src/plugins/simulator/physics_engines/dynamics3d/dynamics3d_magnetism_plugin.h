/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_magnetism_plugin.h>
 *
 * @author Michael Allwright - <allsey87@gmail.com>
 * @author Haitham Elfaham - <haithamelfaham@gmail.com>
 * @author Weixu Zhu - <zhuweixu_harry@126.com>
 */

#ifndef DYNAMICS3D_MAGNETISM_PLUGIN_H
#define DYNAMICS3D_MAGNETISM_PLUGIN_H

#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_plugin.h>
#include <argos3/plugins/simulator/physics_engines/dynamics3d/dynamics3d_model.h>
#include <argos3/plugins/simulator/entities/magnet_equipped_entity.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <functional>
#include <vector>

namespace argos {
   
   /****************************************/
   /****************************************/
   
   class CDynamics3DMagnetismPlugin : public CDynamics3DPlugin {
   public:
      CDynamics3DMagnetismPlugin() :
         m_fForceConstant(7.0500949e-13),
         m_fMaxDistance(std::numeric_limits<Real>::infinity()) {}
      
      ~CDynamics3DMagnetismPlugin() {}
      
      virtual void Init(TConfigurationNode& t_tree);
      
      virtual void Reset() {}
      
      virtual void Destroy() {}

      virtual void RegisterModel(CDynamics3DModel& c_model);
      
      virtual void UnregisterModel(CDynamics3DModel& c_model);
      
      virtual void Update();

   private:
      
      btScalar m_fForceConstant;
      btScalar m_fMaxDistance;

      struct SMagneticDipole {
         /* Constructor */
         SMagneticDipole(std::shared_ptr<CDynamics3DModel::CAbstractBody>& ptr_body,
                         const std::function<btVector3()>& fn_get_field,
                         const btTransform& c_offset) :
            Body(ptr_body),
            GetField(fn_get_field),
            Offset(c_offset) {}
         /* Members */
         std::shared_ptr<CDynamics3DModel::CAbstractBody> Body;
         std::function<btVector3()> GetField;
         btTransform Offset;
      };    

      std::vector<SMagneticDipole> m_vecDipoles;
   };
   
   /****************************************/
   /****************************************/
   
}

#endif
