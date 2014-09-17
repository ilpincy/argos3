/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_MODEL_H
#define DYNAMICS2D_MODEL_H

namespace argos {
   class CDynamics2DEngine;
}

#include <argos3/core/simulator/physics_engine/physics_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/chipmunk-physics/include/chipmunk.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CDynamics2DModel : public CPhysicsModel {

   public:

      typedef std::map<std::string, CDynamics2DModel*> TMap;

   public:

      CDynamics2DModel(CDynamics2DEngine& c_engine,
                       CEmbodiedEntity& c_entity) :
         CPhysicsModel(c_engine, c_entity),
         m_cDyn2DEngine(c_engine) {}

      virtual ~CDynamics2DModel() {}

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false) {
         return false;
      }

      virtual void Reset() = 0;

   protected:

      CDynamics2DEngine& m_cDyn2DEngine;

   };

}

#endif
