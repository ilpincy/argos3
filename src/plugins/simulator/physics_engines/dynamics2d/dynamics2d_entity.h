/**
 * @file <argos3/plugins/simulator/physics_engines/dynamics2d_entity.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_ENTITY_H
#define DYNAMICS2D_ENTITY_H

namespace argos {
   class CDynamics2DEngine;
}

#include <argos3/core/simulator/physics_engine/physics_engine_entity.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/chipmunk-physics/include/chipmunk.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CDynamics2DEntity : public CPhysicsEngineEntity {

   public:

      typedef std::map<std::string, CDynamics2DEntity*> TMap;

   public:

      CDynamics2DEntity(CDynamics2DEngine& c_engine,
                        CEmbodiedEntity& c_entity) :
         CPhysicsEngineEntity(c_entity),
         m_cEngine(c_engine) {}
      virtual ~CDynamics2DEntity() {}

      virtual bool CheckIntersectionWithRay(Real& f_distance,
                                            const CRay& c_ray) const = 0;

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false) {
         return false;
      }

      virtual void Reset() = 0;

      virtual void UpdateEntityStatus() = 0;
      virtual void UpdateFromEntityStatus() = 0;

   protected:

      CDynamics2DEngine& m_cEngine;
      CVector3 m_cSpacePosition;
      CQuaternion m_cSpaceOrientation;

   };

}

#endif
