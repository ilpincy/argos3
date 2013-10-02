/**
 * @file <argos3/plugins/simulator/physics_engines/physx/physx_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSX_MODEL_H
#define PHYSX_MODEL_H

namespace argos {
   class CPhysXEngine;
}

#include <argos3/core/simulator/physics_engine/physics_model.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_engine.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/core/utility/math/quaternion.h>

namespace argos {

   class CPhysXModel : public CPhysicsModel {

   public:

      typedef std::map<std::string, CPhysXModel*> TMap;

   public:

      CPhysXModel(CPhysXEngine& c_engine,
                  CEmbodiedEntity& c_entity) :
         CPhysicsModel(c_engine, c_entity),
         m_cPhysXEngine(c_engine) {}

      virtual ~CPhysXModel() {}

      virtual bool CheckIntersectionWithRay(Real& f_distance,
                                            const CRay3& c_ray) const = 0;

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false) {
         return false;
      }

      virtual void Reset() = 0;

      virtual void UpdateEntityStatus() = 0;
      virtual void UpdateFromEntityStatus() = 0;

      inline CPhysXEngine& GetPhysXEngine() {
         return m_cPhysXEngine;
      }

   private:

      CPhysXEngine& m_cPhysXEngine;

   };

}

#endif
